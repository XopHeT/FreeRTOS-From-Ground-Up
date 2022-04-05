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

[[noreturn]] void vSenderTask1(void *pvParameters);
[[noreturn]] void vSenderTask2(void *pvParameters);

[[noreturn]] void vReceiverTask(void *pvParameters);

TaskHandle_t SenderHandle;
TaskStack SenderStack;

TaskHandle_t SenderHandle2;
TaskStack SenderStack2;

TaskHandle_t ReceiverHandle;
TaskStack ReceiverStack;

QueueHandle_t YearQueueHandle;
YearQueueData Data;

int main() {
  // Sender tasks should have lower priority
  YearQueueHandle = xQueueCreateStatic(Data.QueueSize, sizeof(Year_t), Data.QueueItems, &Data.TheQueue);
  SenderHandle = xTaskCreateStatic(vSenderTask1, "Sender task 1", SenderStack.stackDepth, nullptr, 1, SenderStack.stack,
          &SenderStack.task);
  SenderHandle2 = xTaskCreateStatic(vSenderTask2, "Sender task 2", SenderStack2.stackDepth, nullptr, 1, SenderStack2.stack,
          &SenderStack2.task);
  ReceiverHandle = xTaskCreateStatic(vReceiverTask, "Receiver task", ReceiverStack.stackDepth, nullptr, 2,
          ReceiverStack.stack, &ReceiverStack.task);

  vTaskStartScheduler();

  return 0;
}

[[noreturn]] void vSenderTask1(void *pvParameters) {
  uint16_t year = 2050;
  BaseType_t qStatus;
  while (true) {
    qStatus = xQueueSend(YearQueueHandle, &year, 0);
    if (qStatus != pdPASS) {
      printf("Error. Data can not be sent\n");
    }
    vTaskDelay(pdMS_TO_TICKS(1));
  }
}

[[noreturn]] void vSenderTask2(void *pvParameters) {
  uint16_t year = 3050;
  BaseType_t qStatus;
  while (true) {
    qStatus = xQueueSend(YearQueueHandle, &year, 0);
    if (qStatus != pdPASS) {
      printf("Error. Data can not be sent\n");
    }
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
