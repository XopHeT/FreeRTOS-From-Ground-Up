#include "FreeRTOS.h"
#include "task.h"

/* When configSUPPORT_STATIC_ALLOCATION is set to 1 the application writer can
use a callback function to optionally provide the memory required by the idle
and timer tasks.  This is the stack that will be used by the timer task.  It is
declared here, as a global, so it can be checked by a test that is implemented
in a different file. */
StackType_t uxTimerTaskStack[ configTIMER_TASK_STACK_DEPTH ];

/* Notes if the trace is running or not. */
static BaseType_t xTraceRunning = pdTRUE;

/* This demo uses heap_5.c, and these constants define the sizes of the regions
that make up the total heap.  heap_5 is only used for test and example purposes
as this demo could easily create one large heap region instead of multiple
smaller heap regions - in which case heap_4.c would be the more appropriate
choice.  See http://www.freertos.org/a00111.html for an explanation. */
#define mainREGION_1_SIZE  10801
#define mainREGION_2_SIZE  29905
#define mainREGION_3_SIZE  6007

/*
 * Prototypes for the standard FreeRTOS application hook (callback) functions
 * implemented within this file.  See http://www.freertos.org/a00016.html .
 */
void vApplicationMallocFailedHook(void);

void vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName);

void vApplicationTickHook(void);

void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer,
        uint32_t *pulIdleTaskStackSize);

void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer,
        uint32_t *pulTimerTaskStackSize);

/*
 * Writes trace data to a disk file when the trace recording is stopped.
 * This function will simply overwrite any trace files that already exist.
 */
static void prvSaveTraceFile(void);

/*
 * This demo uses heap_5.c, so start by defining some heap regions.  It is not
 * necessary for this demo to use heap_5, as it could define one large heap
 * region.  Heap_5 is only used for test and example purposes.  See
 * http://www.freertos.org/a00111.html for an explanation.
 */
static void prvInitialiseHeap(void);

/*
 * Performs a few sanity checks on the behaviour of the vPortGetHeapStats()
 * function.
 */
static void prvExerciseHeapStats(void);

void vApplicationMallocFailedHook(void) {
  /* vApplicationMallocFailedHook() will only be called if
  configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h.  It is a hook
  function that will get called if a call to pvPortMalloc() fails.
  pvPortMalloc() is called internally by the kernel whenever a task, queue,
  timer or semaphore is created.  It is also called by various parts of the
  demo application.  If heap_1.c, heap_2.c or heap_4.c is being used, then the
  size of the	heap available to pvPortMalloc() is defined by
  configTOTAL_HEAP_SIZE in FreeRTOSConfig.h, and the xPortGetFreeHeapSize()
  API function can be used to query the size of free heap space that remains
  (although it does not provide information on how the remaining heap might be
  fragmented).  See http://www.freertos.org/a00111.html for more
  information. */
  vAssertCalled(__LINE__, __FILE__);
}

/*-----------------------------------------------------------*/

void vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName) {
  (void)pcTaskName;
  (void)pxTask;

  /* Run time stack overflow checking is performed if
  configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
  function is called if a stack overflow is detected.  This function is
  provided as an example only as stack overflow checking does not function
  when running the FreeRTOS Windows port. */
  vAssertCalled(__LINE__, __FILE__);
}

/*-----------------------------------------------------------*/

void vApplicationTickHook(void) {
  /* This function will be called by each tick interrupt if
  configUSE_TICK_HOOK is set to 1 in FreeRTOSConfig.h.  User code can be
  added here, but the tick hook is called from an interrupt context, so
  code must not attempt to block, and only the interrupt safe FreeRTOS API
  functions can be used (those that end in FromISR()). */

#if (mainCREATE_SIMPLE_BLINKY_DEMO_ONLY != 1)
  {
  }
#endif /* mainCREATE_SIMPLE_BLINKY_DEMO_ONLY */
}

/*-----------------------------------------------------------*/

void vApplicationDaemonTaskStartupHook(void) {
  /* This function will be called once only, when the daemon task starts to
  execute	(sometimes called the timer task).  This is useful if the
  application includes initialisation code that would benefit from executing
  after the scheduler has been started. */
}

/*-----------------------------------------------------------*/

