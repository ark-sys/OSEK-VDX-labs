//
// Created by Khaled on 14/02/2021.
//

#include "myfunctions.h"
#include "Arduino.h"

// Make some sort of an alarm
void doTheBuzz(int buzzPin){
  tone(buzzPin, 2000);
  delay(20);
  noTone(buzzPin);
  delay(20);
  tone(buzzPin, 2000);
  delay(20);
  noTone(buzzPin);
  delay(20);
  tone(buzzPin, 2000);
  delay(20);
  noTone(buzzPin);
}

// Blink the led a little bit
void doTheBlink(int ledPin){

  digitalWrite(ledPin, HIGH);
  delay(10);
  digitalWrite(ledPin, LOW);
  delay(10);
  digitalWrite(ledPin, HIGH);
  delay(100);
  digitalWrite(ledPin, LOW);
  delay(10);
  digitalWrite(ledPin, HIGH);
  delay(10);
  digitalWrite(ledPin, LOW);
  delay(10);
}

// Get distance from ultrasound sensor
int getDistance(int echoPin, int trigPin){
  unsigned long lecture_echo = 0;
  int res = 0;

  /* Emit some ultrasound */
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(10);

  // listen to echo
  lecture_echo = pulseIn(ECHO_PIN, HIGH, 1000000L);

  // Evaluate distance according to speed of sound
  res = (lecture_echo * 0.034f / 2);

  return res;
}

void initPID(struct pid_controller_t * pidController){
  pidController->p_error = 0.0f;
  pidController->i_error = 0.0f;
  pidController->d_error = 0.0f;
  pidController->total_error = 0.0f;
}

void updateError(struct pid_controller_t * pidController, double newerror){
  pidController->p_error = newerror;
  pidController->i_error += newerror;
  pidController->d_error = newerror - pidController->p_error;

  pidController->total_error = KP * pidController->p_error + KD * pidController->d_error + KI * pidController->i_error;
}
