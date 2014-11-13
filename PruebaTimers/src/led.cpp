#include "led.hpp"
/*
  Para ver la distribución de los pines vea el datasheet del atmega328p, página 2.
*/
Led::Led() {
    // bit 5 el puerto B en modo salida
    DDRB |= _BV(DDB5);
}

void Led::on() {
    // bit 5 del puerto B a 1
    PORTB |= _BV(PORTB5);
}

void Led::off() {
    // bit 5 del puerto B a 0
    PORTB &= ~_BV(PORTB5);
}

void Led::toggle(){
  if( PORTB & _BV(PORTB5) ) {
    off();
  }
  else{
    on();
  }
}