#include <iostream>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

struct TaskStack {
  static const BaseType_t stackDepth = configMINIMAL_STACK_SIZE;
  StackType_t stack[stackDepth];
  StaticTask_t task;
};

typedef uint16_t Year_t;
struct YearQueueData {
  static const uint16_t QueueSize = 16;
  uint8_t QueueItems[QueueSize * sizeof(Year_t)];
  StaticQueue_t TheQueue;
};

[[noreturn]] void vSenderTask(void *pvParameters);

[[noreturn]] void vReceiverTask(void *pvParameters);

TaskHandle_t SenderHandle;
TaskStack SenderStack;

TaskHandle_t ReceiverHandle;
TaskStack ReceiverStack;

QueueHandle_t YearQueueHandle;
YearQueueData Data;

int main() {
  YearQueueHandle = xQueueCreateStatic(Data.QueueSize, sizeof(Year_t), Data.QueueItems, &Data.TheQueue);
  SenderHandle = xTaskCreateStatic(vSenderTask, "Sender task", SenderStack.stackDepth, nullptr, 2, SenderStack.stack,
          &SenderStack.task);
  ReceiverHandle = xTaskCreateStatic(vReceiverTask, "Receiver task", ReceiverStack.stackDepth, nullptr, 1,
          ReceiverStack.stack, &ReceiverStack.task);

  vTaskStartScheduler();

  return 0;
}

[[noreturn]] void vSenderTask(void *pvParameters) {
  uint16_t year = 2050;
  BaseType_t qStatus;
  while (true) {
    qStatus = xQueueSend(YearQueueHandle, &year, 0);
    if (qStatus != pdPASS) {
      printf("Error. Data can not be sent\n");
    }
    year++;
    vTaskDelay(pdMS_TO_TICKS(1));
  }
}

[[noreturn]] void vReceiverTask(void *pvParameters) {
  uint16_t yearReceived;
  const TickType_t waitUpTo = pdMS_TO_TICKS(100);
  BaseType_t qStatus;
  while (true) {
    qStatus = xQueueReceive(YearQueueHandle, &yearReceived, waitUpTo);
    if (qStatus == pdPASS) {
      printf("Year received: %d\n", yearReceived);
    } else {
      printf("Error: Can not receive year");
    }
  }
}
