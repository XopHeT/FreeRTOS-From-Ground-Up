add_subdirectory(../3rdParty 3rdParty)

add_executable(001_TaskCreation main.cpp)
target_link_libraries(001_TaskCreation FreeRTOS)

#include headers used by FreeRTOS
target_include_directories(001_TaskCreation
        PRIVATE $<TARGET_PROPERTY:FreeRTOS,INTERFACE_INCLUDE_DIRECTORIES>)
target_sources(FreeRTOS PRIVATE FreeRTOSHooks.c Run-time-stats-utils.c MemMang/heap_5.c)
target_include_directories(FreeRTOS PUBLIC Settings)
target_link_libraries(FreeRTOS winmm)
