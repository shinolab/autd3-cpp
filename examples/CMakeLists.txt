cmake_minimum_required(VERSION 3.21)

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
  message(FATAL_ERROR "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there. You may need to remove CMakeCache.txt. ")
endif()

project(autd3-examples)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(AUTD_LOCAL_TEST "For local test build" OFF)

if(${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.24.0") 
  cmake_policy(SET CMP0135 NEW)
endif()

include(FetchContent)
if(AUTD_LOCAL_TEST)
  FetchContent_Declare(
    autd3
    SOURCE_DIR ${PROJECT_SOURCE_DIR}/..
  )
elseif(WIN32)
  if(${CMAKE_SYSTEM_PROCESSOR} MATCHES "AMD64|IA64")
    FetchContent_Declare(
      autd3
      URL https://github.com/shinolab/autd3-cpp/releases/download/v35.0.1/autd3-v35.0.1-win-x64.zip
    )
  elseif(${CMAKE_SYSTEM_PROCESSOR} MATCHES "ARM64")
    FetchContent_Declare(
      autd3
      URL https://github.com/shinolab/autd3-cpp/releases/download/v35.0.1/autd3-v35.0.1-win-arm.zip
    )
  endif()
elseif(APPLE)
  FetchContent_Declare(
    autd3
    URL https://github.com/shinolab/autd3-cpp/releases/download/v35.0.1/autd3-v35.0.1-macos-aarch64.tar.gz
  )
else()
  FetchContent_Declare(
    autd3
    URL https://github.com/shinolab/autd3-cpp/releases/download/v35.0.1/autd3-v35.0.1-linux-x64.tar.gz
  )
endif()
FetchContent_MakeAvailable(autd3)

add_library(autd3-example INTERFACE)
target_compile_definitions(autd3-example INTERFACE AUTD3_RESOURCE_PATH="${PROJECT_SOURCE_DIR}/resources")

add_executable(twincat twincat.cpp)
target_link_libraries(twincat PRIVATE autd3::autd3 autd3::modulation::audio_file autd3::gain::holo autd3::link::twincat)
target_link_libraries(twincat PRIVATE autd3-example)

add_executable(remote_twincat remote_twincat.cpp)
target_link_libraries(remote_twincat PRIVATE autd3::autd3 autd3::modulation::audio_file autd3::gain::holo autd3::link::twincat)
target_link_libraries(remote_twincat PRIVATE autd3-example)

add_executable(simulator simulator.cpp)
target_link_libraries(simulator PRIVATE autd3::autd3 autd3::modulation::audio_file autd3::gain::holo autd3::link::simulator)
target_link_libraries(simulator PRIVATE autd3-example)
