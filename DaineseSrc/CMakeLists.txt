cmake_minimum_required(VERSION 3.11-<max>)

project(fileRead)

add_library(fileRead STATIC 
            src/analysis.cpp
            src/dataCLeaning.cpp
            src/fileRead.cpp
            src/FileTypesData.cpp
            src/plotting.cpp 
            src/timeData.cpp
            src/timeHelpers.cpp
            src/vectorHelpers.cpp
            src/stringHelpers.cpp
            src/lapTimes.cpp
            src/latLongHelpers.cpp
            src/config.cpp)
add_executable(laptime src/daineseMain.cpp)
target_link_libraries(laptime PRIVATE fileRead)

# PUBLIC needed to make both testMatPlot.h and ReadTest library available elsewhere in project.
target_include_directories(fileRead
    PUBLIC ${PROJECT_SOURCE_DIR}/include
    #PRIVATE C:\\users\\gwalk\\appdata\\local\\programs\\python\\python39\\include
    #PRIVATE C:\\users\\gwalk\\appdata\\local\\programs\\python\\python39\\lib\\site-packages\\numpy\\core\\include
    PRIVATE C:\\program\ files\\python310\\include
    PRIVATE C:\\program\ files\\python310\\lib\\site-packages\\numpy\\core\\include
)
target_include_directories(laptime
    PUBLIC ${PROJECT_SOURCE_DIR}/include
    #PRIVATE C:\\users\\gwalk\\appdata\\local\\programs\\python\\python39\\include
    #PRIVATE C:\\users\\gwalk\\appdata\\local\\programs\\python\\python39\\lib\\site-packages\\numpy\\core\\include
    PRIVATE C:\\program\ files\\python310\\include
    PRIVATE C:\\program\ files\\python310\\lib\\site-packages\\numpy\\core\\include
)

target_link_directories(fileRead PRIVATE C:\\program\ files\\python310\\libs)
target_link_directories(laptime PRIVATE C:\\program\ files\\python310\\libs)

add_custom_command(TARGET fileRead POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy
    "$<TARGET_FILE:fileRead>"
    "../tests/$<CONFIGURATION>/$<TARGET_FILE_NAME:fileRead>"
    COMMENT "Copying to output directory"
)

target_compile_features(fileRead PUBLIC cxx_std_20)
