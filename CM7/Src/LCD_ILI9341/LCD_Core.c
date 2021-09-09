#include "main.h"
#include "LCD_CMDS.h"

/*These are the core functions to get the LCD to startup correctly*/

void LCD_init() {

	ILI9341_SPI4_init();		//Initializing the SPI4
	LCD_Reset();				//Performs a hard reset on the display
	LCD_Configure();
	LCD_Exit_Standby();			//Getting the display out of sleep in mode (into sleep out) and turning on the display.
	return;
}

//Doing a hard reset on the lcd. Note the ILI9341 Reset is active low
void LCD_Reset() {
	LL_GPIO_ResetOutputPin(GPIOE, ILI9341_RST_PIN);
	delay_ms(100);
	LL_GPIO_SetOutputPin(GPIOE, ILI9341_RST_PIN);
	delay_ms(120);

	return;
}

/*Takes the display out of sleep mode and turns the display on*/
void LCD_Exit_Standby() {
	ILI9341_set8();
	Transmit_Cmd(ILI9341_Sleep_Out);
	delay_ms(150);
	Transmit_Cmd(ILI9341_Display_On);
	delay_ms(100);
	Transmit_Cmd(ILI9341_NORMAL_DISPLAY);

	return;
}


/*Configures the LCD for our needed settings. YEAH be more descriptive about this buddy*/
void LCD_Configure() {
	ILI9341_set8();

	/*More power controls*/
	Transmit_Cmd(ILI9341_PWR_A);
	Transmit_Data8(0x39);
	Transmit_Data8(0x2C);
	Transmit_Data8(0x00);
	Transmit_Data8(0x34);
	Transmit_Data8(0x02);

	Transmit_Cmd(ILI9341_PWR_B);
	Transmit_Data8(0x00);
	Transmit_Data8(0xC1);
	Transmit_Data8(0x30);

	Transmit_Cmd(ILI9341_PWR_SEQ);
	Transmit_Data8(0x64);
	Transmit_Data8(0x03);
	Transmit_Data8(0x12);
	Transmit_Data8(0x81);

	Transmit_Cmd(ILI9341_DRIVE_TIM_A);
	Transmit_Data8(0x85);
	Transmit_Data8(0x00);
	Transmit_Data8(0x78);

	Transmit_Cmd(ILI9341_DRIVE_TIM_B);
	Transmit_Data8(0x00);
	Transmit_Data8(0x00);

	Transmit_Cmd(ILI9341_PUMP_RATIO);
	Transmit_Data8(0x20);

	Transmit_Cmd(ILI9341_PWR_CTRL_1); //power control 1
	Transmit_Data8(0x23);

    Transmit_Cmd(ILI9341_PWR_CTRL_2); //power control 2
    Transmit_Data8(0x10);

    Transmit_Cmd(ILI9341_VCOM_CTRL_1); //vcom control 1
    Transmit_Data8(0x3E);//35
    Transmit_Data8(0x28);//3E

    Transmit_Cmd(ILI9341_VCOM_CTRL_2); //vcom control 2
    Transmit_Data8(0x86);

    /*Memory access*/
    Transmit_Cmd(ILI9341_MAC);
    Transmit_Data8(0x48);

    /*Vertical Scrolling*/
    Transmit_Cmd(ILI9341_VERT_SCROLL_ADDY);
    Transmit_Data8(0x00);

    /*Pixel Format*/
    Transmit_Cmd(ILI9341_PIXEL_FORMAT);
	Transmit_Data8(0x55);

	/*Controlling the Framerate*/
	Transmit_Cmd(ILI9341_FRAMERATE_CTRL_1);
	Transmit_Data8(0x00);
	Transmit_Data8(0x18);

	/*Display Control*/
	Transmit_Cmd(ILI9341_DISPLAY_FUNC_CTRL);
	Transmit_Data8(0x08);
	Transmit_Data8(0x82);
	Transmit_Data8(0x27);

	/*Disabling 3D Gamma*/
	Transmit_Cmd(ILI9341_3D_GAMMA_EN);
	Transmit_Data8(0x00);

	/*Configuring GAMMA*/
	Transmit_Cmd(ILI9341_GAMMA_CURVE);
	Transmit_Data8(0x01);

	Transmit_Cmd(ILI9341_POS_GAMMA);
	Transmit_Data8(0x0F);
	Transmit_Data8(0x31);
	Transmit_Data8(0x2B);
	Transmit_Data8(0x0C);
	Transmit_Data8(0x0E);
	Transmit_Data8(0x08);
	Transmit_Data8(0x4E);
	Transmit_Data8(0xF1);
	Transmit_Data8(0x37);
	Transmit_Data8(0x07);
	Transmit_Data8(0x10);
	Transmit_Data8(0x03);
	Transmit_Data8(0x0E);
	Transmit_Data8(0x09);
	Transmit_Data8(0x00);

	Transmit_Cmd(ILI9341_NEG_GAMMA);
	Transmit_Data8(0x00);
	Transmit_Data8(0x0E);
	Transmit_Data8(0x14);
	Transmit_Data8(0x03);
	Transmit_Data8(0x11);
	Transmit_Data8(0x07);
	Transmit_Data8(0x31);
	Transmit_Data8(0xC1);
	Transmit_Data8(0x48);
	Transmit_Data8(0x08);
	Transmit_Data8(0x0F);
	Transmit_Data8(0x0C);
	Transmit_Data8(0x31);
	Transmit_Data8(0x36);
	Transmit_Data8(0x0F);

	return;
}

/*This will set the window in which the MCU can access. Essentially where the MCU can draw on the LCD*/
void LCD_Set_Address_Window(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {


	ILI9341_set8();

	Transmit_Cmd(ILI9341_COL_SET);
	Transmit_Data8(y1 >> 8);		//y1 getting the upper 8 bits
	Transmit_Data8(y1);				//y1 getting the lower 8 bits, since this function takes uint8_t data is should chop off the upper bits
	Transmit_Data8(y2 >> 8);		//y2 getting the upper 8 bits
	Transmit_Data8(y2);				//y2 getting the lower 8 bits, since this function takes uint8_t data is should chop off the upper bits

	Transmit_Cmd(ILI9341_PAGE_SET);
	Transmit_Data8(x1 >> 8);		//x1 getting the upper 8 bits
	Transmit_Data8(x1);				//x1 getting the lower 8 bits, since this function takes uint8_t data is should chop off the upper bits
	Transmit_Data8(x2 >> 8);		//x2 getting the upper 8 bits
	Transmit_Data8(x2);				//x2 getting the lower 8 bits, since this function takes uint8_t data is should chop off the upper bits

	return;
}
