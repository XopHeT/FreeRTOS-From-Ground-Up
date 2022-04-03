set(TargetName 001_TaskCreation)
add_subdirectory(../3rdParty 3rdParty)
add_subdirectory(FreeRTOSSettings)

add_executable(${TargetName} main.cpp
        ${ThirdPartySources} ${FreeRTOSSettingsSources})
target_include_directories(${TargetName} PRIVATE ${ThirdPartyHeaders} ${FreeRTOSSettings})
target_include_directories(${TargetName} PRIVATE Settings)
target_link_libraries(${TargetName} winmm)
