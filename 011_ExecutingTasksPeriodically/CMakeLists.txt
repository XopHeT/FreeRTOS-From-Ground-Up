add_executable(011_ExecutingTasksPeriodically main.cpp)
target_link_libraries(011_ExecutingTasksPeriodically FreeRTOS winmm)

#include headers used by FreeRTOS
target_include_directories(011_ExecutingTasksPeriodically
        PRIVATE $<TARGET_PROPERTY:FreeRTOS,INTERFACE_INCLUDE_DIRECTORIES>)
target_sources(FreeRTOS PRIVATE FreeRTOSHooks.c Run-time-stats-utils.c MemMang/heap_5.c)
target_include_directories(FreeRTOS INTERFACE Settings)
