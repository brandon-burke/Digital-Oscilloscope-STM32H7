#include "main.h"

/*This is the file that contains the TIMER that will be triggering when the ADC will sample
 * according to how fast it counts up to a certain value*/

void TIM8_init_TRGO() {
	/* -Turning on the TIM8 peripheral clk
	 * -Resetting the TIM8 block
	 * -Turning off the reset for TIM8*/
	RCC->APB2ENR |= RCC_APB2ENR_TIM8EN;
	RCC->APB2RSTR |= RCC_APB2RSTR_TIM8RST;
	RCC->APB2RSTR &= ~(RCC_APB2RSTR_TIM8RST);

	/* -Configuring the external trigger on an update event*/
	TIM8->CR2 |= LL_TIM_TRGO2_UPDATE;

	/* -Reducing the incoming clock frequency*/
	TIM8->PSC = (uint16_t)(64000 - 1);

	/* -Configuring the time until the update occurs (depends on clk frequency of course)*/
	TIM8->ARR = (uint16_t)1;

	/* -Transferring the values into the registers*/
	TIM8->EGR |= TIM_EGR_UG;

	/*Enabling the Timer*/
	TIM8->CR1 |= TIM_CR1_CEN;

	return;
}





