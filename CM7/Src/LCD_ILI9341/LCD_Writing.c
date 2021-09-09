#include "main.h"
#include "LCD_CMDs.h"

/*These are the basic functions to be able to actual draw to the LCD*/


/*This function will draw a single pixel on the LCD according to the color and x and y locations*/
void LCD_set_pixel(uint16_t color, uint16_t x, uint16_t y) {
	LCD_Set_Address_Window(x, y, x, y);
	Transmit_Cmd(ILI9341_MEM_WRITE);		//Sending command to begin memory write
	ILI9341_set16();						//Setting the SPI to accept 16bit data

	Transmit_Data16(color);					//Sending the color information

	ILI9341_set8();							//Setting the SPI to accept 8bit data
	Transmit_Cmd(0x00);						//Sending a noop to end frame right
	return;
 }

/*This will fill the screen with whatever color is passed*/
void LCD_fill_screen(uint16_t color) {
	LCD_Set_Address_Window(0, 0, LCD_WIDTH, LCD_HEIGHT);		//Setting the writing window to the entire screen (320x240)
	Transmit_Cmd(ILI9341_MEM_WRITE);							//Sending the command to begin memory write
	ILI9341_set16();											//Setting the SPI to accept 16bit data

	Transmit_Data16_Cont(color, (LCD_WIDTH*LCD_HEIGHT));

	ILI9341_set8();
	Transmit_Cmd(0x00);

	return;
}

/*Draws a horizontal line across the screen according to the Y position*/
void draw_horizontal_line(uint16_t color, uint16_t y_pos) {

	/*-Setting the address windows to only the space where the line will be drawn
	 *-Sending command that the next data will go to memory
	 *-Setting the data to be a width of 16bits on the SPI
	 *-Sending the data to draw the line*/
	LCD_Set_Address_Window(0, y_pos, LCD_WIDTH, y_pos);
	Transmit_Cmd(ILI9341_MEM_WRITE);
	ILI9341_set16();
	Transmit_Data16_Cont(color, LCD_WIDTH);

	/*Setting the SPI data size back to command length*/
	ILI9341_set8();
	Transmit_Cmd(0x00);

	return;
}

/*Draws a horizontal line across the screen according to the x position*/
void draw_vertical_line(uint16_t color, uint16_t x_pos) {

	/*-Setting the address windows to only the space where the line will be drawn
	 *-Sending command that the next data will go to memory
	 *-Setting the data to be a width of 16bits on the SPI
	 *-Sending the data to draw the line*/
	LCD_Set_Address_Window(x_pos, 0, x_pos, LCD_HEIGHT);
	Transmit_Cmd(ILI9341_MEM_WRITE);
	ILI9341_set16();
	Transmit_Data16_Cont(color, LCD_HEIGHT);

	/*Setting the SPI data size back to command length*/
	ILI9341_set8();
	Transmit_Cmd(0x00);

	return;
}
