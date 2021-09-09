#include "main.h"

/*These functions allow us to have accurate millisecond or microsecond delays. These
 * can help with setup times peripherals might have when starting them up or
 * transferring data.*/

/*These variables help with prescaling the system clk speed to give us accurate timer delays*/
uint16_t prescaler_ms;
uint16_t prescaler_us;

/*As it stand we are assuming we are either using a 64MHz or 128Mhz clk.
 * If the parameter is 0 then we are doing 64Mhz and if it is 1 then its
 * 128Mhz*/
void Delay_Init(uint16_t clk_freq) {

	/* -Turning on the peripheral clk for TIM7
	 * -Resetting the block for TIM7
	 * -Turn off reset of TIM7 block*/
	RCC->APB1LENR |= RCC_APB1LENR_TIM7EN;
	RCC->APB1LRSTR |= RCC_APB1LRSTR_TIM7RST;
	RCC->APB1LRSTR &= ~RCC_APB1LRSTR_TIM7RST;

	/* -Buffering the Auto Reload Register*/
	TIM7->CR1 |= TIM_CR1_ARPE;

	 /* -Figuring out what the clk speed of the M7 is*/
	if (clk_freq == 0) {
		prescaler_ms = 63999;
		prescaler_us = 63;
	}
	else {
		prescaler_ms = 63999;		//NOTE THIS LEAVES OUR 128 MHZ clk to 2MHZ so well have to compensate in the ARR register
		prescaler_us = 128;
	}

	return;
}


void delay_ms(uint16_t delay){

	/*Off-setting the fact the the clk freq is 128MHZ*/
	if (prescaler_us == 128){
		delay = delay * 2;
	}

	/* -Setting the prescaler according to the clk freqency
	 * -Setting the delay amount
	 * -Generating an update to latch the autoreload and prescaler register
	 * -Clearing the Update flag
	 * -Enabling the Timer*/
	TIM7->PSC = prescaler_ms;
	TIM7->ARR = delay;
	TIM7->EGR |= TIM_EGR_UG;
	TIM7->SR &= ~TIM_SR_UIF;
	TIM7->CR1 |= TIM_CR1_CEN;


	/*Looping until we have finished counting the delay amount*/
	while ((TIM7->SR & TIM_SR_UIF) == 0) { }

	/*Disabling the Timer*/
	TIM7->CR1 &= ~TIM_CR1_CEN;

	return;
}

void delay_us(uint16_t delay){

	/* -Setting the prescaler according to the clk freqency
	 * -Setting the delay amount
	 * -Generating an update to latch the autoreload and prescaler register
	 * -Clearing the Update flag
	 * -Enabling the Timer*/
	TIM7->PSC = prescaler_us;
	TIM7->ARR = delay;
	TIM7->EGR |= TIM_EGR_UG;
	TIM7->SR &= ~TIM_SR_UIF;
	TIM7->CR1 |= TIM_CR1_CEN;


	/*Looping until we have finished counting the delay amount*/
	while ((TIM7->SR & TIM_SR_UIF) == 0) { }

	/*Disabling the TIMER*/
	TIM7->CR1 &= ~TIM_CR1_CEN;

	return;
}







