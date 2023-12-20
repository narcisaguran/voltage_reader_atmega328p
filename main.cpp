#include <avr/io.h>
#include <util/delay.h>
#include "config.h"

#define ADMUX_DEFAULT 0b01100000

#define SPI_DDR DDRB
#define CS      PINB2
#define MOSI    PINB3
#define MISO    PINB4
#define SCK     PINB5

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
	SPDR = data;

	// Wait for transmission complete
	while(!(SPSR & (1 << SPIF)));
}

int fvoltage3(void){//ADC3
	ADMUX = ADMUX_DEFAULT | 3;
	ADCSRA |= (1<<ADSC);
	while((ADCSRA& (1<<ADIF)) ==0); //wait until conversion is done
	ADCSRA |= (1<<ADIF);
	
	uint16_t voltage3 = ((ADCL << 8) | ADCH);
	
	return voltage3;
}

int fvoltage2(void){//ADC2
	ADMUX = ADMUX_DEFAULT | 2;
	ADCSRA |= (1<<ADSC);
	while((ADCSRA& (1<<ADIF)) ==0); //wait until conversion is done
	ADCSRA |= (1<<ADIF);
	
	uint16_t voltage2 = ((ADCL << 8) | ADCH);
	
	return voltage2;
}

int fvoltage1(void){//ADC1
	ADMUX = ADMUX_DEFAULT | 1;
	ADCSRA |= (1<<ADSC);
	while((ADCSRA& (1<<ADIF)) ==0); //wait until conversion is done
	ADCSRA |= (1<<ADIF);
	
	uint16_t voltage1 = ((ADCL << 8) | ADCH);
	
	return voltage1;
}

int fvoltage0(void){//ADC0
	ADMUX = ADMUX_DEFAULT;
	ADCSRA |= (1<<ADSC);
	while((ADCSRA& (1<<ADIF)) ==0); //wait until conversion is done
	ADCSRA |= (1<<ADIF);
	
	uint16_t voltage0 = ((ADCL << 8) | ADCH);
	
	return voltage0;
}


int main(void){
	DDRD = 0xFF;
	ADCSRA = 0b10100011;
	ADCSRB = 0b00000000;
	PORTD = 0x00;
	PORTB = 0x00;
	uint8_t thVal = 5;
	
	while(1){
		uint16_t voltage[4];
		uint16_t minVal;
		voltage[0] = fvoltage0();
		voltage[1] = fvoltage1();
		voltage[2] = fvoltage2();
		voltage[3] = fvoltage3();
		
		minVal = voltage[0];
		for(int i = 1; i < 4; i++){
			if(voltage[i] < minVal) minVal = voltage[i];
		}
		
		if(voltage[0] >  (minVal + thVal)) PORTD = 0x08; //PD3 HIGH
		if(voltage[1] >  (minVal + thVal)) PORTD |= 0x20; //PD5 HIGH
		if(voltage[2] >  (minVal + thVal)) PORTD |= 0x40; //PD6 HIGH
		if(voltage[3] >  (minVal + thVal)) PORTB = 0x02; //PB1 HIGH
				
		_delay_ms(10000);
		PORTB = 0x00;
		PORTD = 0x00;
		_delay_ms(10000);
		
	}
	return 0;
}

