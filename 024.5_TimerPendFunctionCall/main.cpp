#include <iostream>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

void delayedFunction(void *pvParameter1, uint32_t ulDelayMs);

void Task(void *pvParameters);

static constexpr const uint16_t stack_size = 100;

StackType_t stackBuffer[stack_size];
StaticTask_t theTask;

int main() {

  xTaskCreateStatic(&Task, "ATask", stack_size, nullptr, tskIDLE_PRIORITY + 1, stackBuffer, &theTask);

  vTaskStartScheduler();

  return 0;
}

void Task(void *pvParameters) {
  for (uint32_t callId = 0; callId < configTIMER_QUEUE_LENGTH + 20; ++callId) {
    uint32_t delay = pdMS_TO_TICKS((callId + 1) * 1000);
    BaseType_t succeed = xTimerPendFunctionCall(&delayedFunction, NULL, delay, 100);
    if (succeed == pdPASS) {
      printf("Pended function call %d with delay %d ms\n", callId, delay);
    } else {
      printf("ERROR: can not pend function call %d with delay %d ms\n", callId, delay);

    }
  }
}

void delayedFunction(void *pvParameter1, uint32_t ulDelayMs) {
  printf("Delayed function with delay %d ms called\n", ulDelayMs);
  vTaskDelay(1000);
}

