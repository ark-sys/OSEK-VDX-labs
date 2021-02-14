#include "Arduino.h"
#include "tpl_os.h"

// ============================== HEADERS THAT SHOULD BE IN A SEPARATE FILE =======================================================

//=================================== PIN DEFINITION ===================================
#define TRIG_PIN 3
#define ECHO_PIN 2
#define RED_LED 13
#define BLUE_LED 12
#define BUZZ_PIN 11
#define IMPACT_PIN 4

//=================================== PID CONTROLLER DEFINITION ===================================
#define KP 1
#define KI 0.2
#define KD 0.5

struct pid_controller_t{
  double p_error;
  double i_error;
  double d_error;
  double total_error;
};

// ======================================== END OF HEADERS THAT SHOULD BE IN A SEPARATE FILE


long distance_val = 0; // value of distance retrieved by sonar
double setPoint = 25.0f;

unsigned char periodCounter = 0; // count number of periods, if periodCounter==9 then take random direction

unsigned long currTimer; // Value used for timestamping
int buttonState = 0; // State of fake crash button is active

struct pid_controller_t steer_pid;
struct pid_controller_t throttle_pid;


// ======================================== FUNCTIONS THAT SHOULD BE IN A SEPARATE FILE ============================================


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
long getDistance(int echoPin, int trigPin){
  unsigned long lecture_echo = 0;
  long res = 0;

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

void updateError(struct pid_controller_t * pidController, double setPoint, double newData){
  pidController->p_error = setPoint - newData;
  pidController->i_error += pidController->p_error * 0.2f; // 200ms
  pidController->d_error = (pidController->p_error - newData)/0.2f; // 200ms

  pidController->total_error = KP * pidController->p_error + KD * pidController->d_error + KI * pidController->i_error;
}

// ================================================== END OF FUNCTIONS THAT SHOULD BE IN A SEPARATE FILE ======================================================


void setup()
{
  // initialize serial port at baudrate of 9600 for communication
  Serial.begin(9600);

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
    currTimer = millis();
    Serial.print(currTimer);
    Serial.print("ms - Distance :  ");
    Serial.println(distance_val);


    /*
     * if current distance is equal to zero, then Reverse and take a different path
     * */
    if(distance_val == 0){
      Serial.println("Reversing...");
      digitalWrite(RED_LED, HIGH);
      digitalWrite(BLUE_LED, LOW);
    }else {
        /*
         * If distance is <50cm then turn red led on and turn off the blue led
         * else, keep the blue led on
         * */
        if(distance_val < 50 ){
          digitalWrite(RED_LED, HIGH);
          digitalWrite(BLUE_LED, LOW);
          /*  */
          if(distance_val < 5){
          //  doTheBuzz(BUZZ_PIN);
            doTheBlink(RED_LED);
            Serial.println("Imminent collision!");
          }
        } else {
          digitalWrite(RED_LED, LOW);
          digitalWrite(BLUE_LED, HIGH);
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
    updateError(&throttle_pid,setPoint, distance_val);
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
      distance_val = getDistance(ECHO_PIN, TRIG_PIN);
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
        distance_val = 0;
        ReleaseResource(distVal);
      }

      // release mutex on collision sensor state
      ReleaseResource(contactState);

      // end task
      TerminateTask();
    }