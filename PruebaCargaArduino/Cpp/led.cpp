#include <avr/io.h>
#include <util/delay.h>

using namespace std;

class Led {
    public:
        Led();
        void on();
        void off();
};

Led::Led() {
    // bit 7 el puerto C en modo salida
    //DDRC |= 0x80;
    DDRB |= _BV(DDB5);
}

void Led::on() {
    // bit 7 del puerto C a 1
    //PORTC |= 0x80;
    PORTB |= _BV(PORTB5);
}

void Led::off() {
    // bit 7 del puerto C a 0
    //PORTC &= 0x7F;
    PORTB &= ~_BV(PORTB5);
}

int main() {
    Led led;
    while (1) {
        led.on();
        _delay_ms(2000);
        led.off();
        _delay_ms(2000);
    }
}