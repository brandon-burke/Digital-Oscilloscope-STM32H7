#include "main.h"

/*This is the main file to configure the DMA. MAKE NOTE, depending if you use the flash or SRAM
 * storage will determine which DMA peripheral you can use since not all DMAs use the same bus as
 * either the flash or SRAM*/

void DMA_ADC3_init(uint32_t *data) {

	/* -Turning on the peripheral clk for DMA1
	 * -Resetting the DMA1 block
	 * -Stopping the reset on the DMA1 block*/
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;
	RCC->AHB1RSTR |= RCC_AHB1RSTR_DMA1RST;
	RCC->AHB1RSTR &= ~(RCC_AHB1RSTR_DMA1RST);
	delay_ms(20);	//Letting things mesh

	/* -Enabling the transfer complete interrupt
	 * -Enabling the interrupt in the NVIC*/
	DMA1_Stream0->CR |= DMA_SxCR_TCIE;
	NVIC_EnableIRQ(DMA1_Stream0_IRQn);

	 /* -Enabling Circular mode as the ADC will be sending continuous data
	 * -Memory destination will be incremented after each data transfer
	 * -Setting the peripheral data size to 16bits
	 * -Setting the memory location data size to 16bits
	 * -Giving the DMA a high priority*/
	DMA1_Stream0->CR |= DMA_SxCR_CIRC;
	DMA1_Stream0->CR |= LL_DMA_MEMORY_INCREMENT;
	DMA1_Stream0->CR |= LL_DMA_PDATAALIGN_HALFWORD;
	DMA1_Stream0->CR |= LL_DMA_MDATAALIGN_HALFWORD;
	DMA1_Stream0->CR |= LL_DMA_PRIORITY_HIGH;

	/* -Setting the number of data items to be transferred to 320
	 * (but we are in circular mode so does this matter?)*/
	DMA1_Stream0->NDTR = (uint16_t)320;

	/* -Setting the destination memory address to the passed pointer address*/
	DMA1_Stream0->M0AR = (uint32_t)data;

	/* -Setting the peripheral source addy to ADC3*/
	DMA1_Stream0->PAR = (uint32_t) &(ADC3->DR);

	/* -Setting the channel 0s dma request coming from ADC3*/
	DMAMUX1_Channel0->CCR |= LL_DMAMUX1_REQ_ADC3;

	/* -Enabling the channel 0 stream*/
	DMA1_Stream0->CR |= DMA_SxCR_EN;

	return;
}

















