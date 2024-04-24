/*
DESCRIPTION
This sketch allows to acquire data from a peak detector.  It takes an interrupt from a comparator, at which point it samples the peak
detector output with its ADC, and then afterwards it sends a digital high to a MOSFET which resets the holding capacitor.
inputs:  
 - TTL interrupt from the comparator as a trigger / interrupt
 - analog signal from the PD to its ADC
outputs:
 - reset signal to an external MOSFET
 - serial out to the computer with the value of ADC

 DATE created: 04.2024
 Author:  Areg Danagoulian, aregjan@mit.edu
*/

#include <SoftwareSerial.h>
//#include <AltSoftSerial.h>

//#include <Tiny4kOLED_bitbang.h>

#define rx 0//3
#define tx 1//4
#define reset_pin 4 //reset of mosfet
#define INTERRUPT_PIN 2
String s = "a";
volatile byte i=0;

SoftwareSerial serial(rx, tx); //this is most likely backwards
//AltSoftSerial serial;
void setup() {
  cli();
//  MCUCR |= (1 << ISC00);  //CHANGE mode
//  GIMSK = 0b00100000;  //enable all pins
//  PCMSK = 0b00000010;  //enable external interrupt to pb1
  GIMSK |= (1 << INT0); // enable external interrupt...doesn't this have to be Interrupt_pin?
  MCUCR |= (1 << ISC00); // CHANGE mode
  pinMode(INTERRUPT_PIN, INPUT);
  sei();                 // enables interrupts
  pinMode(A3,INPUT); //analog in
  pinMode(reset_pin,OUTPUT);
  PORTB ^= 1<<reset_pin; //keep mosfet off


  serial.begin(115200); //9600
  serial.println("Initializing...");
}

void loop() {
  if(i){
//    uint16_t time_before=micros();
    delayMicroseconds(2); //wait for the peak detector to settle
    uint16_t adc=analogRead(A3); //2^10 ADC
//    uint16_t time_after=micros();
//    delayMicroseconds(250);
    reset();
    uint16_t adc_baseline=analogRead(A3); //2^10 ADC
    serial.println(String(adc)+"\t"+String(adc_baseline));
//    serial.println(String(adc)+"\t"+String(time_after-time_before));
    i=0; //reset i
  }
}

ISR(INT0_vect){
  i = digitalRead(INTERRUPT_PIN);
}
void reset(){ //send reset to mosfet

    PORTB |= 1<<reset_pin; //turn on PB1
    delayMicroseconds(20); //this is blocking, however this is very precise
    PORTB ^= 1<<reset_pin; //turn off PB1

}