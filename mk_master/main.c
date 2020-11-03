#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/sleep.h>

#define BT1 1
#define BT2 2
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
			________			   ______.______	1
LEFT				|______.___0__|					0

			________	   .___1__.______.______	1
Right				|______|	  					0

*/


/*
TODO list 
	 
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
	t = 0x01;
	GIMSK &= ~(1 << 5); // Отключение прерывания
}

ISR(TIM0_COMPB_vect) {
	if(--send_count == 0) {
		TIMSK0 &= ~(1 << OCIE0B);
		TIFR0 |= (1 << OCF0B);
	}
	else {
		PORTB = (PORTB & ~(1 << 0)) | (send_ir & 0x01);
		temp = PORTB;
		send_ir = send_ir >> 1; 
	}
}


void send_command() {
	temp = PORTB;
	send_ir = command;
	while(send_count);
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
			while (((PINB & (1<<BT1)) == 0) || ((PINB & (1<<BT2)) == 0)) {
				if ((PINB & (1<<BT1)) == 0 && (PINB & (1<<BT2)) == 0) {
				}
				else { 
					send_count = 3;
					TIMSK0 |= (1 << OCIE0B);
					TIFR0 |= (1 << OCF0B);
					PORTB &= ~(1 << 0); // set start bit | назначение стартового бита
					temp = PINB & (1 << BT1);
					temp = PINB & (1 << BT2);
					if ((PINB & (1 << BT1)) == 0) {//PORTB &= ~(1 << 3);
						command = 0b10; // отправка сигнала налево | send left signal
					}
					if ((PINB & (1 << BT2)) == 0) {
						command = 0b11; // отправка сигнала направо | send right signal
					}
					send_command(); 
				}
			}
		GIMSK |= (1 << 5);
		t = 0; // end set to '0' t
		}
	}

}