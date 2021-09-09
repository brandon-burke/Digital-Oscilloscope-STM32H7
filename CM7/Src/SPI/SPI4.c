#include "main.h"

/*This is the main SPI file that does the low level data control and configuration using the SPI
 * protocol. So this can stop, start, and change outgoing data lengths*/

void ILI9341_SPI4_init() {
	/*Getting the SPI GPIO pins ready*/
	ILI9341_GPIO_init();

	/*Turning on the SPI4 block clk and resetting its block*/
	RCC->APB2ENR |= RCC_APB2ENR_SPI4EN;
	RCC->APB2RSTR |= RCC_APB2RSTR_SPI4RST;
	RCC->APB2RSTR &= ~(RCC_APB2RSTR_SPI4RST);

	/* -Setting the master baudrate you can change how fast the LCD updates here (Assumes the MBR bits are all 0 to start)
	 * -Disabling CRC calculation
	 * -Disabling DMA Transmission
	 * -Disabling DMA Reception
	 * -Setting the FIFO Threshold lvl to 8 bits
	 * -Setting the packet size to 8 bits
	 * -*/
	SPI4->CFG1 |= LL_SPI_BAUDRATEPRESCALER_DIV4;		//128MHZ go as far as div4, while 64MHZ can do
	SPI4->CFG1 &= ~(SPI_CFG1_CRCEN);
	SPI4->CFG1 &= ~(SPI_CFG1_TXDMAEN);
	SPI4->CFG1 &= ~(SPI_CFG1_RXDMAEN);
	SPI4->CFG1 |= LL_SPI_FIFO_TH_08DATA;
	SPI4->CFG1 |= LL_SPI_DATAWIDTH_8BIT;

	/* -Configuring for FULL duplex
	 * -NOT allowing multi-master communication
	 * -Configuring as the MASTER
	 * -Configuring for MSB first
	 * -Configuring the clk to be 0 when idle
	 * -Configuring the clk to capture on the first edge
	 * -Configuring SS pin to be software managed*/
	SPI4->CFG2 &= ~(SPI_CFG2_COMM);
	SPI4->CFG2 |= SPI_CFG2_SSOE;
	SPI4->CFG2 |= SPI_CFG2_SSM;
	SPI4->CFG2 |= SPI_CFG2_MASTER;
	SPI4->CFG2 &= ~(SPI_CFG2_LSBFRST);
	SPI4->CFG2 &= ~(SPI_CFG2_CPOL);
	SPI4->CFG2 &= ~(SPI_CFG2_CPHA);

	/* -Enabling the SPI
	 * -Starting the master transfer*/
	SPI4->CR1 |= SPI_CR1_SPE;
	SPI4->CR1 |= SPI_CR1_CSTART;

	return;
}


/*Resets the CS bit (active low) to allow SCK, commands and parameters to be read by the LCD*/
void ILI9341_SPI_Start() {
	LL_GPIO_ResetOutputPin(GPIOE, ILI9341_SS_PIN);
	return;
}

/*Setting the CS bit high (active low) to ignore commands, SCK, or parameters by the LCD */
void ILI9341_SPI_Stop() {
	LL_GPIO_SetOutputPin(GPIOE, ILI9341_SS_PIN);
	return;
}


/*Changing the data width for the SPI to 16bit*/
void ILI9341_set16() {
	LL_SPI_Disable(SPI4);
	LL_SPI_SetFIFOThreshold(SPI4, LL_SPI_FIFO_TH_16DATA);
	LL_SPI_SetDataWidth(SPI4, LL_SPI_DATAWIDTH_16BIT);
	LL_SPI_Enable(SPI4);
	LL_SPI_StartMasterTransfer(SPI4);
	return;
}

/*Changing the data width for the SPI to 8bit*/
void ILI9341_set8() {
	LL_SPI_Disable(SPI4);
	LL_SPI_SetFIFOThreshold(SPI4, LL_SPI_FIFO_TH_08DATA);
	LL_SPI_SetDataWidth(SPI4, LL_SPI_DATAWIDTH_8BIT);
	LL_SPI_Enable(SPI4);
	LL_SPI_StartMasterTransfer(SPI4);
	return;
}

/*Function to wait until all data has been transferred to the LCD from the MCU*/
void ILI9341_wait() {
	while (!(LL_SPI_IsActiveFlag_TXC(SPI4))) {}


	return;
}
