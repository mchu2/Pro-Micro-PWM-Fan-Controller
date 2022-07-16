#include <Arduino.h>

#define fan1 3
#define fan2 5
#define fan3 6
#define fan4 9
#define button 21

//button stuff
int buttonState;
int lastButtonState = LOW;
byte dutyCycle = 25;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

//timer stuff
const byte OC1A_PIN = 9;
const byte OC3A_PIN = 5;

const byte OC0B_PIN = 3;
const byte OC4D_PIN = 6;

const word PWM_FREQ_HZ = 25000; 

const word TCNT1_TOP = 16000000/(2*PWM_FREQ_HZ);
const word TCNT3_TOP = TCNT1_TOP;

void setPwmDuty(byte duty) {
  OCR1A = (word)(duty*TCNT1_TOP)/100;
  OCR3A = (word)(duty*TCNT3_TOP)/100;
  OCR0B = (word)(duty*40)/100;
  OCR4D = (word)(duty*40)/100;
}

void checkButtonPress(){
  int readButton = digitalRead(button);
  if(readButton != lastButtonState){
    lastDebounceTime = millis();
  }
  if ((millis()-lastDebounceTime)>debounceDelay){
    if(readButton != buttonState){
      buttonState = readButton;
      if(readButton == HIGH){
        switch(dutyCycle){
          case 0:
            dutyCycle=25;
            break;
          case 25:
            dutyCycle=50;
            break;
          case 50:
            dutyCycle=75;
            break;
          case 75:
            dutyCycle=100;
            break;
          case 100:
            dutyCycle=0;
            break;
          default:
            break;
        }
      }
    }
    
  }
  lastButtonState = readButton;
}

void setup() {
  pinMode(button, INPUT);
  pinMode(OC1A_PIN, OUTPUT);
  pinMode(OC3A_PIN, OUTPUT);
  pinMode(OC0B_PIN, OUTPUT);
  pinMode(OC4D_PIN, OUTPUT);

  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;
  TCCR1A |= (1 << COM1A1) | (1 << WGM11);
  TCCR1B |= (1 << WGM13) | (1 << CS10);
  ICR1 = TCNT1_TOP;

  TCCR3A = 0;
  TCCR3B = 0;
  TCNT3  = 0;
  TCCR3A |= (1 << COM3A1) | (1 << WGM31);
  TCCR3B |= (1 << WGM33) | (1 << CS30);
  ICR3 = TCNT3_TOP;

  TCCR0A = 0;
  TCCR0B = 0;
  TCNT0 = 0;
  TCCR0A |= (1 << COM0B1) | (1 << WGM00);
  TCCR0B |= (1 << WGM02) | (1 << CS01);
  OCR0A = 40;

  TCCR4A = 0;
  TCCR4B = 0;
  TCCR4C = 0;
  TCCR4D = 0;
  TCCR4E = 0;
  TCNT4 = 0;
  TCCR4B |= (1 << CS42);
  TCCR4C |= (1 << COM4D1) | (1 << PWM4D);
  TCCR4D |= (1 << WGM40);
  OCR4C = 40;
}

void loop() {
  checkButtonPress();
  setPwmDuty(dutyCycle);
}