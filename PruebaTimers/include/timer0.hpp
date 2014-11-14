#ifndef TIMER0_HPP
#define TIMER0_HPP

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

class Timer0{
	public:
		Timer0();
		int modeCTC(); // return 0 on success and -1 on error
		void setOCR0A(uint8_t oc0a);
		void InterruptOnOCF0A();
		void InterruptOnOCF0B();
		int setPreescaler(uint16_t preescaler);
		void setFastPWMModeNonInverted();
};


#endif