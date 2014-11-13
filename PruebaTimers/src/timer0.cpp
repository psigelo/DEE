#include "timer0.hpp"



/*

Del datasheet: 
-------------

Para escojer el modo de operacion se debe tener en cuenta la siguiente tabla: 
	(leer datasheet para detalles de los modos).


Mode WGM02 WGM01 WGM00 Description	TOP		Update of OCRx		TOV Flag set
----------------------------------------------------------------------------
0 		0 	0 		0  Normal 		0xFF 	Immediate 			MAX

1 		0 	0 		1  (PWM, Phase  0xFF 	TOP 				BOTTOM
						Correct) 

2 		0 	1 		0  CTC 			OCRA 	Immediate 			MAX

3 		0 	1 		1  (Fast PWM) 	0xFF 	BOTTOM 				MAX

4 		1 	0 		0  Reserved 	– 		– 					–

5 		1 	0 		1  (PWM,Phase 	OCRA 	TOP 				BOTTOM 
						Correct) 

6 		1 	1 		0  Reserved 	– 		– 					–

7 		1 	1 		1  (Fast PWM) 	OCRA 	BOTTOM 				TOP
----------------------------------------------------------------------------
TOV: timer over flow


IMPORTANTE:
Se mostrarán los registros:


registro TCCR0A:
	bit 0: WGM00
	bit 1: WGM01 
	bit 2: -
	bit 3: -
	bit 4: COM0B0
	bit 5: COM0B1
	bit 6: COM0A0
	bit 7: COM0A1



registro TCCR0B:
	bit 0: CS00
	bit 1: CS01
	bit 2: CS02
	bit 3: WGM02
	bit 4: -
	bit 5: -
	bit 6: FOC0A
	bit 7: FOC0B


Respecto al preescaler:

CS02 		CS01 		CS00  		Description
-------------------------------------------------------------------------------------------
0 			0 			0 			No clock source (Timer/Counter stopped)
0 			0 			1 			clk I/O /(No prescaling)
0 			1 			0 			clk I/O /8 (From prescaler)
0 			1 			1 			clk I/O /64 (From prescaler)
1 			0 			0 			clk I/O /256 (From prescaler)
1 			0 			1 			clk I/O /1024 (From prescaler)
1 			1 			0 			External clock source on T0 pin. Clock on falling edge.
1 			1 			1 			External clock source on T0 pin. Clock on rising edge.
-------------------------------------------------------------------------------------------

Respecto a las interrupciones:
-----------------------------

TIMSK0: Timer Interrupt Mask Register
	bit 0: TOIE0
	bit 1: OCIE0A
	bit 2: OCIE0B
	bit 3: -
	bit 4: -
	bit 5: -
	bit 6: -
	bit 7: -

		• Bit 2 – OCIE0B: 
			Timer/Counter Output Compare Match B Interrupt Enable
		When the OCIE0B bit is written to one, and the I-bit in the Status Register is set, the
		Timer/Counter Compare Match B interrupt is enabled. The corresponding interrupt is executed if
		a Compare Match in Timer/Counter occurs, i.e., when the OCF0B bit is set in the Timer/Counter
		Interrupt Flag Register – TIFR0.


		• Bit 1 – OCIE0A: 
			Timer/Counter0 Output Compare Match A Interrupt Enable
		When the OCIE0A bit is written to one, and the I-bit in the Status Register is set, the
		Timer/Counter0 Compare Match A interrupt is enabled. The corresponding interrupt is executed
		if a Compare Match in Timer/Counter0 occurs, i.e., when the OCF0A bit is set in the
		Timer/Counter 0 Interrupt Flag Register – TIFR0.


		• Bit 0 – TOIE0: 
			Timer/Counter0 Overflow Interrupt Enable
		When the TOIE0 bit is written to one, and the I-bit in the Status Register is set, the
		Timer/Counter0 Overflow interrupt is enabled. The corresponding interrupt is executed if an
		overflow in Timer/Counter0 occurs, i.e., when the TOV0 bit is set in the Timer/Counter 0 Inter-
		rupt Flag Register – TIFR0.




TIFR0: Timer Interrupt Flag Register
	bit 0: TOV0
	bit 1: OCF0A
	bit 2: OCF0B
	bit 3: -
	bit 4: -
	bit 5: -
	bit 6: -
	bit 7: -


*/





Timer0::Timer0(){
	
}


