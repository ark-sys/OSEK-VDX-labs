// see ultrasound.oil head of file for information about the compilation process.

#include "tpl_os.h"
#include "Arduino.h"

const int trigPin = 7;
const int echoPin = 6;
const int redLed = 3;
const int blueLed = 2;

unsigned long lecture_echo;
long distance;

void setup()
{
    // initialize digital pin 13 as an output.
    pinMode(trigPin, OUTPUT);
    digitalWrite(trigPin, LOW);
    pinMode(echoPin, INPUT);
    pinMode(redLed, OUTPUT);
    pinMode(blueLed, OUTPUT);


  Serial.begin(9600);
}

//The TASK is activated by the alarm "periodicAl":
//* The alarm "periodicAl" is configured in the .oil file to activate task 
//"periodicTask" each 1000 pulses of counter SystemCounter.
//* The SystemCounter is the same as the systick used in Arduino, a tick each 1024us on a 16MHz arduino.
//* This task is periodic, with a period of 1024ms.
TASK(periodicTask)
{

        digitalWrite(trigPin, HIGH);
        delayMicroseconds(10);
        digitalWrite(trigPin, LOW);
        delayMicroseconds(10);
        lecture_echo = pulseIn(echoPin, HIGH);
        distance = lecture_echo * (0.034 / 2 );
        Serial.print("Current distance is ");
        Serial.println(distance);

        if(distance < 20 ){
          digitalWrite(redLed, HIGH);
          digitalWrite(blueLed, LOW);
        } else {
          digitalWrite(redLed, LOW);
          digitalWrite(blueLed, HIGH);
        }




}

