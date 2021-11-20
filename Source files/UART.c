#include "lpc214x.h"
#include "stdio.h"
void UartInit(unsigned int baudrate)	  //setting the baud rate for 115200 baud
{
 int i,FDiv;

 i = PINSEL0;			   // read the value of the pins function
 i = i & 0xFFF0FFF0;	   // modify the value
 PINSEL0 = (i | 0x50005);	   // set the functionality of the TxD and Rxd Pin	:01

//set the baud rate
	U1LCR = 0x83;						// Line control register :DLAB=1 ; 8 bits ; 1 stop bit ; no parity
	FDiv = (15000000 / 16 ) / baudrate ; //
	U1DLM = FDiv /256;					//0x00;
	U1DLL = FDiv %256;					//0x97;
	U1LCR = 0x03;						// Line control register :DLAB=0 ; 8 bits ; 1 stop bit ; no parity
	U1TER = 0x80;
}

int UART_GetChar(void)
{
	while(!(U1LSR & 0x1));		   		//wait till data is received im receive register
return(U1RBR);
}

int UART_PutChar(unsigned char ch)
{	
 while (!(U1LSR & 0x20));			//wait for transmit register to become empty
if (ch == '\n') 
 {
    ch = 0x0D;                       //output <CR>
 } 
U1THR = ch;
return(ch);
}


/* 
** Re-targetting to allow to redefine low-level I/O routines.
** Implements the functionality required to adapt character I/O functions such as printf() and scanf().
** Refer The template file .../Keil/Arm/Startup/RETARGET.C
*/
int fputc(int ch, FILE *f) {
  return (UART_PutChar(ch));
}

struct __FILE { int handle; 		/* Add whatever you need here */ };
FILE __stdout;
  
