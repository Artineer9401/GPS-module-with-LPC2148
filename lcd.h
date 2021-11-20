#ifndef LCD_H
#define LCD_H

void delay(unsigned int time);
void LCD_data(unsigned char);			   //function to send data
void LCD_cmd(unsigned char);			   //function to send command
void LCD_init(void);
void LCD_display(int, int, unsigned char*);

#endif
