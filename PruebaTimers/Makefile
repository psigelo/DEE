VPATH= ./include ./src

all: led.o timer0.o pruebaTimer.o enlazar crearHexxfile

timer0.o: timer0.cpp
	mkdir -p objects
	avr-g++ -Os -DF_CPU=16000000UL -mmcu=atmega328p -I./include/ -c  ./src/timer0.cpp  -o ./objects/timer0.o 

led.o: led.cpp
	mkdir -p objects
	avr-g++ -Os -DF_CPU=16000000UL -mmcu=atmega328p -I./include/ -c  ./src/led.cpp  -o ./objects/led.o 

pruebaTimer.o:
	mkdir -p objects
	avr-g++ -Os -DF_CPU=16000000UL -mmcu=atmega328p -I./include/ -c  ./src/pruebaTimer.cpp  -o ./objects/pruebaTimer.o

enlazar:
	mkdir -p bin
	avr-g++ -mmcu=atmega328p -I./include/ ./objects/pruebaTimer.o ./objects/led.o ./objects/timer0.o -o ./bin/pruebaTimer

crearHexxfile:
	mkdir -p hex
	avr-objcopy -O ihex -R .eeprom ./bin/pruebaTimer ./hex/pruebaTimer.hex

upload:
	avrdude -V -c arduino -p ATMEGA328P -P /dev/ttyACM0 -b 115200 -U flash:w:./hex/pruebaTimer.hex

clean:
	rm -Rf objects bin hex