#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>

#define T_DIV 0x05

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
	- сделать если посылается очередь команд
	- 
*/


volatile uint8_t 	recieve_count;
volatile uint8_t 	recieve_ir;
volatile uint8_t	t = 0;

uint8_t	temp = 0x00;
uint8_t	temp1 = 0x00;
uint8_t	temp2 = 0x00;

ISR(PCINT0_vect) { 
	//if (PINB & 0x01) {
	//	temp = PINB;
	//	temp = PORTB;
	//}
	//else {
		temp = PINB;
		temp = PORTB;
		t = 1;
		GIMSK &= ~(1 << 5); // Отключение прерывания
	//}
}

ISR(TIM0_COMPB_vect) {
	if(--recieve_count == 0) {
		TIMSK0 &= ~(1 << OCIE0B);
		TIFR0 |= (1 << OCF0B);
		recieve_ir = recieve_ir >> 1;
	}
	else {
		temp = PINB & 0x01; //| PORTB & 0x01;
		recieve_ir |= temp; // не получается передать информацию
		recieve_ir = recieve_ir << 1;
	}
}

uint8_t recieve(uint8_t* b) {
	while(recieve_count) {
	}
	*b = recieve_ir;
	if (*b & 0x01) {
		return 1;
	} 
	else {
		return -1;
	}

}

void init() {
	recieve_count = 0x03;
	recieve_ir = 0x00;

	DDRB = 0x1E; // 0001 111E
	PORTB = 0x0A; 

	MCUCR = 0x30; 
	GIMSK = 0x20;
	PCMSK = 0x01;
	TCCR0B |= T_DIV;
	sei();
}

int main(void) {
 
init();
 
while (1) {
	sleep_mode();
	if (t == 1) {
		while ((PINB & 0x01) == 0) {
			recieve_ir = 0x00;
			recieve_count = 0x03;
			//if (TCNT0 < (T_DIV / 2)) {
			//	OCR0B = TCNT0 + (T_DIV / 2);
			//}
			//else {
			//	OCR0B = TCNT0 - (T_DIV / 2);
			//}
			
			TIMSK0 |= (1 << OCIE0B);
			TIFR0 |= (1 << OCF0B);
			uint8_t b;
			uint8_t	temp1 = PORTB;
			if (recieve(&b) == 1) {
				temp = b;
				uint8_t	temp1 = PORTB;
				if ((b & 0b11) == 0b11) { // check left or right
					switch(PORTB & 0x1E) { 	// left
				    	case 0x0A: 	
							PORTB =0x12;	
							break;  // 0x12
				    	 case 0x0C:  	
							PORTB =0x0A;	
							break; // 0x14
				    	 case 0x14: 	
							PORTB =0x0C;  
							break; // 0x12
				    	 case 0x12: 	
							PORTB =0x14;	
							break; // 0x0A
				 	}
    			} 
				else {
					switch(PORTB & 0x1E) { 	// right
						case 0x0A: 	
							PORTB =0x0C;	
							break;  // 0x0C
				    	case 0x0C:  	
							PORTB =0x14;	
							break; // 0x14
				    	case 0x14: 	
							PORTB =0x12;  
							break; // 0x12
				    	case 0x12: 	
							PORTB =0x0A;	
							break; // 0x0A
				 	}
				}
		}
			recieve_ir = 0x00;
			temp2 = PORTB;
		}
		
		t = 0;
		GIMSK |= (1 << 5);

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