add_executable(005_ChangingTaskPrioritiesInRealTime main.cpp)
target_link_libraries(005_ChangingTaskPrioritiesInRealTime FreeRTOS winmm)

#include headers used by FreeRTOS
target_include_directories(005_ChangingTaskPrioritiesInRealTime
        PRIVATE $<TARGET_PROPERTY:FreeRTOS,INTERFACE_INCLUDE_DIRECTORIES>)
target_sources(FreeRTOS PRIVATE FreeRTOSHooks.c Run-time-stats-utils.c MemMang/heap_5.c)
target_include_directories(FreeRTOS INTERFACE Settings)
