/**
 * @file tpl_os.c
 *
 * @section File description
 *
 * This file gathers system call declarations.
 *
 * Generated from application only_one_periodic_task
 * Automatically generated by goil on Tue Feb  9 06:53:54 2021
 * from root OIL file tp1.oil
 *
 * @section File informations
 *
 * $Date$
 * $Rev$
 * $Author$
 * $URL$
 */

#include "tpl_os.h"

#include "tpl_os_os_kernel.h"
#include "tpl_os_os.h"
#include "tpl_os_interrupt_kernel.h"
#include "tpl_os_task_kernel.h"
#include "tpl_os_resource_kernel.h"
#include "tpl_os_alarm_kernel.h"

/*--------------------------------------------------------------------------*
 * os system calls.
 * corresponding kernel implementation is in files
 * tpl_os_os_kernel.h and tpl_os_os_kernel.c
 * tpl_os_os.h and tpl_os_os.c
 *--------------------------------------------------------------------------*/

/*
 * GetActiveApplicationMode
 *
 * This service returns the current application mode. It may be used to 
 * write mode dependent code. 
 *
 *
 * Return value:
 * The active application mode 
 */
FUNC(AppModeType, OS_CODE) GetActiveApplicationMode(void)
{
  return tpl_get_active_application_mode_service();
}

/*
 * tpl_start_os
 *
 * The user can call this system service to start the operating system in 
 * a specific mode. 
 *
 * Note: StartOS is provided in tpl_os_os.c.
 *       StartOS calls tpl_start_os.
 *       tpl_start_os should not be called directly by the application
 *
 * mode: The application mode 
 */
FUNC(void, OS_CODE) tpl_start_os(CONST(AppModeType, AUTOMATIC) mode)
{
  tpl_start_os_service(mode);
}

/*
 * ShutdownOS
 *
 * The user can call this system service to abort the overall system 
 * (e.g. emergency off). The operating system also calls this function 
 * internally, if it has reached an undefined internal state and is no 
 * longer ready to run. 
 *
 * error: The error that occured 
 */
FUNC(void, OS_CODE) ShutdownOS(CONST(StatusType, AUTOMATIC) error)
{
  tpl_shutdown_os_service(error);
}

/*--------------------------------------------------------------------------*
 * interrupt system calls.
 * corresponding kernel implementation is in files
 * tpl_os_interrupt_kernel.h and tpl_os_interrupt_kernel.c
 *--------------------------------------------------------------------------*/

/*
 * EnableAllInterrupts
 *
 * This service restores the state saved by DisableAllInterrupts. 
 *
 */
FUNC(void, OS_CODE) EnableAllInterrupts(void)
{
  tpl_enable_all_interrupts_service();
}

/*
 * DisableAllInterrupts
 *
 * This service disables all interrupts for which the hardware supports 
 * disabling. The state before is saved for the EnableAllInterrupts call. 
 *
 */
FUNC(void, OS_CODE) DisableAllInterrupts(void)
{
  tpl_disable_all_interrupts_service();
}

/*
 * ResumeAllInterrupts
 *
 * This service restores the recognition status of all interrupts saved 
 * by the SuspendAllInterrupts service. 
 *
 */
FUNC(void, OS_CODE) ResumeAllInterrupts(void)
{
  tpl_resume_all_interrupts_service();
}

/*
 * SuspendAllInterrupts
 *
 * This service saves the recognition status of all interrupts and 
 * disables all interrupts for which the hardware supports disabling. 
 *
 */
FUNC(void, OS_CODE) SuspendAllInterrupts(void)
{
  tpl_suspend_all_interrupts_service();
}

/*
 * ResumeOSInterrupts
 *
 * This service restores the recognition status of interrupts saved by 
 * the SuspendOSInterrupts service. 
 *
 */
FUNC(void, OS_CODE) ResumeOSInterrupts(void)
{
  tpl_resume_os_interrupts_service();
}

/*
 * SuspendOSInterrupts
 *
 * This service saves the recognition status of interrupts of category 2 
 * and disables the recognition of these interrupts. 
 *
 */
FUNC(void, OS_CODE) SuspendOSInterrupts(void)
{
  tpl_suspend_os_interrupts_service();
}

/*
 * CallTerminateISR2
 *
 * 
 *
 */
FUNC(void, OS_CODE) CallTerminateISR2(void)
{
  tpl_call_terminate_isr2_service();
}

/*--------------------------------------------------------------------------*
 * task system calls.
 * corresponding kernel implementation is in files
 * tpl_os_task_kernel.h and tpl_os_task_kernel.c
 *--------------------------------------------------------------------------*/

