#include "Arduino.h"
#include "tpl_os.h"
#include "ecrobot_emul.h"
#include "Drive.h"

//=================================== PIN DEFINITION ===================================

#define IMPACT_PIN 10

#define IN1_A 16
#define IN1_B 17

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

double setPoint = 25.0f; // Fixed target for the PID controller
int speed = 0; // Global variable that holds the speed value
unsigned char periodCounter = 0; // count number of periods, if periodCounter==9 then take random direction

unsigned long currTimer; // Value used for timestamping
int buttonState = 0; // State of fake crash button is active

struct pid_controller_t steer_pid;
struct pid_controller_t throttle_pid;

// ================================================== END OF FUNCTIONS THAT SHOULD BE IN A SEPARATE FILE ======================================================


Drive drive(IN1_A, IN1_B, IN2_A, IN2_B);  //Create an instance of the function


void setup()
{
  // initialize serial port at baudrate of 9600 for communication
  Serial.begin(9600);

  pinMode(SONAR_TRIG_1, OUTPUT);
  digitalWrite(SONAR_TRIG_1, LOW);
  pinMode(SONAR_ECHO_1, INPUT);

  pinMode(SONAR_TRIG_2, OUTPUT);
  digitalWrite(SONAR_TRIG_2, LOW);
  pinMode(SONAR_ECHO_2, INPUT);

  pinMode(IMPACT_PIN, INPUT);

  // initialize pid controllers for steering and throttle control
  initPID(&steer_pid);
  initPID(&throttle_pid);
}


/*
 * Update info of PID controller and print some events on serial port
 *
 * */
TASK(navigation)
    {
    /* if this is the tenth period, then chose a random orientation for main wheel */
    if(periodCounter == 9){
      periodCounter = 0;
      Serial.println("Taking random direction...");
    } else {
      periodCounter += 1;
    }

    // lock mutex
    GetResource(distVal);
    /* get current time and distance and send message via serial port*/
    currTimer = systick_get_ms();
    Serial.print(currTimer);
    Serial.print("ms - Distance_l :  ");
    Serial.print(distance_val_l);
    Serial.print(" - Distance_r :  ");
    Serial.println(distance_val_r);

    /*
     * if current distance is equal to zero, then Reverse and take a different path
     * */
    if((distance_val_l == 0) || (distance_val_r == 0)){
      Serial.println("Reversing...");
      drive.moveBackward(10);
    }else {
        /*
         * If distance is <50cm then turn red led on and turn off the blue led
         * else, keep the blue led on
         * */
        if((distance_val_l < 20) || (distance_val_r < 20) ){

          /*  */
          if((distance_val_l < 5) || (distance_val_r < 5)){
            drive.stopMoving();
            Serial.println("Imminent collision!");
          }
        } else {
            Serial.println("Moving forward!");

            drive.moveForward(150);
        }
    // go forward

    }

    ReleaseResource(distVal);
    TerminateTask();
    }


/*
 * interact with motors according to values from PID controller
 *
 * */
TASK(guidance)
    {

    GetResource(distVal);
    updateError(&throttle_pid,setPoint, distance_val_l);
    ReleaseResource(distVal);

    Serial.print("\tcontroller error ");
    Serial.println(throttle_pid.total_error);
    TerminateTask();
    }

/* Update distance value using sonar every 300ms
   If collision sensor is triggered, distance will not update
  */
TASK(distance_detection)
    {
    // lock mutex on collision sensor state
    GetResource(contactState);
    // if sensor is not activated then sonar sensor is accessible
    if(buttonState == HIGH){
      /*
       * Update the distance with sonar and release the mutex
       * */
      GetResource(distVal);

      /*
       *
       * Get distance from left sonar info
       *
       * */
      digitalWrite(SONAR_TRIG_1, HIGH);
      delayMicroseconds(10);
      digitalWrite(SONAR_TRIG_1, LOW);
      delayMicroseconds(10);

      // Listen to echo
      echo_val = pulseIn(SONAR_ECHO_1, HIGH, 1000000L);
      // Evaluate distance according to speed of sound
      distance_val_l = (echo_val * 0.034f)/2;

      /**
       * Get distance from left sonar info
       *
       * */
        digitalWrite(SONAR_TRIG_2, HIGH);
        delayMicroseconds(10);
        digitalWrite(SONAR_TRIG_2, LOW);
        delayMicroseconds(10);
        // Listen to echo
        echo_val = pulseIn(SONAR_ECHO_2, HIGH, 1000000L);
        // Evaluate distance according to speed of sound
      distance_val_r = (echo_val * 0.034f)/2;

      ReleaseResource(distVal);
    }
    // release mutex on collision sensor state
    ReleaseResource(contactState);
    // chain to guidance task that will update pid controller with new values
    ChainTask(guidance);
    }

/*
 *  Check collision sensor state every 100ms.
 *  If activated, then set distance to 0
 * */
TASK(contact_detection)
    {
      // lock mutex on collision sensor state
      GetResource(contactState);

      /*
       * Check that the sensor is not enabled
       * */
      buttonState = digitalRead(IMPACT_PIN);
      if(buttonState == LOW){
        /*
         * get mutex of distance value and set the distance to 0
         * */
        GetResource(distVal);
        distance_val_l = 0;
        distance_val_r = 0;
        ReleaseResource(distVal);
      }

      // release mutex on collision sensor state
      ReleaseResource(contactState);

      // end task
      TerminateTask();
    }