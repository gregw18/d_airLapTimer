cmake_minimum_required(VERSION 3.12)

project(laptime)

add_subdirectory(daineseSrc)
if(PROJECT_NAME STREQUAL CMAKE_PROJECT_NAME)
  add_subdirectory(tests)
  message("Added tests dir")
endif()

