#include <iostream>
#include "FreeRTOS.h"
#include "task.h"

struct TaskStack {
  static const BaseType_t stackDepth = configMINIMAL_STACK_SIZE;
  StackType_t stack[stackDepth];
  StaticTask_t task;
};

[[noreturn]] void vBlueLEDControllerTask(void *pvParameters);
[[noreturn]] void vRedLEDControllerTask(void *pvParameters);
[[noreturn]] void vGreenLEDControllerTask(void *pvParameters);

TaskStack BlueLed;
TaskStack RedLed;
TaskStack GreenLed;

int main() {
  xTaskCreateStatic(vBlueLEDControllerTask, "Blue LED controller", BlueLed.stackDepth,
          nullptr, 1, BlueLed.stack, &BlueLed.task);
  xTaskCreateStatic(vRedLEDControllerTask, "Red LED controller", RedLed.stackDepth,
          nullptr, 1, RedLed.stack, &RedLed.task);
  xTaskCreateStatic(vGreenLEDControllerTask, "Green LED controller", GreenLed.stackDepth,
          nullptr, 1, GreenLed.stack, &GreenLed.task);

  vTaskStartScheduler();

  return 0;
}

[[noreturn]] void vBlueLEDControllerTask(void *pvParameters) {
  while (true) {
    printf("Blue LED works\n");
    vTaskDelay(10);
  }
}

[[noreturn]] void vRedLEDControllerTask(void *pvParameters) {
  while (true) {
    printf("Red LED works\n");
    vTaskDelay(10);
  }
}

[[noreturn]] void vGreenLEDControllerTask(void *pvParameters) {
  while (true) {
    printf("Green LED works\n");
    vTaskDelay(10);
  }
}
