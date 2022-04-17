#include <iostream>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

TimerHandle_t continuousTimer;
StaticTimer_t ctTimer;
TimerHandle_t timerWillBeStopped;
StaticTimer_t twbsTimer;

void continuousTimerTask(TimerHandle_t xTimer);

void notVeryContinuousTimerTask(TimerHandle_t xTimer);


int main() {

  continuousTimer = xTimerCreateStatic("Continuous timer", pdMS_TO_TICKS(100), pdTRUE, NULL, &continuousTimerTask,
          &ctTimer);
  timerWillBeStopped = xTimerCreateStatic("Will be stopped timer", pdMS_TO_TICKS(200), pdTRUE, NULL,
          &notVeryContinuousTimerTask, &twbsTimer);

  xTimerStart(continuousTimer, 0);
  xTimerStart(timerWillBeStopped, 0);

  vTaskStartScheduler();

  return 0;
}

void continuousTimerTask(TimerHandle_t xTimer) {
  printf("Continuous timer working\n");
}

void notVeryContinuousTimerTask(TimerHandle_t xTimer) {
  static int wasCalled = 0;
  static const int maxCalls = 5;
  printf("I will be stopped in %d calls\n", maxCalls - wasCalled);
  wasCalled++;
  if (wasCalled >= maxCalls) {
    xTimerStop(xTimer, 0);
  }
}
