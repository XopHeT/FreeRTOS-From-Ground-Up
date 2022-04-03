set(FreeRTOSSettingsSources
        ${CMAKE_CURRENT_SOURCE_DIR}/FreeRTOSHooks.c
        ${CMAKE_CURRENT_SOURCE_DIR}/Run-time-stats-utils.c
        ${CMAKE_CURRENT_SOURCE_DIR}/heap_5.c
        PARENT_SCOPE)

set(FreeRTOSSettings
        ${CMAKE_CURRENT_SOURCE_DIR}
        PARENT_SCOPE)
