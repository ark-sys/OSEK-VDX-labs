#include "../ecrobot_emul.h"
#include "tpl_os.h"
#include "tpl_posixvp_irq_gen.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>

int distance_val = 0;

int main(void)
{
    StartOS(OSDEFAULTAPPMODE);
    return 0;
}
DeclareAlarm(alarm_contact);
DeclareAlarm(alarm_distance);
DeclareAlarm(alarm_nav);
DeclareAlarm(alarm_stop);

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
       GetResource(distance_val);

       distance_val = 0;
       ReleaseResource(distance_val);
    }

    TerminateTask();

    }

TASK(detection_distance)
    {
    GetResource(distance_val);

    distance_val = ecrobot_get_sonar_sensor(NXT_PORT_S2);
    ReleaseResource(distance_val);


    TerminateTask();

    }

TASK(navigation)
    {

      GetResource(distance_val);

      printf("Distance is %d\r\n", distance_val);

      if (distance_val == 0 ){
          printf("marche arriere\r\n");
        } else if (distance_val < 50){
          printf("tourner\r\n");
        }
      ReleaseResource(distance_val);


    TerminateTask();
    }

TASK(stop)
        {
        printf("\tEXIT TASK Shutdown\r\n");
        ShutdownOS(E_OK);
        TerminateTask();
        }
