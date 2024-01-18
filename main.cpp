#include <avr/io.h>
#include <util/delay.h>
#include "config.h"

#define ADMUX_DEFAULT 0b01000000

int fvoltage3(void){//ADC3, C4
	ADMUX = ADMUX_DEFAULT | 3;
	ADCSRA |= (1<<ADSC);
	while((ADCSRA& (1<<ADIF)) ==0); //wait until conversion is done
	ADCSRA |= (1<<ADIF);
	
	uint16_t voltage3 = ADC;
	
	return voltage3;
}

int fvoltage2(void){//ADC2, C3
	ADMUX = ADMUX_DEFAULT | 2;
	ADCSRA |= (1<<ADSC);
	while((ADCSRA& (1<<ADIF)) ==0); //wait until conversion is done
	ADCSRA |= (1<<ADIF);
	
	uint16_t voltage2 = ADC ;

	return voltage2;
}

int fvoltage1(void){//ADC1, C2
	ADMUX = ADMUX_DEFAULT | 1;
	ADCSRA |= (1<<ADSC);
	while((ADCSRA& (1<<ADIF)) ==0); //wait until conversion is done
	ADCSRA |= (1<<ADIF);
	
	uint16_t voltage1 = ADC;
	
	return voltage1;
}


int fvoltage0(void){//ADC0, C1
	ADMUX = ADMUX_DEFAULT;
	ADCSRA |= (1<<ADSC);
	
	while((ADCSRA& (1<<ADIF)) ==0); //wait until conversion is done
	ADCSRA |= (1<<ADIF);
	
	uint16_t voltage0 = ADC;
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
		for(int i = 0; i < 4; ++i){
			if(voltage[i] < minVal) minVal = voltage[i];
		}
		
		if(voltage[0] > (minVal + thVal)) PORTD = 0x08; //PD3 HIGH
		if(voltage[1] >  (minVal + thVal)) PORTD |= 0x20; //PD5 HIGH
		if(voltage[2] >  (minVal + thVal)) PORTD |= 0x40; //PD6 HIGH
		if(voltage[3] >  (minVal + thVal)) PORTB = 0x02; //PB1 HIGH
				
		_delay_ms(4000);
		PORTB = 0x00;
		PORTD = 0x00;
		_delay_ms(4000);
	}
	return 0;
}

