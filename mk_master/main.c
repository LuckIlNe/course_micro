#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/sleep.h>

#define BT1 1
#define BT2 2
#define BAUD_DIV 0x7D
#define T_DIV 0x05


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

SEND DATA
			  wait  |first |data  |last  | wait
			________			   ______.______ 	   	1
LEFT				|______.___0__|						0

			________	   .___1__.______.______		1
Right				|______|	  						0

*/


/*
TODO list 
	- избавиться от дребезга кнопок возмодно и не надо
	- создавать ли цикл или просто один раз проходить


For me 
	PORTB &= ~(1 << PBN); // PBN -> 0  
	PORTB |= (1 << PBN); // PBN -> 1
*/

volatile uint8_t  	send_ir;
volatile uint8_t  	send_count;
volatile uint8_t	t = 0x00;
volatile uint8_t	command = 0x00;
volatile uint8_t	temp = 0x00;

uint8_t temp1;
uint8_t temp2;

ISR(PCINT0_vect) { 
	//if ( ((PORTB & (1 << BT1)) == 0)  ((PORTB & (1 << BT2)) == 0) )
	t = 0x01;
}

ISR(TIM0_COMPB_vect) {
	if(--send_count == 0) {
		TIMSK0 &= ~(1 << OCIE0B);
		TIFR0 |= (1 << OCF0B);
		GIMSK |= (1 << 5);
	}
	else {
		PORTB = (PORTB & ~(1 << 0)) | (send_ir & 0x01);
		temp = PORTB;
	}
}


void send_command() {
	temp = PORTB;
	while(send_count > 0) {
		send_ir = command;
	}
	temp = PORTB;
	PORTB |= (1 << 0);
	temp = PORTB;
}

void init() {
	send_ir = 0x00;
	send_count = 0x00;

	DDRB = 0x07; // DDRB = --00 0001
	PORTB = 0x01;
	PINB = 0x06;

	MCUCR = 0x30; 
	GIMSK = 0x20;
	PCMSK = 0x06;

	TCCR0B |= T_DIV;
	asm("sei");
}

int main() {

	init();

	while (1) {

		sleep_mode(); // режим низкого потребления | power-down mode
		if (t == 1) {
			temp1 = PORTB & (1 << BT1);
			temp2 = PORTB & (1 << BT2); 
			while (((PINB & (1<<BT1)) == 0) || ((PINB & (1<<BT2)) == 0)) {
				send_count = 2;
				GIMSK &= ~(1 << 5); // Отключение прерывания
				TIMSK0 |= (1 << OCIE0B);
				temp = PORTB;
				PORTB &= ~(1 << 0);
				temp = PORTB;
				if ((PINB & (1<<BT1)) == 0 && (PINB & (1<<BT2)) == 0) {
					send_count = 0;
					TIMSK0 &= ~(1 << OCIE0B);
					TIFR0 |= (1 << OCF0B);
					GIMSK &= ~(1 << 5);
				}
				else {
					if (PINB & (1 << BT1)) {//PORTB &= ~(1 << 3);
						command = 0x00; // отправка сигнала налево | send left signal
					}
					if (PINB & (1 << BT2)) {
						command = 0x01; // отправка сигнала направо | send right signal
					}
					send_command(); 
				}
				temp1 = PORTB & (1 << BT1);
				temp2 = PORTB & (1 << BT2);
			}
		t = 0; // end set to '0' t
		}
	}

}