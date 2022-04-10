#include "FreeRTOS.h"

void *pvPortMalloc(size_t size) {
  return malloc(size);
}

void vPortFree(void *what) {
  free(what);
}
