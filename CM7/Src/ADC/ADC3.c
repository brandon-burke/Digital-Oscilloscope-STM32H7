#include "main.h"

/*This is the main file for all ADC3 configuration*/

/*This function will enable the GPIOC clk and setup the PC2 port
 * to be the input for ADC3*/
void ADC3_PC2_enable() {

	/*Turning on the peripheral clk for GPIOC and resetting the block*/
	RCC->AHB4ENR |= RCC_AHB4ENR_GPIOCEN;
	RCC->AHB4RSTR |= RCC_AHB4RSTR_GPIOCRST;
	RCC->AHB4RSTR &= ~(RCC_AHB4RSTR_GPIOCRST);
	delay_ms(20);	//Letting things mesh

	/*Changing port 2 to analog mode*/
	GPIOC->MODER |= GPIO_MODER_MODE2;
	delay_ms(20);	//Letting things mesh

	return;
}

/*This will enable the ADC3 Bus clock and reset the block*/
void ADC3_PERPH_CLK_ENABLE() {

	RCC->AHB4ENR |= RCC_AHB4ENR_ADC3EN;
	RCC->AHB4RSTR |= RCC_AHB4RSTR_ADC3RST;
	RCC->AHB4RSTR &= ~RCC_AHB4RSTR_ADC3RST;
	delay_ms(20);

	return;
}

/*This will turn on the peripheral clk for ADC3 and set it up to be calibrated*/
void ADC3_calibrate() {

	/*Setting the ADC3 clk to the system clk (default 64Mhz) and by default this will get
	 * divided by 2. As well Setting the reference voltage to come from internal (3.3V)*/
	ADC3_COMMON->CCR |= LL_ADC_CLOCK_SYNC_PCLK_DIV1;
	ADC3_COMMON->CCR |= LL_ADC_PATH_INTERNAL_VREFINT;
	delay_ms(20);	//Letting things mesh


	/*-Disable Deep Power Down
	 *-Enable internal voltage regulator
	 *-Must wait the setup time (t_ADCVREG_SETUP == 10us) for the regulator before calibration
	 */
	ADC3->CR &= ~(ADC_CR_DEEPPWD);
	ADC3->CR |= ADC_CR_ADVREGEN;
	delay_us(10);

	/*-Single_ended input
	 *-Linear Calibration enabled
	 *-Start Calibration
	 */
	ADC3->CR &= ~(ADC_CR_ADCALDIF);
	ADC3->CR |= ADC_CR_ADCALLIN;
	ADC3->CR |= ADC_CR_ADCAL;

	/*-Waiting until calibration is complete*/
	while ((READ_BIT(ADC3->CR, ADC_CR_ADCAL)) == 1){
		//do nothing
	}

	delay_ms(20); //Letting things mesh

	return;
}

/*This will enable ADC3 but will NOT start conversion. This function needs to be called first
 * and then you can configure ADC3 to your settings*/
void ADC3_enable() {

	/*-Clearing the ADRDY flag
	 *-Enabling the ADC3
	 */
	ADC3->ISR &= ~(ADC_ISR_ADRDY);
	ADC3->CR |= ADC_CR_ADEN;

	/*Waiting until the ADRDY flag is set*/
	while ((READ_BIT(ADC3->ISR, LL_ADC_FLAG_ADRDY)) == 0){
	}

	/*-Clearing the ADRDY flag (clears by setting the bit)*/
	ADC3->ISR = ADC_ISR_ADRDY;

	delay_ms(20); //Letting things mesh

	return;
}

/*This will configure the ADC3 to use the fast channel 0, Sample at 810.5 ADC cycles,
 * Continuously convert, overwrite bits, using single shot for the DMA, and only convert 1 channel which in this
 * case is channel 0 for right now*/
void ADC3_configure() {

	/*-Setting BOOST mode of 50Mhz since our clk is only 8 Mhz after sys_clk/6*/
	ADC3->CR |= LL_ADC_BOOST_MODE_50MHZ;

	/*-Single Conversion Mode
	 *-Overrun enabled
	 *-Triggering on the rising edge of the ext trigger
	 *-The trigger is timer 8 trgo2
	 *-DMA one shot mode
	 */
	ADC3->CFGR &= ~(ADC_CFGR_CONT);
	ADC3->CFGR &= ~(ADC_CFGR_OVRMOD);
	ADC3->CFGR |= LL_ADC_REG_TRIG_EXT_RISING;
	ADC3->CFGR |= LL_ADC_REG_TRIG_EXT_TIM8_TRGO2;
	ADC3->CFGR |= LL_ADC_REG_DMA_TRANSFER_UNLIMITED;

	/*-Sampling channel 0 at 810.5 clk cycles*/
	ADC3->SMPR1 |= (0x7UL << 0U);

	/*-Preselecting Channel 0
	 *-Time to mesh*/
	ADC3->PCSEL |= ADC_PCSEL_PCSEL_0 ;
	delay_ms(20);

	return;
}

/*Here we are just setting timer 8 to be the trigger for a conversion. When the
 * Timer finishes counting then the ADC will sample the signal.*/
void ADC3_TRGO_config() {
	LL_ADC_REG_SetTriggerSource(ADC3, LL_ADC_REG_TRIG_EXT_TIM8_TRGO2);
	LL_ADC_REG_SetTriggerEdge(ADC3, LL_ADC_REG_TRIG_EXT_RISING);
	return;
}

/*This function executes then the ADC starts to convert. */
void ADC3_start() {
	LL_ADC_REG_StartConversion(ADC3);

	return;
}

/*Any ongoing conversions will stop after this is ran*/
void ADC3_stop() {
	LL_ADC_REG_StopConversion(ADC3);

	return;
}
