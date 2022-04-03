add_executable(004_TaskPriorities main.cpp)
target_link_libraries(004_TaskPriorities FreeRTOS winmm)

#include headers used by FreeRTOS
target_include_directories(004_TaskPriorities
        PRIVATE $<TARGET_PROPERTY:FreeRTOS,INTERFACE_INCLUDE_DIRECTORIES>)
target_sources(FreeRTOS PRIVATE FreeRTOSHooks.c Run-time-stats-utils.c MemMang/heap_5.c)
target_include_directories(FreeRTOS INTERFACE Settings)
