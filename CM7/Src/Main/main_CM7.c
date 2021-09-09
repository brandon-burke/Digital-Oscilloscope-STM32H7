#include <stdio.h>
#include <stdlib.h>
#include "main.h"

/*This project I left of successfully getting digital signals to show up on the LCD for a simple look
 * at what your digital logic signals look like. Next I need to clean up the LCD background to have it
 * include grid lines and such to make it look more official. Later on I would also like an easier way to
 * scale the graph is I want a closer look at a signal.
 */

	/*These are the pin connections between the LCD and MCU
	  PE2 = SCK		(alternate function) Orange
	  PE3 = D/C 	(General Output) White/Grey
	  PE4 = CS/SS	(General Output) Yellow
	  PE5 = RESET 	(General Output) Brown
	  PE6 = MOSI  	(alternate function) Blue

	  PC2 - This is the ADC sampling pin
	  PE11 - This is if you want to connect to TIMER1 and test the oscilloscope
	 */


uint16_t raw_data[320];				//This is the raw bytes that come from the ADC
Coordinate converted_data[320];		//Actual data that's able to be plotted on a graph
Coordinate *converted;

int main(void)
 {

	int i;

	/*Zeroing out ADC buffer for raw data*/
	for (i = 0; i < 320; i++) {
		raw_data[i] = 0;
	}

	/*Init Setup of delay (0 is 64Mhz clk and 1 is 128MHZ clk) note of the commented
	 * function below is uncommented the MCU will run at 128MHz and the Delay_Init function
	 * will need to have the right parameter passed to ensure the delays are accurate*/
	//overclock_128Mhz();
	Delay_Init(0);

	/*TIMER1 Setup to test the oscilloscope*/
	TIM1_PE11_connect();
	TIM1_init();
	TIM1_PWM_config(200, 100);		//The parameters are the period and interval respectively

	/*ADC3 Setup*/
	ADC3_PC2_enable();
	ADC3_PERPH_CLK_ENABLE();
	ADC3_calibrate();
	ADC3_enable();
	ADC3_configure();
	ADC3_TRGO_config();
	TIM8_init_TRGO();

	/*DMA Setup*/
	DMA_ADC3_init((uint32_t *)&raw_data[0]);

	/*LCD setup (The value passed will change the LCD screen color and let you know things are working)*/
	LCD_init();
	LCD_fill_screen(0xFF00);

	/*ADC will start and sampling will begin after this*/
	ADC3_start();

	while (1)
	{

	}

  return 0;
}


/* This interrupt will fire when the DMA has finished filling up the raw data array.
 * Its then going to try to have the data converted and put into the converted array.
 * Then the LCD will read this converted array will be plotted onto the lcd.*/
void DMA1_Stream0_IRQHandler () {

	converted = &converted_data[0];

	/*Stopping any conversions of the ADC*/
	ADC3_stop();

	/* -Clearing the Transfer Complete interrupt flag
	 * -Clearing the NVIC pending interrupt for this as well*/
	DMA1->LIFCR |= DMA_LIFCR_CTCIF0;
	NVIC_ClearPendingIRQ(DMA1_Stream0_IRQn);

	/* -Start converting the raw data into actual voltage levels*/
	Convert_Raw_Data(&converted_data[0], &raw_data[0]);

	/*Wiping the screen to update it and then plot the new pts*/
	LCD_fill_screen(0xFFFF);
	plot_pts(converted_data, 1.0, 20.0); //1.0, 20.0

	/* -Starting the ADC again, but must check that the ADC has actually stopped*/
	while ((ADC3->CR & ADC_CR_ADSTART)== 1) {	}
	ADC3_start();

	return;
}

/*Using the PLL to create a system clk frequency of 128MHZ to help the screen update faster. NOTE
 * be careful on how fast the clk speed runs and the SPI baudrate because you can get weird results
 * on the LCD*/
void overclock_128Mhz() {
	/*-Clearing the pllon bits
	 *-CLearing the HSIDIV bits
	 *-Setting the HSIDIV to divided its clk by 4 (now a 16MHz clk)
	 */
	RCC->CR &= ~(0x15UL << 24U);
	RCC->CR &= ~(0x3UL << 3U);
	RCC->CR |= (0x2UL << 3U);

	/*-Waiting until the HSIRDY bit is set*/
	while ((RCC->CR & RCC_CR_HSIRDY_Msk) == 0) {}

	//Assuming sys_clk is already HSI by default


	/*-Selecting the reference clk to be HSI
	 *-Clearing the DIVM1 bits
	 *-Configuring DIVM1 to div 1 or bypass HSI clk
	 */
	RCC->PLLCKSELR &= ~(RCC_PLLCKSELR_PLLSRC);
	RCC->PLLCKSELR &= ~(RCC_PLLCKSELR_DIVM1);
	RCC->PLLCKSELR |= (0x1UL << 4U);


	/*-Configuring integer mode and wide range VCO range
	 *-Configuring for range of 8-16MHz
	 *-Enabling Post divider for DIV1P
	 */
	RCC->PLLCFGR &= ~(RCC_PLLCFGR_PLL1FRACEN | RCC_PLLCFGR_PLL1VCOSEL);
	RCC->PLLCFGR |= (RCC_PLLCFGR_PLL1RGE);
	RCC->PLLCFGR |= (RCC_PLLCFGR_DIVP1EN);


	/*-Clearing the DIVP and DIVN bits
	 *-Multiplying the clk(16MHz) by 16 in DIVN then dividing by 2 in DIVP to get 128MHZ*/
	RCC->PLL1DIVR &= ~(0xFFFF);
	RCC->PLL1DIVR |= (0x020FUL << 0U);

	/*-Turning on the PLLxOn bits
	 *-Waiting until PLL1RDY is set meaning the clk is ready */
	RCC->CR |= (0x15UL << 24U);
	while ((RCC->CR & RCC_CR_PLL1RDY) == 0){}

	/*-Switching sys_clk to HSI, which is should be by default*/
	RCC->CFGR &= ~(RCC_CFGR_SW);
	RCC->CFGR |= (0x3UL << 0U);

	SystemCoreClockUpdate();

	return;
}

/*This part below is in progress and needs external inverting HW*/

/*This interrupt will fire when conversion starts for the signal. If the signal is negative
 * then it will apply the negative sign to the incoming signal. This is due to the MCU only being
 * able to take in positive voltages.*/
void ADC_IRQHandler() {


	/*Determine if signal is neg or pos
	if (signal == 0) {
		*converted = 1.0;
	} else {
		*converted = -1.0;
	}

	converted++;

	NVIC_ClearPendingIRQ(ADC3_IRQn);
	*/
	return;
}
