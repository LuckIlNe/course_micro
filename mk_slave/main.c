
#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>


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


SEND DATA
			  wait  |first |data  |last  | wait
			________			   ______.______ 	   	1
LEFT				|______.___0__|						0

			________	   .___1__.______.______		1
Right				|______|	  						0


*/

/*
TODO list 
	- прерывание для приема данных с порта 0 возможно можно сделать прерывание которое будет запускать прерывания таймера, 
в вечном цикле в основной подпрограмме проверять есть ли данные (сомнительный вариант)
*/


volatile uint8_t recieve_count;
volatile uint8_t recieve_ir;
volatile uint8_t	t = 0;

ISR(INT0_vect) { 
	t = 1;
	recieve_count = 0x02;
	GIMSK &= ~(1 << 5); // Отключение прерывания
	TIMSK0 |= (1 << OCIE0B);
}

ISR(TIM0_COMPB_vect) {
	if(--recieve_count == 0) {
		TIMSK0 &= ~(1 << OCIE0B);
		TIFR0 |= (1 << OCF0B);
		GIMSK &= ~(1 << 5);
	}
	else {
		recieve_ir = PORTB & 0x01;
	}
}

int16_t recieve(uint8_t* b) {
	while(recieve_count) {
		*b = recieve_ir;
	}
	return 1;
}

void init() {
	recieve_count = 0x02;
	recieve_ir = 0x00;

	DDRB = 0x1E; // 0001 111E
	PORTB = 0x0A; 

	MCUCR = 0x30; 
	MCUCR |= 0x03;//(1 << ISC01); // настрока преывния INT0
	GIMSK |= 0x40; //(1 << INT0);//0x40;
	//PCMSK = 0x01;
	sei();
}

int main(void) {
 
init();
 
while (1) {
	//sleep_mode();
	if (t == 1) {
		uint8_t b;
		if (recieve(&b) == 1) {
			if (b) { // check left or right
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
			 	//_delay_ms(10); 
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
			 	//_delay_ms(10); 
			}
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