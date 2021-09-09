#include "main.h"

/*This is the file to setup the pins that will connect between the LCD and MCU and below are what pins
 * are used on the MCU and their counter part on the LCD with the SPI peripheral. */

	/*PE2 = SCK		(alternate function) Orange
	  PE3 = D/C 	(General Output) White/Grey
	  PE4 = CS/SS	(General Output) Yellow
	  PE5 = RESET 	(General Output) Brown
	  PE6 = MOSI  	(alternate function) Blue
	 */

void ILI9341_GPIO_init() {

	/*Setting up the ports for the D/C, RESET, and SS/CS. These are General Output Ports*/
	LL_GPIO_InitTypeDef general_output;
	general_output.Pin = LL_GPIO_PIN_3 | LL_GPIO_PIN_4 | LL_GPIO_PIN_5;
	general_output.Mode = LL_GPIO_MODE_OUTPUT;
	general_output.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	general_output.Pull = LL_GPIO_PULL_NO;
	general_output.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
	LL_GPIO_Init(GPIOE, &general_output);
	GPIOE->ODR = LL_GPIO_PIN_5 | LL_GPIO_PIN_4;		//Setting CS and RESET pins. LCD will not accept commands at this moment
	delay_ms(200);									//Letting everything mesh


	/*Setting up the ports for the SCK, and MOSI*/
	LL_GPIO_InitTypeDef alternate_output;
	alternate_output.Pin = LL_GPIO_PIN_2 | LL_GPIO_PIN_6;
	alternate_output.Mode = LL_GPIO_MODE_ALTERNATE;
	alternate_output.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	alternate_output.Pull = LL_GPIO_PULL_NO;
	alternate_output.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
	alternate_output.Alternate = LL_GPIO_AF_5;
	LL_GPIO_Init(GPIOE, &alternate_output);
	delay_ms(200);									//Letting everything mesh

	return;
}
