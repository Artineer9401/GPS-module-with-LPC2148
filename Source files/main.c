#include<lpc214x.h>
#include "stdio.h"
#include "lcd.h"
#include "UART.h"


/* 	This function fetches a string from the serial port using UART_GetChar() function**	In case of GPS, the co-ordinate string starts with a character '$'
**	And ends with a character '*' followed by CRC
*/
void GPS_string(unsigned char *temp)
{
	unsigned int i=0;
	do
	{
		temp[i] = UART_GetChar();

	}while(temp[i++] != '*');			
	temp[i] = '\0';							
}


/*	This function takes the latitude and longitude string pointers as the input parameters
**	And displays them on the LCD in a proper fashion
*/
void GPS_display(unsigned char *lat, unsigned char *lon)
{
	unsigned int i;

	LCD_cmd(0x84);		  	//cursor on 1st line 5th position
	for(i=0;i<2;i++)
		LCD_data(*lat++);			//display degrees
	LCD_data('\'');				//degree symbol
	for(i=0;i<8;i++)			//display rest of the string
		LCD_data(*lat++);

	LCD_cmd(0xC4);			//cursor on 2nd line 5th position
	for(i=0;i<3;i++)
		LCD_data(*lon++);			//display degrees
	LCD_data('\'');				//degree icon
	for(i=0;i<8;i++)			//display rest of the string
		LCD_data(*lon++);			
}


/*	In the main function, we will continuously keep checking the serial port for character '$'
**	If the character '$' is received, fetch all the characters between '$' and '*'
**	Extract the latitude and longitude details from the string and display it on the LCD
**	Eg:	$GPGGA,155635,3730.4379,N,02357.4137,E,1,04,5.6,3.8,M,34.5,M,,*41
*/
int main()
{
	unsigned char ch, temp[20],lat[10],lon[10];
	unsigned int i=0,j=0;
	LCD_init();
	UartInit(9600);
	LCD_display(1,1,"GPS Program");	  
	delay(200000);
	LCD_cmd(0x01);						 //Clear Display
	LCD_display(1,1,"LAT:");
	LCD_display(2,1,"LON:");
	while(1)
	{
		if((ch = UART_GetChar()) == '$')	//if '$' is received
		{
			GPS_string(temp);				 //fetch all characters upto '*'
				if(temp[2] == 'G' && temp[3] == 'G' && temp[4] == 'A') //GGA
				{
					i=0;
					j=0;
				while(temp[i++] != ',');	 	//$GPGGA (ignore)
				while(temp[i++] != ',');		//Time Stamp (ignore)
					
				while(temp[i] != ',')			//Capture Latitude
					lat[j++] = temp[i++];
				
				while(temp[i++] != ',');		//North/South
				lat[j++] = temp[i++];
			
				j=0;
				while(temp[i++] != ',');
				while(temp[i] != ',')			//Capture Longitude
					lon[j++] = temp[i++];
				while(temp[i++] != ',');		//East/West
				lon[j++] = temp[i++];
				//(ignore the rest part of the string)
				GPS_display(lat,lon);		

			//display latitude and longitude on the LCD
					}
		}
	}
	return 0;
}
