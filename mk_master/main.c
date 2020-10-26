#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define BT1 1
#define BT2 2


/*
 ____________________
| attiny13 			|
|			PORTB	|
|				0	|----->  IR
|				1	|<-----  left button
|				2	|<-----	 right button
|				3	|-
|				4	|-
|				5	|<-----	reset
|___________________|

PORTB = --00 0000

*/


/*
TODO list 
	- понять можно ли обслуживать цикл в прерывании
	- понять какие данные и как отправлять 
	- избавиться от дребезга кнопок возмодно и не надо
*/

//#define F_CPU 9600000UL

volatile uint8_t  send_ir;
volatile uint8_t  send_count;

ISR(PCINT0_vect) { 
	if ((PORTB & (1 << BT1)) & (PORTB & (1 << BT2))) send_ir = 1;
	if (PINB & (1 << BT1)) {//PORTB &= ~(1 << 3);
		PORTB &= ~(1 << 0); 
		//PORTB |= (1 << 0);
	
	}

	if (PINB & (1 << BT2)) PORTB &= ~(1 << 4);
}


void init() {
	send_ir = 0xFF;
	send_count = 0x00;

	DDRB = 0x01; // DDRB = --00 0001
	PORTB = 0x01;
	PINB = 0x06;

	MCUCR = 0x30;
	GIMSK = 0x20;
	PCMSK = 0x06;
	asm("sei");
}

int main() {

	init();

	while (1) {
		asm("sleep");
		//_delay_ms(1000); 
	}

}