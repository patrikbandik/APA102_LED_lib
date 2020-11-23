#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#define SPI_DDR DDRB
#define CS      PINB2
#define MOSI    PINB3
#define MISO    PINB4
#define SCK     PINB5

#include <string.h>
#include <math.h>
#include <stdio.h>
//#include <stdlib.h>
#include <time.h>
#define REDLED buildLED(1,255,0,0);
#define GREENLED buildLED(1,0,255,0);
#define BLUELED buildLED(1,0,0,255);
#define OFFLED buildLED(1,0,0,0);


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


void initADC() {
	// AREF = AVcc
	ADMUX = (1<<REFS0);
	// ADC Enable and prescaler of 128
	// 16000000/128 = 125000
	ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
}

uint16_t ReadADC(uint8_t mux) {
	mux &= 0b00000111;							// AND operation with 7 to always keep the value between 0 and 7
	// clears the last 3 bits before OR-ing, F8 = 1111 1000
	ADMUX = (ADMUX & 0xF8) | mux;

	// make ADSC '1' to start conversion
	ADCSRA |= (1<<ADSC);

	// wait until ADSC becomes '0', which means the conversion is done
	while(ADCSRA & (1<<ADSC));

	return (ADC);								// return the value
}

void seedRandom(void)
{
	uint16_t u_seed_rand_val = 0;
	
	for(uint8_t i=0; i<16; i++){
		u_seed_rand_val = u_seed_rand_val<<1 | (ReadADC(0)&0b1);
	}
	srand (u_seed_rand_val);
}



void movingled(int bright, int ledcount)
{
	ledcount = 64;
	volatile int counter;
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
		SPI_masterTransmitByte(225);
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
		
		/*SPI_masterTransmitByte(255);
		SPI_masterTransmitByte(255);
		SPI_masterTransmitByte(255);
		SPI_masterTransmitByte(255);*/
		
		SPI_masterTransmitByte(0);
		SPI_masterTransmitByte(0);
		SPI_masterTransmitByte(0);
		SPI_masterTransmitByte(0);
		
		_delay_ms(50);
	}
}




void buildLED(int brightness, int red, int green, int blue)
{
	SPI_masterTransmitByte(224+brightness);
	SPI_masterTransmitByte(blue);
	SPI_masterTransmitByte(green);
	SPI_masterTransmitByte(red);
}

void emptyFrame(void)
{
	SPI_masterTransmitByte(0);
	SPI_masterTransmitByte(0);
	SPI_masterTransmitByte(0);
	SPI_masterTransmitByte(0);
}

void resetBoard(int ledcount)
{
	emptyFrame();
	for (int i = 1; i<= ledcount; i++)
	{
		OFFLED
	}
	emptyFrame();
}

void matrixeffect(int bright, int ledcount)
{
	char ledarray[ledcount];
	int rowlenght = sqrt(ledcount);
	char ledrow[rowlenght+1];
	char x[2];
	
	while (1)
	{
	
	
	//create row with random 0/1
	for (int i=0; i < rowlenght; i++)
	{
		int foo = rand() > RAND_MAX/2;
		sprintf(x,"%d", foo);
		ledrow[i] = *x;
	}
	
	
	strcat(ledarray, ledrow); //concatenate the char array
	
	char kokos[] = "111111111111111";
	emptyFrame();
	for (int i = strlen(kokos); i >= 1; i--)
	{
		if (kokos[i] == '1')
		{
			buildLED(1,0,255,0);
			buildLED(1,0,255,0);
			buildLED(1,0,255,0);
			buildLED(1,0,255,0);
			buildLED(1,0,255,0);
			buildLED(1,0,255,0);
		}
		else
		{
			buildLED(1,0,255,0);
			buildLED(1,0,255,0);
			buildLED(1,0,255,0);
			buildLED(1,0,255,0);
			buildLED(1,0,255,0);
			buildLED(1,0,255,0);
		}
	}
	emptyFrame();
	}
}

void matrixeffect1(int bright, int ledcount, int speed)
{
	char ledarray[ledcount+1];
	int rowlenght = sqrt(ledcount); //lenght of a row
	char ledrow[rowlenght];
	char x[]= "";

	//create ledrow with random 0/1
	for (int i=0; i < rowlenght-1; i++)
	{
		seedRandom();
		int foo = rand()%2;
		sprintf(x,"%d", foo);
		ledrow[i] = *x;
	}

	rowlenght = rowlenght + 1;
	sprintf(ledrow, "%s%c", ledrow, *x);
	int foo;
	while(1)
	{
		for (int i=0; i < rowlenght-1; i++)
		{
			seedRandom();
			foo = rand()%2;
			sprintf(x,"%d", foo);
			ledrow[i] = *x;
		}

		snprintf(ledarray,sizeof(ledarray), "%s%s",ledrow, ledarray);

		_delay_ms(2);

		emptyFrame();
		for (int i = 0; i < strlen(ledarray); i++)
		{
			if (ledarray[i] == '1')
			{
				buildLED(bright,0,255,0);
				
			}
			else
			{
				buildLED(1,0,0,0);
			}
			
			_delay_ms(5);
		}
		
	}
}

void skuska(void)
{
	static bool initialized;
	if (!initialized) {
		initialized = true;
		// do the initialization part
		resetBoard(64);
	}
	char kokos[] = "101011010011011";
	emptyFrame();
	for (int i = 20; i >= 1; i--)
	{
		if (kokos[i] == '1')
		{
			GREENLED;
		}
		else
		{
			OFFLED;
		}
	}
	emptyFrame();
}
	
	
	



int main(void)
{	
	initADC();
	SPI_init();
	resetBoard(64);
	
	while(1)
	{
		
		//skuska();
		//movingled(30,30);
		matrixeffect1(1,64,5);
	}
}

