SET(FreeRTOS_Sources
        ${CMAKE_CURRENT_SOURCE_DIR}/croutine.c
        ${CMAKE_CURRENT_SOURCE_DIR}/event_groups.c
        ${CMAKE_CURRENT_SOURCE_DIR}/list.c
        ${CMAKE_CURRENT_SOURCE_DIR}/queue.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stream_buffer.c
        ${CMAKE_CURRENT_SOURCE_DIR}/tasks.c
        ${CMAKE_CURRENT_SOURCE_DIR}/timers.c
        ${CMAKE_CURRENT_SOURCE_DIR}/portable/MSVC-MingW/port.c)
list(APPEND FreeRTOS_Headers ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}/portable/MSVC-MingW/)
SET(FreeRTOS_Sources ${FreeRTOS_Sources} PARENT_SCOPE)
SET(FreeRTOS_Headers ${FreeRTOS_Headers} PARENT_SCOPE)
