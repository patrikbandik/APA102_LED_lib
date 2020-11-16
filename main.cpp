#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#define SPI_DDR DDRB
#define CS      PINB2
#define MOSI    PINB3
#define MISO    PINB4
#define SCK     PINB5
#include <string.h>


void SPI_init()
{
	// set CS, MOSI and SCK to output
	SPI_DDR |= (1 << CS) | (1 << MOSI) | (1 << SCK);

	// enable SPI, set as master, and clock to fosc/128
	SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR1) | (1 << SPR0);
}

void SPI_masterTransmitByte(uint8_t data)
{
	// load data into register
	SPDR = uint8_t(data);

	// Wait for transmission complete
	while(!(SPSR & (1 << SPIF)));
}



int main(void)
{
	int ledcount = 8;
	int counter;
	int x = 0;
	SPI_init();
	/* Replace with your application code */
	while (1)
	{
		if (x == ledcount)
		{
			x = 0;
		}
		SPI_masterTransmitByte(0);
		SPI_masterTransmitByte(0);
		SPI_masterTransmitByte(0);
		SPI_masterTransmitByte(0);
	
		for (int i = 1; i <= x;i++)
		{
		SPI_masterTransmitByte(255);
		SPI_masterTransmitByte(0);
		SPI_masterTransmitByte(0);
		SPI_masterTransmitByte(0);
		}
		if (x < ledcount)
		{	
			x++;
			counter = x;
		}
		SPI_masterTransmitByte(255);
		SPI_masterTransmitByte(255);
		SPI_masterTransmitByte(0);
		SPI_masterTransmitByte(0);
		
		for (counter; (counter <= ledcount);counter++)
		{
		SPI_masterTransmitByte(255);
		SPI_masterTransmitByte(0);
		SPI_masterTransmitByte(0);
		SPI_masterTransmitByte(0);
		}
		
		SPI_masterTransmitByte(255);
		SPI_masterTransmitByte(255);
		SPI_masterTransmitByte(255);
		SPI_masterTransmitByte(255); 
		
		_delay_ms(10);
	}
	
}

