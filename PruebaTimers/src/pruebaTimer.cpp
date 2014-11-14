#include <avr/interrupt.h>
#include <avr/sleep.h>
#include "led.hpp"
#include "timer0.hpp"
/* 
    Inicializare el timer0 como interrupcion CTC.
    Con prescaler 1024.
*/
Led led;
int main(){
  
    
    /*
    Timer0 timer0;
    timer0.modeCTC();
    timer0.setOCR0A((uint8_t)254); 
    timer0.setPreescaler((uint16_t)1024);
    timer0.InterruptOnOCF0A();
    */

    Timer0 timer0;
    timer0.setOCR0A((uint8_t)200);
    timer0.setPreescaler((uint16_t)8);
    timer0.setFastPWMModeNonInverted();
    timer0.InterruptOnOCF0B();


    while(1){
        // Para el modo power-save
        SMCR = _BV(SM1) | _BV(SM0) | _BV(SE);
        // Luego de la interrupcion se vuelve a la rutina normal, se debe volver a hacer el loop
    }
    

    cli();
    
}

/* 

// Interrup Service Rutine:    
ISR(TIMER0_COMPA_vect)
{
    //Para evitar que otra interrupcion sea ejecutada entre medio.
    cli();
        led.toggle();
    sei();
}
*/