/*
 * ActivateTask
 *
 * Activate a task 
 *
 * task_id: The identifier of the task to activate 
 *
 * Return value:
 * E_OK: No error (Standard & Extended) 
 * E_OS_LIMIT: Too many activations of <task_id> (Standard & Extended) 
 * E_OS_ID: <task_id> is invalid (Extended) 
 */
FUNC(StatusType, OS_CODE) ActivateTask(CONST(TaskType, AUTOMATIC) task_id)
{
  return tpl_activate_task_service(task_id);
}

/*
 * TerminateTask
 *
 * Terminate the calling task 
 *
 *
 * Return value:
 * E_OS_RESOURCE: The calling task still occupies a resource (Extended) 
 * E_OS_CALLEVEL: Call at interrupt level (Extended) 
 */
FUNC(StatusType, OS_CODE) TerminateTask(void)
{
  return tpl_terminate_task_service();
}

/*
 * ChainTask
 *
 * Terminate the calling task and activate task task_id 
 *
 * task_id: The identifier of the task to chain to 
 *
 * Return value:
 * E_OS_LIMIT: Too many activations of <task_id> (Standard & Extended) 
 * E_OS_ID: <task_id> is invalid (Extended) 
 * E_OS_RESOURCE: The calling task still occupies a resource (Extended) 
 * E_OS_CALLEVEL: Call at interrupt level (Extended) 
 */
FUNC(StatusType, OS_CODE) ChainTask(CONST(TaskType, AUTOMATIC) task_id)
{
  return tpl_chain_task_service(task_id);
}

/*
 * Schedule
 *
 * Call the scheduler 
 *
 *
 * Return value:
 * E_OK: No error (Standard & Extended) 
 * E_OS_RESOURCE: The calling task still occupies a resource (Extended) 
 * E_OS_CALLEVEL: Call at interrupt level (Extended) 
 */
FUNC(StatusType, OS_CODE) Schedule(void)
{
  return tpl_schedule_service();
}

/*
 * GetTaskID
 *
 * Get the id of the calling task 
 *
 * task_id: A pointer to the var where the identifier of the task will be 
 *           stored 
 *
 * Return value:
 * E_OK: No error (Standard & Extended) 
 */
FUNC(StatusType, OS_CODE) GetTaskID(VAR(TaskRefType, AUTOMATIC) task_id)
{
  return tpl_get_task_id_service(task_id);
}

/*
 * GetTaskState
 *
 * Get the task state of a task 
 *
 * task_id: The identifier of the task 
 * state: A pointer to the var where the state of the task will be stored 
 *
 * Return value:
 * E_OK: No error (Standard & Extended) 
 * E_OS_ID: <alarm_id> is invalid (Extended) 
 */
FUNC(StatusType, OS_CODE) GetTaskState(CONST(TaskType, AUTOMATIC) task_id,VAR(TaskStateRefType, AUTOMATIC) state)
{
  return tpl_get_task_state_service(task_id, state);
}

/*
 * CallTerminateTask
 *
 * Used internally as return address of tasks, should not be called by 
 * the user 
 *
 */
FUNC(void, OS_CODE) CallTerminateTask(void)
{
  tpl_call_terminate_task_service();
}

/*--------------------------------------------------------------------------*
 * resource system calls.
 * corresponding kernel implementation is in files
 * tpl_os_resource_kernel.h and tpl_os_resource_kernel.c
 *--------------------------------------------------------------------------*/

/*
 * GetResource
 *
 * Get resource res_id. As a result, the priority of the caller may be 
 * raised to the priority of the resource if the latter is higher 
 *
 * res_id: The id of the resource to get. 
 *
 * Return value:
 * E_OK: No error (Standard & Extended) 
 * E_OS_ID: <res_id> is invalid (Extended) 
 */
FUNC(StatusType, OS_CODE) GetResource(CONST(ResourceType, AUTOMATIC) res_id)
{
  return tpl_get_resource_service(res_id);
}

/*
 * ReleaseResource
 *
 * Release resource res_id. The priority of the caller returns to the 
 * priority it had before getting the resource 
 *
 * res_id: The id of the resource to release. 
 *
 * Return value:
 * E_OK: No error (Standard & Extended) 
 * E_OS_ID: <res_id> is invalid (Extended) 
 * E_OS_ACCESS: Attempt to get a resource which is already occupied by 
 *     any task or ISR, or the statically assigned priority of the 
 *     calling task or interrupt routine is higher than the calculated 
 *     ceiling priority (Extended) 
 */
FUNC(StatusType, OS_CODE) ReleaseResource(CONST(ResourceType, AUTOMATIC) res_id)
{
  return tpl_release_resource_service(res_id);
}

/*--------------------------------------------------------------------------*
 * alarm system calls.
 * corresponding kernel implementation is in files
 * tpl_os_alarm_kernel.h and tpl_os_alarm_kernel.c
 *--------------------------------------------------------------------------*/

