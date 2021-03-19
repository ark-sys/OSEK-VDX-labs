#include "Arduino.h"
#include "tpl_os.h"
#include "Drive.h"

//=================================== PIN DEFINITION ===================================

#define IMPACT_PIN 10

#define IN1_A 17
#define IN1_B 16

#define IN2_A 14
#define IN2_B 15


#define SONAR_TRIG_1 3
#define SONAR_ECHO_1 2
#define SONAR_TRIG_2 5
#define SONAR_ECHO_2 4
// ======================================== END OF HEADERS THAT SHOULD BE IN A SEPARATE FILE


long echo_val = 0; // Duration of echo measured by ultrsound sensor
long distance_val_l = 0; // value of distance retrieved by left sonar
long distance_val_r = 0; // value of distance retrieved by right sonar

double setPoint = 25.0f;

unsigned char periodCounter = 0; // count number of periods, if periodCounter==9 then take random direction

unsigned long currTimer; // Value used for timestamping
int buttonState = 0; // State of fake crash button is active
// ================================================== END OF FUNCTIONS THAT SHOULD BE IN A SEPARATE FILE ======================================================


Drive drive(IN1_A, IN1_B, IN2_A, IN2_B);  //Create an instance of the function
int in1a_state = 0;
int in1b_state = 0;
int in2a_state = 0;
int in2b_state = 0;

void setup()
{
  // initialize serial port at baudrate of 9600 for communication
  Serial.begin(9600);

}
int speed = 100;

/*
 * Update info of PID controller and print some events on serial port
 *
 * */
TASK(navigation)
    {
    speed++;
    Serial.print("driving forward at ");
    Serial.println(speed);
    drive.moveForward(speed);
    delayMicroseconds(1000);

    in1a_state = analogRead(IN1_A);
    Serial.print("Current IN1_A holds : ");
    Serial.println(in1a_state);

    in1b_state = analogRead(IN1_B);
    Serial.print("Current IN1_B holds : ");
    Serial.println(in1b_state);

    in2a_state = analogRead(IN2_A);
    Serial.print("Current IN2_A holds : ");
    Serial.println(in2a_state);

    in2b_state = analogRead(IN2_B);
    Serial.print("Current IN2_B holds : ");
    Serial.println(in2b_state);
    TerminateTask();
    }

