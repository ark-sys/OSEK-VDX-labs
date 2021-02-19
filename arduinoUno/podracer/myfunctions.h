//
// Created by Khaled on 14/02/2021.
//

#ifndef TRAMPOLINE_UTILITY_H
#define TRAMPOLINE_UTILITY_H

//=================================== PIN DEFINITION ===================================
#define TRIG_PIN 3
#define ECHO_PIN 2
#define RED_LED 13
#define BLUE_LED 12
#define BUZZ_PIN 11
#define IMPACT_PIN 4

//=================================== PID CONTROLLER DEFINITION ===================================
#define KP 0.5
#define KI 0.2
#define KD 2

struct pid_controller_t{
    double p_error;
    double i_error;
    double d_error;
    double total_error;
};

extern void doTheBuzz(int buzzPin);
extern void doTheBlink(int ledPin);
extern int getDistance(int echoPin, int trigPin);

extern void initPID(struct pid_controller_t * pidController);
extern void updateError(struct pid_controller_t * pidController, double newerror);
#endif // TRAMPOLINE_UTILITY_H