void vAssertCalled(unsigned long ulLine, const char *const pcFileName) {
  static BaseType_t xPrinted = pdFALSE;
  volatile uint32_t ulSetToNonZeroInDebuggerToContinue = 0;

  /* Called if an assertion passed to configASSERT() fails.  See
  http://www.freertos.org/a00110.html#configASSERT for more information. */

  /* Parameters are not used. */
  (void)ulLine;
  (void)pcFileName;


  taskENTER_CRITICAL();
  {
    /* Stop the trace recording. */
    if (xPrinted == pdFALSE) {
      xPrinted = pdTRUE;
      if (xTraceRunning == pdTRUE) {
        prvSaveTraceFile();
      }
    }

    /* You can step out of this function to debug the assertion by using
    the debugger to set ulSetToNonZeroInDebuggerToContinue to a non-zero
    value. */
    while (ulSetToNonZeroInDebuggerToContinue == 0) {
      __asm volatile( "NOP" );
      __asm volatile( "NOP" );
    }
  }
  taskEXIT_CRITICAL();
}

/*-----------------------------------------------------------*/

static void prvSaveTraceFile(void) {
}


static void prvInitialiseHeap(void) {
  /* The Windows demo could create one large heap region, in which case it would
  be appropriate to use heap_4.  However, purely for demonstration purposes,
  heap_5 is used instead, so start by defining some heap regions.  No
  initialisation is required when any other heap implementation is used.  See
  http://www.freertos.org/a00111.html for more information.

  The xHeapRegions structure requires the regions to be defined in start address
  order, so this just creates one big array, then populates the structure with
  offsets into the array - with gaps in between and messy alignment just for test
  purposes. */
  static uint8_t ucHeap[configTOTAL_HEAP_SIZE];
  volatile uint32_t ulAdditionalOffset = 19; /* Just to prevent 'condition is always true' warnings in configASSERT(). */
  HeapStats_t xHeapStats;
  const HeapRegion_t xHeapRegions[] = {
          /* Start address with dummy offsets						Size */
          {ucHeap + 1,                                          mainREGION_1_SIZE},
          {ucHeap + 15 + mainREGION_1_SIZE,                     mainREGION_2_SIZE},
          {ucHeap + 19 + mainREGION_1_SIZE + mainREGION_2_SIZE, mainREGION_3_SIZE},
          {NULL, 0}};

  /* Sanity check that the sizes and offsets defined actually fit into the
  array. */
  configASSERT(
          (ulAdditionalOffset + mainREGION_1_SIZE + mainREGION_2_SIZE + mainREGION_3_SIZE) < configTOTAL_HEAP_SIZE);

  /* Prevent compiler warnings when configASSERT() is not defined. */
  (void)ulAdditionalOffset;

  /* The heap has not been initialised yet so expect stats to all be zero. */
  vPortGetHeapStats(&xHeapStats);

  vPortDefineHeapRegions(xHeapRegions);

  /* Sanity check vTaskGetHeapStats(). */
  prvExerciseHeapStats();
}

/*-----------------------------------------------------------*/

