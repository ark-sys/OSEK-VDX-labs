#include "tpl_os.h"
#include "Arduino.h"
#include "../mylib/Servo.h"

#define TRIG_PIN 7
#define ECHO_PIN 6
#define RED_LED 3
#define BLUE_LED 2
#define BUZZ_PIN 13

#define ASTEP 8
#define BSTEP 9
#define CSTEP 10
#define DSTEP 11
#define STEP_DELAY 1

#define SERVO_PIN 5
#define SERVO_MIN 45
#define SERVO_CENTER 90
#define SERVO_MAX 135

unsigned char periodCounter = 0;
long distance_val = 0;
unsigned int orientation = 90; // Servo at center position
Servo servo;

void setup()
{
    // initialize sonar pins
    pinMode(TRIG_PIN, OUTPUT);
    digitalWrite(TRIG_PIN, LOW);
    pinMode(ECHO_PIN, INPUT);

    // initialize red and blue led pins
    pinMode(RED_LED, OUTPUT);
    pinMode(BLUE_LED, OUTPUT);

    // initialize buzzer pin
    pinMode(BUZZ_PIN, OUTPUT);

    // initialize step motor pin
    pinMode(ASTEP, OUTPUT);
    pinMode(BSTEP, OUTPUT);
    pinMode(CSTEP, OUTPUT);
    pinMode(DSTEP, OUTPUT);

    servo.attach(SERVO_PIN);
    servo.write(90);
    Serial.begin(9600);
}

void doTheBuzz(){
  tone(BUZZ_PIN, 2000);
  delay(100);
  noTone(BUZZ_PIN);
  delay(100);
  tone(BUZZ_PIN, 2000);
  delay(100);
  noTone(BUZZ_PIN);
  delay(100);
  tone(BUZZ_PIN, 2000);
  delay(100);
  noTone(BUZZ_PIN);
  tone(BUZZ_PIN, 2000);
  delay(100);
  noTone(BUZZ_PIN);
  delay(100);
}

void coilspin(int a,int b,int c,int d){
  digitalWrite(ASTEP,a);
  digitalWrite(BSTEP,b);
  digitalWrite(CSTEP,c);
  digitalWrite(DSTEP,d);
}

void onestep(){
  coilspin(1,0,0,0);
  delay(STEP_DELAY);
  coilspin(1,1,0,0);
  delay(STEP_DELAY);
  coilspin(0,1,0,0);
  delay(STEP_DELAY);
  coilspin(0,1,1,0);
  delay(STEP_DELAY);
  coilspin(0,0,1,0);
  delay(STEP_DELAY);
  coilspin(0,0,1,1);
  delay(STEP_DELAY);
  coilspin(0,0,0,1);
  delay(STEP_DELAY);
  coilspin(1,0,0,1);
  delay(STEP_DELAY);
}




TASK(navigation)
    {

    GetResource(distVal);

    Serial.print("Current distance is ");
    Serial.println(distance_val);

    if(periodCounter == 10){
      periodCounter = 0;
      orientation = random(SERVO_MIN,SERVO_MAX);
      servo.write(orientation);
    } else {
      periodCounter += 1;
    }

    if(distance_val == 0){

    }else {

        if(distance_val < 50 ){
          if(distance_val < 20){
            doTheBuzz();
          }
          servo.write(orientation);
          digitalWrite(RED_LED, HIGH);
          digitalWrite(BLUE_LED, LOW);
        } else {

          onestep();
          digitalWrite(RED_LED, LOW);
          digitalWrite(BLUE_LED, HIGH);
        }


    }

    ReleaseResource(distVal);
    TerminateTask();
    }


TASK(distance_detection)
    {

    unsigned long lecture_echo = 0;

    GetResource(distVal);

    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(10);

    lecture_echo = pulseIn(ECHO_PIN, HIGH, 1000000L);

    distance_val = lecture_echo * 0.034 / 2;

    ReleaseResource(distVal);

    TerminateTask();
    }