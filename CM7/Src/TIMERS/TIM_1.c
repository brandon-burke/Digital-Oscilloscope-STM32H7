#include "main.h"

/*This is the TIMER to test and see if the oscilloscope is producing good results*/

/*Simply connects the timers output to pin PE11*/
void TIM1_PE11_connect() {
	//Enabling GPIOE peripheral clk and resetting the block
	RCC->AHB4ENR |= RCC_AHB4ENR_GPIOEEN;
	RCC->AHB4RSTR |= RCC_AHB4RSTR_GPIOERST;
	RCC->AHB4RSTR &= ~RCC_AHB4RSTR_GPIOERST;

	//Setting to alternate mode
	GPIOE->MODER |= (0x1UL << 23U);
	GPIOE->MODER &= ~(0x1UL << 22U);

	//Push pull
	GPIOE->OTYPER &= ~(0x1UL << 11U);

	//Very Fast output
	GPIOE->OSPEEDR |= (0x3UL << 22U);

	//no pull up or down
	GPIOE->PUPDR &= ~(0x3UL << 22U);

	//Configured what port needed he the alternate function
	GPIOE->AFR[1] |= (0x1UL << 12U);
	GPIOE->AFR[1] &= ~(0x1UL << 13U);

	return;
}

/*This function will change the period of a pulse and the interval/duration
 * of a pulse. The interval will indicate when the pulse will occur essentially*/
void TIM1_PWM_config(uint16_t period, uint16_t interval) {
	//Latch value for the counter
	TIM1->ARR = (uint16_t)period;

	//CCR value to determine when that timer will pulse
	TIM1->CCR2 = (uint16_t)interval;

	//Transferring values from shadow registers
	TIM1->EGR |= TIM_EGR_UG;

	return;
}

/*This will configure the timer*/
void TIM1_init() {
	//Turning on the peripheral clk
	RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
	RCC->APB2RSTR |= RCC_APB2RSTR_TIM1RST;
	RCC->APB2RSTR &= ~(RCC_APB2RSTR_TIM1RST);

	//Buffering ARR register
	TIM1->CR1 |= TIM_CR1_ARPE;

	//Setting Capture control 2 to output
	TIM1->CCMR1 &= ~(TIM_CCMR1_CC2S);

	//Setting to PWM2 mode
	TIM1->CCMR1 |= (TIM_CCMR1_OC2M);
	TIM1->CCMR1 &= ~(0x1UL << 24U);

	//Pre-loading CCR2
	TIM1->CCMR1 |= TIM_CCMR1_OC2PE;

	//Setting up the prescaler (1KHZ)
	TIM1->PSC = (uint16_t)(64000 - 1);

	//Latch value for the counter
	TIM1->ARR = (uint16_t)200;

	//CCR value to determine when that timer will pulse
	TIM1->CCR2 = (uint16_t)100;

	//Enabling capture control register 2 output
	TIM1->CCER |= TIM_CCER_CC2E;

	//Enabling master output
	TIM1->BDTR |= TIM_BDTR_MOE;

	//Transferring values from shadow registers
	TIM1->EGR |= TIM_EGR_UG;

	//Turning on the TIMER
	TIM1->CR1 |= TIM_CR1_CEN;

	return;
}

