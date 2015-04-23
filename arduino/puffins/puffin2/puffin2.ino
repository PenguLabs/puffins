#include<Adafruit_SoftServo.h>
#define SERVO1PIN 0
#define SERVO2PIN 1

Adafruit_SoftServo hipServo;
Adafruit_SoftServo neckServo;
const int minFeet = 50;
const int maxFeet = 130;
const int deltaFeet = maxFeet - minFeet;

const int minNeck = 95;
const int maxNeck = 145;
const int deltaNeck = maxNeck - minNeck;
int i = 0;

const int motorDelay=2;
const int stepDelay=motorDelay*deltaFeet;

float n = maxNeck;
const float neckRatio = (float)(deltaNeck)/(float)(2*deltaFeet);


void setup() {
  OCR0A = 0xAF; //any number is OK
  TIMSK |= _BV(OCIE0A); // Turn on the compare interrupt
  
  hipServo.attach(SERVO1PIN);
  hipServo.write(minFeet+deltaFeet/2);
  
  neckServo.attach(SERVO2PIN);
  neckServo.write(maxNeck);
  delay(15);
}

void loop()
{
  leftStepNeckDown();
  delay(stepDelay);
  leftStep();
  delay(stepDelay);
  rightStepNeckUp();
  delay(stepDelay);
  rightStep();
  delay(stepDelay);
  leftStepNeckDown();
  delay(1.5*stepDelay);
  leftStep();
  delay(.5*stepDelay);
  rightStepNeckUp();
  delay(stepDelay);
  delay(deltaFeet*motorDelay+stepDelay);
} 

void leftStep(){
  for(i=deltaFeet/2;i<deltaFeet;i++){
    hipServo.write(minFeet+i);
    delay(motorDelay);
  }  
  for(i=deltaFeet; i>deltaFeet/2;i--){
    hipServo.write(minFeet+i);
    delay(motorDelay);
  }
}

void leftStepNeckDown(){
  for(i=deltaFeet/2;i<deltaFeet;i++){
    hipServo.write(minFeet+i);
    if(n >= minNeck)
      neckServo.write(n);
    n-=neckRatio;
    delay(motorDelay);
  }  
  for(i=deltaFeet; i>deltaFeet/2;i--){
    hipServo.write(minFeet+i);
    if(n >= minNeck)
      neckServo.write(n);
    n-=neckRatio;
    delay(motorDelay);
  }
}

void rightStep(){
  for(i=deltaFeet/2;i>0;i--){
    hipServo.write(minFeet+i);
    delay(motorDelay);
  }
  for(i=0; i<deltaFeet/2;i++){
    hipServo.write(minFeet+i);
    delay(motorDelay);
  }
}


void rightStepNeckUp(){
  for(i=deltaFeet/2;i>0;i--){
    hipServo.write(minFeet+i);
    if(n <= maxNeck)
      neckServo.write(n);
    n += neckRatio;
    delay(motorDelay);
  }
  for(i=0; i<deltaFeet/2;i++){
    hipServo.write(minFeet+i);
    if(n <= maxNeck)
      neckServo.write(n);
    n += neckRatio;
    delay(motorDelay);
  }
}


volatile uint8_t counter = 0;
SIGNAL(TIMER0_COMPA_vect){
  counter += 2;
  
  if(counter >= 20){
    counter = 0;
    hipServo.refresh();
    neckServo.refresh();
  }
}

