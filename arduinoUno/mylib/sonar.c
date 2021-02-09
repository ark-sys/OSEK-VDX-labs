//
// Created by Khaled on 09/02/2021.
//
#include "Arduino.h"



unsigned long get_distance(unsigned char echoPin, unsigned char trigPin){
  unsigned long distance = 0;
  unsigned long lecture_echo = 0;
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(10);
  lecture_echo = pulseIn(echoPin, HIGH, 1000000L);
  distance = lecture_echo * (0.034 / 2 );
  return distance;
}