/*
	datasheet:
	---------

	In Clear Timer on Compare or CTC mode (WGM02:0 = 2), the OCR0A Register is used to
	manipulate the counter resolution. In CTC mode the counter is cleared to zero when the counter
	value (TCNT0) matches the OCR0A. The OCR0A defines the top value for the counter, hence
	also its resolution. This mode allows greater control of the compare match output frequency. It
	also simplifies the operation of counting external events.

	The timing diagram for the CTC mode is shown in Figure 14-5. The counter value (TCNT0)
	increases until a compare match occurs between TCNT0 and OCR0A, and then counter
	(TCNT0) is cleared.

*/
int Timer0::modeCTC(){
	cli();// Desabilita interrupciones
	TCCR0A |=  _BV(WGM01); // WGM01: modo 2 CTC 
	TCCR0A &=	~_BV(WGM00);
	TCCR0B &= 	~_BV(WGM02);
	PRR    =   	PRR & ~_BV(PRTIM0); // Asegurar que este en cero para que funcione el timer, o sino el timer queda dormido por minimizar el consumo energetico.
	sei(); // Habilita interrupciones 
	return 0;	
}

void Timer0::InterruptOnOCF0A(){
	cli();
	// OCF0A es un bit que se hace uno al alcanzar el TCNT0 al valor OCR0A
	TIMSK0 |=  _BV(OCIE0A);
	sei();
}


/*	

	Recordar:

	CS02 		CS01 		CS00  		Description
	-------------------------------------------------------------------------------------------
	0 			0 			0 			No clock source (Timer/Counter stopped)
	0 			0 			1 			clk I/O /(No prescaling)
	0 			1 			0 			clk I/O /8 (From prescaler)
	0 			1 			1 			clk I/O /64 (From prescaler)
	1 			0 			0 			clk I/O /256 (From prescaler)
	1 			0 			1 			clk I/O /1024 (From prescaler)
	1 			1 			0 			External clock source on T0 pin. Clock on falling edge.
	1 			1 			1 			External clock source on T0 pin. Clock on rising edge.
	-------------------------------------------------------------------------------------------

	registro TCCR0B:
		bit 0: CS00
		bit 1: CS01
		bit 2: CS02
		bit 3: WGM02
		bit 4: -
		bit 5: -
		bit 6: FOC0A
		bit 7: FOC0B


*/
int Timer0::setPreescaler(uint16_t preescaler){
	cli();
	if(  preescaler == 1 || preescaler == 8 || preescaler == 64 || preescaler == 256 || preescaler == 1024  ){
		switch(preescaler){
			case 1:
				TCCR0B |=  _BV(CS00); 
				TCCR0B &= ~(_BV(CS01) | _BV(CS02) );
				break;
			case 8:
				TCCR0B |=  _BV(CS01); 
				TCCR0B &= ~(_BV(CS00) | _BV(CS02) );
				break;

			case 64:
				TCCR0B |=  _BV(CS01) | _BV(CS00); 
				TCCR0B &= ~( _BV(CS02) );
				break;

			case 256:
				TCCR0B |=  _BV(CS02); 
				TCCR0B &= ~(_BV(CS01) | _BV(CS00) );
				break;

			case 1024:
				TCCR0B |= _BV(CS02) | _BV(CS00); 
				TCCR0B &= ~_BV(CS01);
				break;
			
			default:
				return -1;
		}
		sei();
		return 0;
	}
	sei();
	return -1;
	
}

void Timer0::setOCR0A(uint8_t oc0a){
	cli();
	OCR0A  =   oc0a;	// No debe superar los 255 porque es un contador de 8 bit 
	sei();
}



/*
Mode WGM02 WGM01 WGM00 Description	TOP		Update of OCRx		TOV Flag set
----------------------------------------------------------------------------
0 		0 	0 		0  Normal 		0xFF 	Immediate 			MAX

1 		0 	0 		1  (PWM, Phase  0xFF 	TOP 				BOTTOM
						Correct) 

2 		0 	1 		0  CTC 			OCRA 	Immediate 			MAX

3 		0 	1 		1  (Fast PWM) 	0xFF 	BOTTOM 				MAX

4 		1 	0 		0  Reserved 	– 		– 					–

5 		1 	0 		1  (PWM,Phase 	OCRA 	TOP 				BOTTOM 
						Correct) 

6 		1 	1 		0  Reserved 	– 		– 					–

7 		1 	1 		1  (Fast PWM) 	OCRA 	BOTTOM 				TOP
----------------------------------------------------------------------------
TOV: timer over flow


IMPORTANTE:
Se mostrarán los registros:


registro TCCR0A:
	bit 0: WGM00
	bit 1: WGM01 
	bit 2: -
	bit 3: -
	bit 4: COM0B0
	bit 5: COM0B1
	bit 6: COM0A0
	bit 7: COM0A1



registro TCCR0B:
	bit 0: CS00
	bit 1: CS01
	bit 2: CS02
	bit 3: WGM02
	bit 4: -
	bit 5: -
	bit 6: FOC0A
	bit 7: FOC0B

*/
void Timer0::setFastPWMModeNonInverted(){
	TCCR0A |=  _BV(WGM01) | _BV(WGM00) | _BV(COM0A1); 
	TCCR0B !=  /* _BV(WGM02) */;
	DDRD |= _BV(DDD6);
	//PORTD |= _BV(PORTD6);

}