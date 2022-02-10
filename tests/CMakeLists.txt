cmake_minimum_required(VERSION 3.12)

project(laptime)

include(GoogleTest)
include(CTest)
include(FetchContent)

enable_testing()

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.10.0
)

FetchContent_GetProperties(googletest)
if(NOT googletest_POPULATED)
  FetchContent_Populate(googletest)
  if(MSVC)
    add_compile_options(/MD$<$<CONFIG:Debug>:d>)
  endif()   # MSVC
  add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
endif()     # NOT googletest_POPULATED

set_target_properties(gtest PROPERTIES FOLDER extern)

set(CMAKE_CONFIGURATION_TYPES Release)

macro(package_add_test_with_libraries TESTNAME FILES LIBRARIES
    TEST_WORKING_DIRECTORY)
    add_executable(${TESTNAME} ${FILES})
    target_link_libraries(${TESTNAME} gtest gmock gtest_main ${LIBRARIES})
    gtest_discover_tests(${TESTNAME}
        WORKING_DIRECTORY ${TEST_WORKING_DIRECTORY}
        PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY
            "${TEST_WORKING_DIRECTORY}"
    )
    set_target_properties(${TESTNAME} PROPERTIES FOLDER tests)
endmacro()

set(TEST_SRCS
  AnalysisTests.cpp
  ConfigTests.cpp
  dataCleaningTests.cpp
  FileTypeInfoTests.cpp
  ReadTests.cpp
  TestLapDataGenerator.cpp
  TestLapDataGeneratorTests.cpp
  TimeDataTests.cpp
)
package_add_test_with_libraries(ReadTests "${TEST_SRCS}" fileRead "${PROJECT_DIR}")
#package_add_test_with_libraries(DataTests dataCleaningTests.cpp dataRead "${PROJECT_DIR}")

target_include_directories(ReadTests
    #PRIVATE C:\\users\\gwalk\\appdata\\local\\programs\\python\\python39\\include
    #PRIVATE C:\\users\\gwalk\\appdata\\local\\programs\\python\\python39\\lib\\site-packages\\numpy\\core\\include
    PRIVATE C:\\program\ files\\python310\\include
    PRIVATE C:\\program\ files\\python310\\lib\\site-packages\\numpy\\core\\include
)
target_link_directories(ReadTests PRIVATE C:\\program\ files\\python310\\libs)

target_compile_features(ReadTests PUBLIC cxx_std_20)
  
