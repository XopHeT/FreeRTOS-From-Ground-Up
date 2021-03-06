#include <iostream>
#include "FreeRTOS.h"
#include "task.h"

struct TaskStack {
  static const BaseType_t stackDepth = configMINIMAL_STACK_SIZE;
  StackType_t stack[stackDepth];
  StaticTask_t task;
};

TaskStack BlueLed;
uint32_t IdleCounter;

[[noreturn]] void vLEDControllerTask(void *pvParameters);


int main() {
  xTaskCreateStatic(vLEDControllerTask, "Blue LED controller", BlueLed.stackDepth,
          (void *)"Blue", 2, BlueLed.stack, &BlueLed.task);

  vTaskStartScheduler();

  return 0;
}

[[noreturn]] void vLEDControllerTask(void *pvParameters) {
  TickType_t xLastWakeTime = xTaskGetTickCount();
  const TickType_t xTaskPeriod = pdMS_TO_TICKS(500);
  while (true) {
    vTaskDelayUntil(&xLastWakeTime, xTaskPeriod);
    printf("%s LED works. Idle cycles: %u\n", (const char *)pvParameters, IdleCounter);

  }
}

extern "C" void vApplicationIdleHook() {
  IdleCounter++;
}
