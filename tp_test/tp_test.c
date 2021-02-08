#include "../ecrobot_emul.h"
#include "tpl_os.h"
#include "tpl_posixvp_irq_gen.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>

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

TASK(a_task)
        {
                printf("\tTASK 0: Here we are doing some stuff\r\n");
                ActivateTask(another_task);
                TerminateTask();
        }

TASK(another_task)
        {
        printf("\tTASK 1: some other stuff is done here\r\n");

        printf ( "\t\tYo dude\r\n");
        printf ( "\t\tHit 'a' or 'b'\r\n");

        systick_wait_ms(3000);


        ChainTask(another_cool_task);
        }

TASK(another_cool_task)
        {
        printf("\tTASK 2: some crazy other stuff is done here\r\n");

        time_t rawtime;
        struct tm * timeinfo;
        time ( &rawtime );
        timeinfo = localtime ( &rawtime );
        printf ( "\t\tCurrent local time and date: %s\r\n", asctime (timeinfo) );
        TerminateTask();
        }

ISR(a_interrupt) {
    printf("\t\t\t\tA freakin' SIGTERM interrupt.\r\n");
}

ISR (b_interrupt) {
    printf("\t\t\t\tA freakin' SIGTRAP interrupt.\r\n");

}

TASK(stop)
        {
        printf("\tEXIT TASK Shutdown\r\n");
        ShutdownOS(E_OK);
        TerminateTask();
        }
