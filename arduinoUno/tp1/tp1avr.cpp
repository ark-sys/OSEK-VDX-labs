#include "tpl_os.h"
#include "Arduino.h"
#include "../mylib/Servo.h"

#define TRIG_PIN 3
#define ECHO_PIN 2
#define RED_LED 13
#define BLUE_LED 12
#define BUZZ_PIN 11
#define IMPACT_PIN 4

#define ASTEP 8
#define BSTEP 9
#define CSTEP 10
#define DSTEP 11
#define STEP_DELAY 1

#define SERVO_PIN 4
#define SERVO_MIN 45
#define SERVO_CENTER 90
#define SERVO_MAX 135

Servo servo; // declaring servo motor
unsigned char periodCounter = 0; // count number of periods, if periodCounter==9 then take random direction
long distance_val = 0; // value of distance retrieved by sonar
unsigned int orientation = 90; // Servo at center position

unsigned long currTimer; // Value used for timestamping
int buttonState = 0;

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

    // initialize button pin (emulates collision sensor)
    pinMode(IMPACT_PIN, INPUT);

//    servo.attach(SERVO_PIN);
//    servo.write(90);
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




TASK(navigation)
    {
    // retrieve mutex
    GetResource(distVal);
    /* get current time and distance and send message via serial port*/
    currTimer = millis();
    Serial.print(currTimer);
    Serial.print("ms - Distance :  ");
    Serial.println(distance_val);

    /* if this is the tenth period, then chose a random orientation for main wheel */
    if(periodCounter == 9){
      periodCounter = 0;

/*      orientation = random(SERVO_MIN,SERVO_MAX);
      servo.write(orientation);*/
      Serial.println("Taking random direction...");
    } else {
      periodCounter += 1;
    }

    /*
     * if current distance is equal to zero, then Reverse and take a different path
     * */
    if(distance_val == 0){
      Serial.println("Reversing");
      digitalWrite(RED_LED, HIGH);
      digitalWrite(BLUE_LED, LOW);
    }else {
        /*
         * If distance is <50cm then turn red led on and turn off the blue led
         * else, keep the blue led on
         * */
        if(distance_val < 50 ){
          /*  */
          if(distance_val < 20){
            doTheBuzz();
            Serial.println("Imminent collision!");
          }
//          servo.write(orientation);
          digitalWrite(RED_LED, HIGH);
          digitalWrite(BLUE_LED, LOW);
        } else {
          digitalWrite(RED_LED, LOW);
          digitalWrite(BLUE_LED, HIGH);
        }
    // go forward

    }

    ReleaseResource(distVal);
    TerminateTask();
    }


TASK(distance_detection)
    {

    unsigned long lecture_echo = 0;

    GetResource(contactState);
    if(buttonState == HIGH){

      GetResource(distVal);

      digitalWrite(TRIG_PIN, HIGH);
      delayMicroseconds(10);
      digitalWrite(TRIG_PIN, LOW);
      delayMicroseconds(10);

      lecture_echo = pulseIn(ECHO_PIN, HIGH, 1000000L);

      distance_val = lecture_echo * 0.034 / 2;

      ReleaseResource(distVal);

    }
    ReleaseResource(contactState);


    TerminateTask();
    }

TASK(contact_detection)
    {
      GetResource(contactState);
      buttonState = digitalRead(IMPACT_PIN);

      if(buttonState == LOW){
        GetResource(distVal);
        distance_val = 0;
        ReleaseResource(distVal);
      }

      ReleaseResource(contactState);

      TerminateTask();

    }