/*
 * GetAlarmBase
 *
 * Get information about the underlying counter. 
 * See page 63 of the OSEK OS 2.2.3 specification. 
 *
 * alarm_id: The identifier of the alarm 
 * info: A pointer to the AlarmBaseType data where the informations will 
 *        be stored 
 *
 * Return value:
 * E_OK: No error (Standard & Extended) 
 * E_OS_ID: <alarm_id> is invalid (Extended) 
 */
FUNC(StatusType, OS_CODE) GetAlarmBase(CONST(AlarmType, AUTOMATIC) alarm_id,VAR(AlarmBaseRefType, AUTOMATIC) info)
{
  return tpl_get_alarm_base_service(alarm_id, info);
}

/*
 * GetAlarm
 *
 * Get the remaining number of ticks before the alarm expire. 
 * See page 63 of the OSEK OS 2.2.3 specification. 
 *
 * alarm_id: The identifier of the alarm 
 * tick: A pointer to the TickType data where the remaining number of 
 *        ticks before the alarm expire will be stored 
 *
 * Return value:
 * E_OK: No error (Standard & Extended) 
 * E_OS_NOFUNC: Alarm <alarm_id> is not in use (Standard & Extended) 
 * E_OS_ID: <alarm_id> is invalid (Extended) 
 */
FUNC(StatusType, OS_CODE) GetAlarm(CONST(AlarmType, AUTOMATIC) alarm_id,VAR(TickRefType, AUTOMATIC) tick)
{
  return tpl_get_alarm_service(alarm_id, tick);
}

/*
 * SetRelAlarm
 *
 * SetRelAlarm starts alarm <alarm_id>. After <increment> ticks have 
 * elapsed, the task assigned to the alarm <alarm_id> is activated or the 
 * assigned event (only for extended tasks) is set or the alarm-callback 
 * routine is called. 
 * See page 63 of the OSEK OS 2.2.3 specification. 
 *
 * alarm_id: The identifier of the alarm 
 * increment: Relative value in ticks. 
 * cycle: Cycle value in case of a cyclic alarm. In case of a one shot 
 *         alarm, cycle shall be zero 
 *
 * Return value:
 * E_OK: No error (Standard & Extended) 
 * E_OS_NOFUNC: Alarm <alarm_id> is already in use (Standard & Extended) 
 * E_OS_ID: <alarm_id> is invalid (Extended) 
 * E_OS_VALUE: <increment> out of bounds (Extended) 
 * E_OS_VALUE: <cycle> out of bounds (Extended) 
 */
FUNC(StatusType, OS_CODE) SetRelAlarm(CONST(AlarmType, AUTOMATIC) alarm_id,CONST(TickType, AUTOMATIC) increment,CONST(TickType, AUTOMATIC) cycle)
{
  return tpl_set_rel_alarm_service(alarm_id, increment, cycle);
}

/*
 * SetAbsAlarm
 *
 * SetAbsAlarm starts alarm <alarm_id>. At <start> ticks, the task 
 * assigned to the alarm <alarm_id> is activated or the assigned event 
 * (only for extended tasks) is set or the alarm-callback routine is 
 * called. 
 * See page 64 of the OSEK OS 2.2.3 specification. 
 *
 * alarm_id: The identifier of the alarm 
 * start: Absolute value in ticks. 
 * cycle: Cycle value in case of a cyclic alarm. In case of a one shot 
 *         alarm, cycle shall be zero 
 *
 * Return value:
 * E_OK: No error (Standard & Extended) 
 * E_OS_NOFUNC: Alarm <alarm_id> is already in use (Standard & Extended) 
 * E_OS_ID: <alarm_id> is invalid (Extended) 
 * E_OS_VALUE: <start> out of bounds (Extended) 
 * E_OS_VALUE: <cycle> out of bounds (Extended) 
 */
FUNC(StatusType, OS_CODE) SetAbsAlarm(CONST(AlarmType, AUTOMATIC) alarm_id,CONST(TickType, AUTOMATIC) start,CONST(TickType, AUTOMATIC) cycle)
{
  return tpl_set_abs_alarm_service(alarm_id, start, cycle);
}

/*
 * CancelAlarm
 *
 * CancelAlarm cancels a started alarm. 
 * See page 65 of the OSEK OS 2.2.3 specification. 
 *
 * alarm_id: The identifier of the alarm 
 *
 * Return value:
 * E_OK: No error (Standard & Extended) 
 * E_OS_NOFUNC: Alarm <alarm_id> is not in use (Standard & Extended) 
 * E_OS_ID: <alarm_id> is invalid (Extended) 
 */
FUNC(StatusType, OS_CODE) CancelAlarm(CONST(AlarmType, AUTOMATIC) alarm_id)
{
  return tpl_cancel_alarm_service(alarm_id);
}


/* End of file tpl_os.c */
