#include "main.h"

/*This contains all the transmission commands for the SPI protocol and is what will
 * invoke the actual sending of data between the SPI and MCU.*/

/*This will take in a command and send it to the lcd. All commands are 8bits*/
void Transmit_Cmd(uint8_t cmd) {

	LL_GPIO_ResetOutputPin(GPIOE, ILI9341_DC_PIN);	//Resetting the D/C pin to 0. This means we are sending a command
	ILI9341_SPI_Start();							//This will set the CS/SS pin to low (active low). This allows the LCD to receive commands and data
	LL_SPI_TransmitData8(SPI4, cmd);				//Sending the command to the SPI
	ILI9341_wait();									//Waiting until the command has been sent
	ILI9341_SPI_Stop();								//This will set the CS/SS pin to high (active low). LCD will ignore commands and data

	return;
}


/*This will send a single data/parameter to the LCD. This will send 8 bit data*/
void Transmit_Data8(uint8_t data) {
	LL_GPIO_SetOutputPin(GPIOE, ILI9341_DC_PIN);	//Setting the D/C pin to 1. LCD is assuming we are sending data/parameter.
	ILI9341_SPI_Start();							//Resetting the CS/SS pin to low (active low). Allows the LCD to receive data and commands
	LL_SPI_TransmitData8(SPI4, data);				//Transmitting 8 bits of data to LCD
	ILI9341_wait();									//Waiting until the data is sent to the LCD
	ILI9341_SPI_Stop();								//Setting the CS/SS pin to high (active low). LCD will ignore commands and data

	return;
}

/*This will send a single data/parameter to the LCD. This will send 16 bit data*/
void Transmit_Data16(uint16_t data) {
	LL_GPIO_SetOutputPin(GPIOE, ILI9341_DC_PIN);	//Setting the D/C pin to 1. LCD will interpret bits as data/parameter.
	ILI9341_SPI_Start();							//Resetting the CS/SS pin to low (active low). Allows the LCD to receive data and commands
	LL_SPI_TransmitData16(SPI4, data);				//Transmitting 16 bits of data to LCD
	ILI9341_wait();
	ILI9341_SPI_Stop();								//Setting the CS/SS pin to high (active low). LCD will ignore commands and data

	return;
}

/*This function will continuously send the same data a certain number of times.
 * This currently sees alot of use filling the screen*/
void Transmit_Data16_Cont(uint16_t data, uint32_t num_of_writes) {
	LL_GPIO_SetOutputPin(GPIOE, ILI9341_DC_PIN);	//Setting the D/C pin to 1. LCD will interpret bits as data/parameter.
	ILI9341_SPI_Start();							//Resetting the CS/SS pin to low (active low). Allows the LCD to receive data and commands

	/*Looping until we finish sending the data*/
	while (num_of_writes != 0) {
		LL_SPI_TransmitData16(SPI4, data);			//Transmitting 16 bits of data to LCD
		ILI9341_wait();
		num_of_writes--;
	}

	ILI9341_SPI_Stop();								//Setting the CS/SS pin to high (active low). LCD will ignore commands and data

	return;


}

