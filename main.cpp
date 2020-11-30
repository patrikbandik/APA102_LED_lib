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



void buildLED(int bright, int red, int green, int blue)
{
	SPI_masterTransmitByte(224+bright);
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


void movingled(int bright, int ledcount, int red, int green, int blue)
{
	volatile int counter;
	int x = 0;
	
	while (1)
	{
		if (x == ledcount)
		{
			x = 0;
		}
		emptyFrame();
		
		for (int i = 1; i <= x;i++)
		{
			OFFLED
		}
		if (x < ledcount)
		{
			x++;
			counter = x;
		}
		buildLED(bright,red,green,blue);
		
		for (counter; (counter <= ledcount);counter++)
		{
			OFFLED
		}
		
		_delay_ms(45);
	}
}




void matrixeffect(int bright, int ledcount)  
{
	//LED MODULE SHOULD BE SQUARE IN ORDER TO WORK NICELY
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

	
void staticlights(int bright, int red, int green, int blue, int ledcount)
{
	emptyFrame();
	for (int i = 1; i <= ledcount;i++)
	{	
		buildLED(bright,red, green, blue);	
	}
}


void nyancat(int bright, int ledcount)
{
	volatile int counter;
	int x = 0;

	while (1)
	{
		if (x == ledcount)
		{
			x = 0;
		}
		emptyFrame();
		
		for (int i = 1; i <= x;i++)
		{
			OFFLED
		}
		if (x < (ledcount))
		{
			x++;
			counter = x;
		}
		
		REDLED
		buildLED(bright, 253,153,0);
		buildLED(bright, 253,253,0);
		buildLED(bright, 51,253,0);
		buildLED(bright, 0,152,254);
		buildLED(bright, 102,51, 253);

		
		for (counter; ((counter) <= ledcount);counter++)
		{
			OFFLED
		}
		
		_delay_ms(60);
	}
}
	
void apaControl(int bright, int ledcount, int effectname, int red = 102, int green = 51, int blue =253)
{
	switch(effectname){
		case 11: // 11 = movingled EFFECT
				movingled(bright, ledcount, red, green, blue);
		case 22: // 22= matrixeffect is only GREEN
				matrixeffect(bright, ledcount);
		case 33:  // 44 = nyancat EFFECT
				nyancat(bright,ledcount);
		case 99:  // 99 = staticlights EFFECT
				staticlights(bright, red, green, blue, ledcount);
	}
}



int main(void)
{	
	initADC();
	SPI_init();
	resetBoard(64);
	
	while(1)
	{
		//movingled(2,64);
		//matrixeffect(1,64);
		//staticlights(3, 255,0,0,63);
		//nyancat(1, 64);
		apaControl(1,64, 11, 100, 50,150);
	}
}

