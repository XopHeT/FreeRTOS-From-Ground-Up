#include <iostream>
#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"

static const uint8_t task1Event = 1U << 0;
static const uint8_t task2Event = 1U << 1;
static const uint8_t task3Event = 1U << 2;
static const uint8_t allEvents = task1Event | task2Event | task3Event;
static const uint8_t tasksCount = 3;
static const uint8_t tasksStackSize = 100;

StaticEventGroup_t synchronizeTasks;
EventGroupHandle_t SyncTasksHandle;

[[noreturn]] void task1(void *pvParameters);

[[noreturn]] void task2(void *pvParameters);

[[noreturn]] void task3(void *pvParameters);

StaticTask_t tasks[tasksCount];
StackType_t stacks[tasksCount][tasksStackSize];
TaskHandle_t handles[tasksCount];

int main() {
  SyncTasksHandle = xEventGroupCreateStatic(&synchronizeTasks);

  handles[0] = xTaskCreateStatic(&task1, "Task 1", tasksStackSize, nullptr, tskIDLE_PRIORITY + 1, stacks[0], &tasks[0]);
  handles[1] = xTaskCreateStatic(&task2, "Task 2", tasksStackSize, nullptr, tskIDLE_PRIORITY + 1, stacks[1], &tasks[1]);
  handles[2] = xTaskCreateStatic(&task3, "Task 3", tasksStackSize, nullptr, tskIDLE_PRIORITY + 1, stacks[2], &tasks[2]);


  vTaskStartScheduler();

  return 0;
}


[[noreturn]] void task1(void *pvParameters) {
  while (true) {
    EventBits_t events = xEventGroupSync(SyncTasksHandle, task1Event, allEvents, portMAX_DELAY);
    if (events == allEvents) {
      printf("Task1: all events set\n");
    }
  }
}

[[noreturn]] void task2(void *pvParameters) {
  while (true) {
    EventBits_t events = xEventGroupSync(SyncTasksHandle, task2Event, allEvents, portMAX_DELAY);
    if (events == allEvents) {
      printf("Task2: all events set\n");
    }
  }
}

[[noreturn]] void task3(void *pvParameters) {
  while (true) {
    EventBits_t events = xEventGroupSync(SyncTasksHandle, task3Event, allEvents, portMAX_DELAY);
    if (events == allEvents) {
      printf("Task3: all events set\n");
    }
  }
}
