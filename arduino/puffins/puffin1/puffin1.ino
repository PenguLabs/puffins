#include<Adafruit_SoftServo.h>jaw
#define SERVO1PIN 0 // jaw
#define SERVO2PIN 1 // ribs
Adafruit_SoftServo jawServo; // RIBS
Adafruit_SoftServo ribsServo; // MOUTH
const int minjaw = 15;
const int maxjaw = 45;
const int deltajaw = maxjaw - minjaw;

const int minribs = 110;
const int maxribs = 140;
const int deltaribs = maxribs-minribs;
double n = maxribs;
int i = 0;

const int motorDelay=30;
const int stepDelay=motorDelay*deltajaw;

const float ribsRatio = (float)(deltaribs)/(float)(deltajaw);


void setup() {
  OCR0A = 0xAF; //any number is OK
  TIMSK |= _BV(OCIE0A); // Turn on the compare interrupt
  
  jawServo.attach(SERVO1PIN);
  jawServo.setRange(544,2400);
  //jawServo.write(20);
  jawServo.write(maxjaw);
  
  ribsServo.attach(SERVO2PIN);
  ribsServo.setRange(544,2400);
  ribsServo.write(maxribs);
  
  delay(15);
}

void loop()
{
  
  ßbreathe();
  breatheAndMouth();
  
} 


void breathe()
{
 
  double r = maxribs;
  for(i=0;i<deltajaw;i++){
    if(r>=minribs)
      ribsServo.write(r);
    r-= ribsRatio;
    delay(motorDelay);
  }
  delay(stepDelay);
  for(i=0;i<deltajaw;i++){
    if(r<=maxribs)
      ribsServo.write(r);
    r+= ribsRatio;
    delay(motorDelay);
  }
  delay(stepDelay); 
}
void breatheAndMouth()
{

  double r = maxribs;
  for(i=0;i<deltajaw;i++){
    jawServo.write(maxjaw-i);
    if(r>=minribs)
      ribsServo.write(r);
    r-= ribsRatio;
    delay(motorDelay);
  }
  delay(stepDelay);
  for(i=0;i<deltajaw;i++){
    jawServo.write(minjaw+i);
    if(r<=maxribs)
      ribsServo.write(r);
    r+= ribsRatio;
    delay(motorDelay);
  }
  delay(stepDelay);
}

volatile uint8_t counter = 0;
SIGNAL(TIMER0_COMPA_vect){
  counter += 2;
  
  if(counter >= 20){
    counter = 0;
    jawServo.refresh();
    ribsServo.refresh();
  }
}

