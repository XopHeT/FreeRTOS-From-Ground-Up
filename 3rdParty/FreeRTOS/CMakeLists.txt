add_library(FreeRTOS croutine.c event_groups.c list.c queue.c stream_buffer.c tasks.c timers.c portable/MSVC-MingW/port.c)
target_include_directories(FreeRTOS PUBLIC portable/MSVC-MingW include)
# To use the library
# 1: target_include_directories(FreeRTOS PUBLIC DIRECTORY with FreeRTOSConfig.h)
# 2: target_sources(FreeRTOS PRIVATE MISSING_C_FILES_LIST)
# 3: target_include_directories(YOUR_TARGET_NAME
#        PRIVATE $<TARGET_PROPERTY:FreeRTOS,INTERFACE_INCLUDE_DIRECTORIES>) # add headers used by the library to the target
