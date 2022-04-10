#include <iostream>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

struct TaskStack {
  static const BaseType_t stackDepth = configMINIMAL_STACK_SIZE;
  StackType_t stack[stackDepth];
  StaticTask_t task;
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

typedef uint16_t Year_t;
QueueHandle_t Message1Queue;
constexpr uint16_t QueueSize = 16;
uint8_t Message1QueueQueueItems[QueueSize * sizeof(Year_t)];
StaticQueue_t Message1QueueTheQueue;

typedef char Message_t[15];
QueueHandle_t Message2Queue;
uint8_t Message2QueueQueueItems[QueueSize * sizeof(Message_t)];
StaticQueue_t Message2QueueTheQueue;

QueueSetHandle_t ReceiverQueueHandle;

int main() {
  // Sender tasks should have lower priority
  Message1Queue = xQueueCreateStatic(QueueSize, sizeof(Year_t), Message1QueueQueueItems, &Message1QueueTheQueue);
  Message2Queue = xQueueCreateStatic(QueueSize, sizeof(Message_t), Message2QueueQueueItems, &Message2QueueTheQueue);

  ReceiverQueueHandle = xQueueCreateSet(2);
  xQueueAddToSet(Message1Queue, ReceiverQueueHandle);
  xQueueAddToSet(Message2Queue, ReceiverQueueHandle);

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
    qStatus = xQueueSend(Message1Queue, &year, 0);
    if (qStatus != pdPASS) {
      printf("Error. Year can not be sent\n");
    }
    vTaskDelay(pdMS_TO_TICKS(10));
  }
}

[[noreturn]] void vSenderTask2(void *pvParameters) {
  Message_t message = "Hello!";
  BaseType_t qStatus;
  while (true) {
    qStatus = xQueueSend(Message2Queue, &message, 0);
    if (qStatus != pdPASS) {
      printf("Error. Message can not be sent\n");
    }
    vTaskDelay(pdMS_TO_TICKS(20));
  }
}

[[noreturn]] void vReceiverTask(void *pvParameters) {
  uint16_t yearReceived;
  Message_t message;
  const TickType_t waitUpTo = pdMS_TO_TICKS(100);
  BaseType_t qStatus;
  while (true) {
    auto queue = xQueueSelectFromSet(ReceiverQueueHandle, waitUpTo);
    if (queue == Message1Queue) {
      qStatus = xQueueReceive(queue, &yearReceived, waitUpTo);
      if (qStatus == pdPASS) {
        printf("Year received: %d\n", yearReceived);
      } else {
        printf("Error: Can not receive year");
      }
    } else if (queue == Message2Queue) {
      qStatus = xQueueReceive(queue, &message, waitUpTo);
      if (qStatus == pdPASS) {
        printf("Message received: %s\n", message);
      } else {
        printf("Error: Can not receive message");
      }
    }
  }
}
