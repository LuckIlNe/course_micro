
#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>

volatile uint8_t recieve_count;
volatile uint8_t recieve_ir;

/*
stepper motor - st
 ____________________
| attiny13 			|
|			PORTB	|
|				0	|<----- IR
|				1	|-----> st coil 2
|				2	|-----> st coil 1
|				3	|-----> st coil 3
|				4	|-----> st coil 4	 
|				5	|----->	reset
|___________________|

*/

/*
TODO list 
	- прерывание для приема данных с порта 0 возможно можно сделать прерывание которое будет запускать прерывания таймера, 
в вечном цикле в основной подпрограмме проверять есть ли данные (сомнительный вариант)
*/


ISR(TIM0_COMPB_vect)
{
	if(RXPORT & (1 << RXD))			// Проверяем в каком состоянии вход RXD
		rxbyte |= 0x80;			// Если в 1, то пишем 1 в старший разряд rxbyte
	
	if(--rxbitcount == 0)			// Уменьшаем на 1 счетчик бит и проверяем не стал ли он нулем
	{
		TIMSK0 &= ~(1 << OCIE0B);	// Если да, запрещаем прерывание TIM0_COMPB
		TIFR0 |= (1 << OCF0B);		// Очищаем флаг прерывания TIM0_COMPB
		GIFR |= (1 << INTF0);		// Очищаем флаг прерывания по INT0
		GIMSK |= (1 << INT0);		// Разрешаем прерывание INT0
	}
	else
	{
		rxbyte >>= 0x01;		// Иначе сдвигаем rxbyte вправо на 1
	}
}


ISR(INT0_vect)
{
	rxbitcount = 0x09;			// 8 бит данных и 1 стартовый бит
	rxbyte = 0x00;				// Обнуляем содержимое rxbyte
	if(TCNT0 < (BAUD_DIV / 2))		// Если таймер не досчитал до середины текущего периода
	{
		OCR0B = TCNT0 + (BAUD_DIV / 2);	// То прерывание произойдет в текущем периоде спустя пол периода
	}
	else
	{
		OCR0B = TCNT0 - (BAUD_DIV / 2);	// Иначе прерывание произойдет уже в следующем периоде таймера
	}
	GIMSK &= ~(1 << INT0);			// Запрещаем прерывание по INT0
	TIFR0 |= (1 << OCF0A) | (1 << OCF0B);	// Очищаем флаги прерываний TIM0_COMPA (B)
	TIMSK0 |= (1 << OCIE0B);		// Разрешаем прерывание по OCR0B
}


int16_t recieve(uint8_t* b) {
	if (1) {
		//recieve_count < 0x02) { //check recieve information
		while(recieve_count);
		recieve_count = 0x02;
		*b = recieve_ir;
		return 1; 
	}
	else {
		return -1;
	}
}

void init() {
	recieve_count = 0x02;
	recieve_ir = 0x00;

	DDRB = 0x1E; // 0001 111E
	PORTB = 0x0A; 
}

void main(void) {
 
init();
 
while (1) {
	uint8_t b;
	if (recieve(&b) == 1) {
		if (1) { // check left or right
			switch(PORTB) { 	// left
		    	case 0x0A: 	
				PORTB=0x0C;	
				break;  // 0x0C
		    	 case 0x0C:  	
				PORTB=0x14;	
				break; // 0x14
		    	 case 0x14: 	
				PORTB=0x12;  
				break; // 0x12
		    	 case 0x12: 	
				PORTB=0x0A;	
				break; // 0x0A
		 	}
		 	_delay_ms(10); 
    	} 
		else {
			switch(PORTB) { 	// right
		    	case 0x0A: 	
				PORTB=0x12;	
				break;  // 0x12
		    	 case 0x0C:  	
				PORTB=0x0A;	
				break; // 0x14
		    	 case 0x14: 	
				PORTB=0x0C;  
				break; // 0x12
		    	 case 0x12: 	
				PORTB=0x14;	
				break; // 0x0A
		 	}
		 	_delay_ms(10); 
		}
	}
}


}


/*
      PORTB=0x0A; //+a +b 
	 _delay_ms(1000); 
	 PORTB=0x0C; //+a +b 
	 _delay_ms(1000); 
	 PORTB=0x14; //+a +b 
	 _delay_ms(1000); 
	 PORTB=0x12; //+a +b 
	 _delay_ms(1000); 
	 PORTB=0x0A; //+a +b 
	 _delay_ms(1000); 


*/