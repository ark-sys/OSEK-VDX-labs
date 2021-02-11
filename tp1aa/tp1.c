#include "../ecrobot_emul.h"
#include "tpl_os.h"
#include "tpl_posixvp_irq_gen.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>

int distance_val = 0;
struct timespec current_time;

int main(void)
{
    StartOS(OSDEFAULTAPPMODE);
    return 0;
}

void PreTaskHook(void)
{
  TaskType id;
  GetTaskID(&id);
  printf("This is a prehook -> Task id: %d\r\n",id);
}

void PostTaskHook(void)
{
  TaskType id;
  GetTaskID(&id);
  printf("This is a posthook -> Task id: %d\r\n\n",id);
}
void StartupHook(void){
  ecrobot_init_sonar_sensor(NXT_PORT_S2);
}

void ShutdownHook(void){
  ecrobot_term_sonar_sensor(NXT_PORT_S2);
}

TASK(detection_contact)
    {

    if (ecrobot_get_touch_sensor(NXT_PORT_S1)==1){
       GetResource(distVal);

       distance_val = 0;
       ReleaseResource(distVal);
    }

    TerminateTask();
    }

TASK(distance_detection)
    {
    GetResource(distVal);

    distance_val = ecrobot_get_sonar_sensor(NXT_PORT_S2);
    ReleaseResource(distVal);


    TerminateTask();

    }

TASK(navigation)
    {

      GetResource(distVal);
      clock_gettime(CLOCK_MONOTONIC_RAW, &current_time);
      printf("%lu Distance is %d\r\n",(current_time.tv_sec*1000000+current_time.tv_nsec/1000000), distance_val);

      if (distance_val == 0 ){
          printf("marche arriere\r\n");
        } else if (distance_val < 50){
          printf("tourner\r\n");
        }
      ReleaseResource(distVal);


    TerminateTask();
    }

TASK(stop)
        {
        printf("\tEXIT TASK Shutdown\r\n");
        ShutdownOS(E_OK);
        TerminateTask();
        }
