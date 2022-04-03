add_subdirectory(FreeRTOS)
SET(ThirdPartySources ${FreeRTOS_Sources} PARENT_SCOPE)
SET(ThirdPartyHeaders ${FreeRTOS_Headers} PARENT_SCOPE)