static void prvExerciseHeapStats(void) {
  HeapStats_t xHeapStats;
  size_t xInitialFreeSpace = xPortGetFreeHeapSize(), xMinimumFreeBytes;
  size_t xMetaDataOverhead, i;
  void *pvAllocatedBlock;
  const size_t xArraySize = 5, xBlockSize = 1000UL;
  void *pvAllocatedBlocks[xArraySize];

  /* Check heap stats are as expected after initialisation but before any
  allocations. */
  vPortGetHeapStats(&xHeapStats);

  /* Minimum ever free bytes remaining should be the same as the total number
  of bytes as nothing has been allocated yet. */
  configASSERT(xHeapStats.xMinimumEverFreeBytesRemaining == xHeapStats.xAvailableHeapSpaceInBytes);
  configASSERT(xHeapStats.xMinimumEverFreeBytesRemaining == xInitialFreeSpace);

  /* Nothing has been allocated or freed yet. */
  configASSERT(xHeapStats.xNumberOfSuccessfulAllocations == 0);
  configASSERT(xHeapStats.xNumberOfSuccessfulFrees == 0);

  /* Allocate a 1000 byte block then measure what the overhead of the
  allocation in regards to how many bytes more than 1000 were actually
  removed from the heap in order to store metadata about the allocation. */
  pvAllocatedBlock = pvPortMalloc(xBlockSize);
  configASSERT(pvAllocatedBlock);
  xMetaDataOverhead = (xInitialFreeSpace - xPortGetFreeHeapSize()) - xBlockSize;

  /* Free the block again to get back to where we started. */
  vPortFree(pvAllocatedBlock);
  vPortGetHeapStats(&xHeapStats);
  configASSERT(xHeapStats.xAvailableHeapSpaceInBytes == xInitialFreeSpace);
  configASSERT(xHeapStats.xNumberOfSuccessfulAllocations == 1);
  configASSERT(xHeapStats.xNumberOfSuccessfulFrees == 1);

  /* Allocate blocks checking some stats value on each allocation. */
  for (i = 0; i < xArraySize; i++) {
    pvAllocatedBlocks[i] = pvPortMalloc(xBlockSize);
    configASSERT(pvAllocatedBlocks[i]);
    vPortGetHeapStats(&xHeapStats);
    configASSERT(xHeapStats.xMinimumEverFreeBytesRemaining ==
                 (xInitialFreeSpace - ((i + 1) * (xBlockSize + xMetaDataOverhead))));
    configASSERT(xHeapStats.xMinimumEverFreeBytesRemaining == xHeapStats.xAvailableHeapSpaceInBytes);
    configASSERT(xHeapStats.xNumberOfSuccessfulAllocations == (2Ul + i));
    configASSERT(xHeapStats.xNumberOfSuccessfulFrees == 1); /* Does not increase during allocations. */
  }

  configASSERT(xPortGetFreeHeapSize() == xPortGetMinimumEverFreeHeapSize());
  xMinimumFreeBytes = xPortGetFreeHeapSize();

  /* Free the blocks again. */
  for (i = 0; i < xArraySize; i++) {
    vPortFree(pvAllocatedBlocks[i]);
    vPortGetHeapStats(&xHeapStats);
    configASSERT(xHeapStats.xAvailableHeapSpaceInBytes ==
                 (xInitialFreeSpace - (((xArraySize - i - 1) * (xBlockSize + xMetaDataOverhead)))));
    configASSERT(xHeapStats.xNumberOfSuccessfulAllocations == (xArraySize + 1)); /* Does not increase during frees. */
    configASSERT(xHeapStats.xNumberOfSuccessfulFrees == (2UL + i));
  }

  /* The minimum ever free heap size should not change as blocks are freed. */
  configASSERT(xMinimumFreeBytes == xPortGetMinimumEverFreeHeapSize());
}
/*-----------------------------------------------------------*/

/* configUSE_STATIC_ALLOCATION is set to 1, so the application must provide an
implementation of vApplicationGetIdleTaskMemory() to provide the memory that is
used by the Idle task. */
void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer,
        uint32_t *pulIdleTaskStackSize) {
  /* If the buffers to be provided to the Idle task are declared inside this
  function then they must be declared static - otherwise they will be allocated on
  the stack and so not exists after this function exits. */
  static StaticTask_t xIdleTaskTCB;
  static StackType_t uxIdleTaskStack[configMINIMAL_STACK_SIZE];

  /* Pass out a pointer to the StaticTask_t structure in which the Idle task's
  state will be stored. */
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;

  /* Pass out the array that will be used as the Idle task's stack. */
  *ppxIdleTaskStackBuffer = uxIdleTaskStack;

  /* Pass out the size of the array pointed to by *ppxIdleTaskStackBuffer.
  Note that, as the array is necessarily of type StackType_t,
  configMINIMAL_STACK_SIZE is specified in words, not bytes. */
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}
/*-----------------------------------------------------------*/

/* configUSE_STATIC_ALLOCATION and configUSE_TIMERS are both set to 1, so the
application must provide an implementation of vApplicationGetTimerTaskMemory()
to provide the memory that is used by the Timer service task. */
void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer,
        uint32_t *pulTimerTaskStackSize) {
  /* If the buffers to be provided to the Timer task are declared inside this
  function then they must be declared static - otherwise they will be allocated on
  the stack and so not exists after this function exits. */
  static StaticTask_t xTimerTaskTCB;

  /* Pass out a pointer to the StaticTask_t structure in which the Timer
  task's state will be stored. */
  *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;

  /* Pass out the array that will be used as the Timer task's stack. */
  *ppxTimerTaskStackBuffer = uxTimerTaskStack;

  /* Pass out the size of the array pointed to by *ppxTimerTaskStackBuffer.
  Note that, as the array is necessarily of type StackType_t,
  configMINIMAL_STACK_SIZE is specified in words, not bytes. */
  *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}
