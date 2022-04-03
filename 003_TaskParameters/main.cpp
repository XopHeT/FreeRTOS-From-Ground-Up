#include <iostream>
#include "FreeRTOS.h"
#include "task.h"

#define mainREGION_1_SIZE	10801
#define mainREGION_2_SIZE	29905
#define mainREGION_3_SIZE	6007

static void prvExerciseHeapStats( void )
{
  HeapStats_t xHeapStats;
  size_t xInitialFreeSpace = xPortGetFreeHeapSize(), xMinimumFreeBytes;
  size_t xMetaDataOverhead, i;
  void *pvAllocatedBlock;
  const size_t xArraySize = 5, xBlockSize = 1000UL;
  void *pvAllocatedBlocks[ xArraySize ];

  /* Check heap stats are as expected after initialisation but before any
  allocations. */
  vPortGetHeapStats( &xHeapStats );

  /* Minimum ever free bytes remaining should be the same as the total number
  of bytes as nothing has been allocated yet. */
  configASSERT( xHeapStats.xMinimumEverFreeBytesRemaining == xHeapStats.xAvailableHeapSpaceInBytes );
  configASSERT( xHeapStats.xMinimumEverFreeBytesRemaining == xInitialFreeSpace );

  /* Nothing has been allocated or freed yet. */
  configASSERT( xHeapStats.xNumberOfSuccessfulAllocations == 0 );
  configASSERT( xHeapStats.xNumberOfSuccessfulFrees == 0 );

  /* Allocate a 1000 byte block then measure what the overhead of the
  allocation in regards to how many bytes more than 1000 were actually
  removed from the heap in order to store metadata about the allocation. */
  pvAllocatedBlock = pvPortMalloc( xBlockSize );
  configASSERT( pvAllocatedBlock );
  xMetaDataOverhead = ( xInitialFreeSpace - xPortGetFreeHeapSize() ) - xBlockSize;

  /* Free the block again to get back to where we started. */
  vPortFree( pvAllocatedBlock );
  vPortGetHeapStats( &xHeapStats );
  configASSERT( xHeapStats.xAvailableHeapSpaceInBytes == xInitialFreeSpace );
  configASSERT( xHeapStats.xNumberOfSuccessfulAllocations == 1 );
  configASSERT( xHeapStats.xNumberOfSuccessfulFrees == 1 );

  /* Allocate blocks checking some stats value on each allocation. */
  for( i = 0; i < xArraySize; i++ )
  {
    pvAllocatedBlocks[ i ] = pvPortMalloc( xBlockSize );
    configASSERT( pvAllocatedBlocks[ i ] );
    vPortGetHeapStats( &xHeapStats );
    configASSERT( xHeapStats.xMinimumEverFreeBytesRemaining == ( xInitialFreeSpace - ( ( i + 1 ) * ( xBlockSize + xMetaDataOverhead ) ) ) );
    configASSERT( xHeapStats.xMinimumEverFreeBytesRemaining == xHeapStats.xAvailableHeapSpaceInBytes );
    configASSERT( xHeapStats.xNumberOfSuccessfulAllocations == ( 2Ul + i ) );
    configASSERT( xHeapStats.xNumberOfSuccessfulFrees == 1 ); /* Does not increase during allocations. */
  }

  configASSERT( xPortGetFreeHeapSize() == xPortGetMinimumEverFreeHeapSize() );
  xMinimumFreeBytes = xPortGetFreeHeapSize();

  /* Free the blocks again. */
  for( i = 0; i < xArraySize; i++ )
  {
    vPortFree( pvAllocatedBlocks[ i ] );
    vPortGetHeapStats( &xHeapStats );
    configASSERT( xHeapStats.xAvailableHeapSpaceInBytes == ( xInitialFreeSpace - ( ( ( xArraySize - i - 1 ) * ( xBlockSize + xMetaDataOverhead ) ) ) ) );
    configASSERT( xHeapStats.xNumberOfSuccessfulAllocations == ( xArraySize + 1 ) ); /* Does not increase during frees. */
    configASSERT( xHeapStats.xNumberOfSuccessfulFrees == ( 2UL + i ) );
  }

  /* The minimum ever free heap size should not change as blocks are freed. */
  configASSERT( xMinimumFreeBytes == xPortGetMinimumEverFreeHeapSize() );
}

static void  prvInitialiseHeap( void )
{
  /* The Windows demo could create one large heap region, in which case it would
  be appropriate to use heap_4.  However, purely for demonstration purposes,
  heap_5 is used instead, so start by defining some heap regions.  No
  initialisation is required when any other heap implementation is used.  See
  http://www.freertos.org/a00111.html for more information.

  The xHeapRegions structure requires the regions to be defined in start address
  order, so this just creates one big array, then populates the structure with
  offsets into the array - with gaps in between and messy alignment just for test
  purposes. */
  static uint8_t ucHeap[ configTOTAL_HEAP_SIZE ];
  volatile uint32_t ulAdditionalOffset = 19; /* Just to prevent 'condition is always true' warnings in configASSERT(). */
  HeapStats_t xHeapStats;
  const HeapRegion_t xHeapRegions[] =
          {
                  /* Start address with dummy offsets						Size */
                  { ucHeap + 1,											mainREGION_1_SIZE },
                  { ucHeap + 15 + mainREGION_1_SIZE,						mainREGION_2_SIZE },
                  { ucHeap + 19 + mainREGION_1_SIZE + mainREGION_2_SIZE,	mainREGION_3_SIZE },
                  { NULL, 0 }
          };

  /* Sanity check that the sizes and offsets defined actually fit into the
  array. */
  configASSERT( ( ulAdditionalOffset + mainREGION_1_SIZE + mainREGION_2_SIZE + mainREGION_3_SIZE ) < configTOTAL_HEAP_SIZE );

  /* Prevent compiler warnings when configASSERT() is not defined. */
  ( void ) ulAdditionalOffset;

  /* The heap has not been initialised yet so expect stats to all be zero. */
  vPortGetHeapStats( &xHeapStats );

  vPortDefineHeapRegions( xHeapRegions );

  /* Sanity check vTaskGetHeapStats(). */
  prvExerciseHeapStats();
}

[[noreturn]] void vLEDControllerTask(void *pvParameters);


int main() {
  prvInitialiseHeap();
  xTaskCreate(vLEDControllerTask, "Blue LED controller", 100, // stack size
          (void *)"B: Blue", 1, nullptr);
  xTaskCreate(vLEDControllerTask, "Red LED controller", 100, // stack size
          (void *)"R: Red", 1, nullptr);
  xTaskCreate(vLEDControllerTask, "Green LED controller", 100, // stack size
          (void *)"G: Green", 1, nullptr);

  vTaskStartScheduler();

  return 0;
}

[[noreturn]] void vLEDControllerTask(void *pvParameters) {
  while (true) {
    printf("%s LED works\n", (const char *)pvParameters);
    vTaskDelay(10);
  }
}
