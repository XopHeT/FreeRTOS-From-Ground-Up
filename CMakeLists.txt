cmake_minimum_required(VERSION 3.21)
project(FreeRTOSFromGroundUp)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(3rdParty)
add_subdirectory(001_TaskCreation)
add_subdirectory(003_TaskParameters)
add_subdirectory(004_TaskPriorities)
add_subdirectory(005_ChangingTaskPrioritiesInRealTime)
