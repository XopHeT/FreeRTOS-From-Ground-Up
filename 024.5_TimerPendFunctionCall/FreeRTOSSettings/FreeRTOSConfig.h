#ifndef INC_3D8ED98C4FFD440E8297E9DA4BC31766
#define INC_3D8ED98C4FFD440E8297E9DA4BC31766

/*-----------------------------------------------------------
 * Application specific definitions.
 *
 * These definitions should be adjusted for your particular hardware and
 * application requirements.
 *
 * THESE PARAMETERS ARE DESCRIBED WITHIN THE 'CONFIGURATION' SECTION OF THE
 * FreeRTOS API DOCUMENTATION AVAILABLE ON THE FreeRTOS.org WEB SITE.  See
 * http://www.freertos.org/a00110.html
 *----------------------------------------------------------*/
//@formatter:off

#define configUSE_PREEMPTION                      1
#define configUSE_PORT_OPTIMISED_TASK_SELECTION   1
#define configUSE_IDLE_HOOK                       0
#define configUSE_TICK_HOOK                       0
#define configUSE_DAEMON_TASK_STARTUP_HOOK        0
#define configTICK_RATE_HZ                        (1000) /* In this non-real time simulated environment the tick frequency has to be at least a multiple of the Win32 tick frequency, and therefore very slow. */
#define configMINIMAL_STACK_SIZE                  ((unsigned short)70) /* In this simulated case, the stack only has to hold one small structure as the real stack is part of the win32 thread. */
#define configMAX_TASK_NAME_LEN                   (12)
#define configUSE_TRACE_FACILITY                  1
#define configUSE_16_BIT_TICKS                    0
#define configIDLE_SHOULD_YIELD                   1
#define configUSE_MUTEXES                         1
#define configCHECK_FOR_STACK_OVERFLOW            0
#define configUSE_RECURSIVE_MUTEXES               1
#define configQUEUE_REGISTRY_SIZE                 20
#define configUSE_APPLICATION_TASK_TAG            1
#define configUSE_COUNTING_SEMAPHORES             1
#define configUSE_ALTERNATIVE_API                 0
#define configUSE_QUEUE_SETS                      1
#define configUSE_TASK_NOTIFICATIONS              1
#define configSUPPORT_STATIC_ALLOCATION           1
#define configSUPPORT_DYNAMIC_ALLOCATION          0
/* Software timer related configuration options.  The maximum possible task
priority is configMAX_PRIORITIES - 1.  The priority of the timer task is
deliberately set higher to ensure it is correctly capped back to
configMAX_PRIORITIES - 1. */
#define configUSE_TIMERS                          1
#define configTIMER_TASK_PRIORITY                 (configMAX_PRIORITIES - 1)
#define configTIMER_QUEUE_LENGTH                  20
#define configTIMER_TASK_STACK_DEPTH              (configMINIMAL_STACK_SIZE * 2)

#define configMAX_PRIORITIES                      (7)

/* Co-routine related configuration options. */
#define configUSE_CO_ROUTINES          0
#define configMAX_CO_ROUTINE_PRIORITIES           (2)

/* Enables the test whereby a stack larger than the total heap size is
requested. */
#define configSTACK_DEPTH_TYPE                    uint32_t

/* Set the following definitions to 1 to include the API function, or zero
to exclude the API function.  In most cases the linker will remove unused
functions anyway. */
#define INCLUDE_vTaskPrioritySet                  0
#define INCLUDE_uxTaskPriorityGet                 0
#define INCLUDE_vTaskDelete                       0
#define INCLUDE_vTaskCleanUpResources             0
#define INCLUDE_vTaskSuspend                      0
#define INCLUDE_vTaskDelayUntil                   0
#define INCLUDE_vTaskDelay                        1
#define INCLUDE_uxTaskGetStackHighWaterMark       0
#define INCLUDE_uxTaskGetStackHighWaterMark2      0
#define INCLUDE_xTaskGetSchedulerState            0
#define INCLUDE_xTimerGetTimerDaemonTaskHandle    0
#define INCLUDE_xTaskGetIdleTaskHandle            0
#define INCLUDE_xTaskGetHandle                    0
#define INCLUDE_eTaskGetState                     0
#define INCLUDE_xSemaphoreGetMutexHolder          0
#define INCLUDE_xTimerPendFunctionCall            1
#define INCLUDE_xTaskAbortDelay                   0


extern void vAssertCalled(unsigned long ulLine, const char *const pcFileName);

/* It is a good idea to define configASSERT() while developing.  configASSERT()
uses the same semantics as the standard C assert() macro.  Don't define
configASSERT() when performing code coverage tests though, as it is not
intended to asserts() to fail, some some code is intended not to run if no
errors are present. */
#define configASSERT(x) if((x) == 0) vAssertCalled( __LINE__, __FILE__ )

#define configUSE_MALLOC_FAILED_HOOK      0
//@formatter:on

#endif //INC_3D8ED98C4FFD440E8297E9DA4BC31766
