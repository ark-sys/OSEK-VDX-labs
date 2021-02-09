#include "tpl_os.h"
#include "Arduino.h"
#define TRIG_PIN 7
#define ECHO_PIN 6
#define RED_LED 3
#define BLUE_LED 2



long distance_val = 0;


void setup()
{
    // initialize digital pin 13 as an output.
    pinMode(TRIG_PIN, OUTPUT);
    digitalWrite(TRIG_PIN, LOW);
    pinMode(ECHO_PIN, INPUT);
    pinMode(RED_LED, OUTPUT);
    pinMode(BLUE_LED, OUTPUT);
    Serial.begin(9600);
}




TASK(navigation)
    {

    GetResource(distVal);

    Serial.print("Current distance is ");
    Serial.println(distance_val);

    if(distance_val < 50 ){
      digitalWrite(RED_LED, HIGH);
      digitalWrite(BLUE_LED, LOW);
    } else {
      digitalWrite(RED_LED, LOW);
      digitalWrite(BLUE_LED, HIGH);
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