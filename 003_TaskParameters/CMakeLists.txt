#add_subdirectory(../3rdParty 3rdParty)

add_library(FreeRTOS_003_TaskParameters ALIAS FreeRTOS)

add_executable(003_TaskParameters main.cpp)
set_target_properties(FreeRTOS PROPERTIES OUTPUT_NAME "FreeRTOS_003_TaskParameters")
target_link_libraries(003_TaskParameters FreeRTOS_003_TaskParameters winmm)

#include headers used by FreeRTOS
target_include_directories(003_TaskParameters
        PRIVATE $<TARGET_PROPERTY:FreeRTOS,INTERFACE_INCLUDE_DIRECTORIES>)
target_sources(FreeRTOS PRIVATE FreeRTOSHooks.c Run-time-stats-utils.c MemMang/heap_5.c)
target_include_directories(FreeRTOS INTERFACE Settings)
