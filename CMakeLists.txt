cmake_minimum_required(VERSION 3.21)

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
  message(FATAL_ERROR "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there. You may need to remove CMakeCache.txt. ")
endif()

project(autd3 VERSION 37.0.1)

message(STATUS "Build AUTD3: ${PROJECT_VERSION}")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

option(USE_SYSTEM_EIGEN "Use system installed eigen3" OFF)

if(${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.24.0") 
  cmake_policy(SET CMP0135 NEW)
endif()

include(FetchContent)
if(USE_SYSTEM_EIGEN)
  find_package(Eigen3 REQUIRED)
else()
  set(EIGEN_BUILD_DOC OFF)
  set(BUILD_TESTING OFF)
  set(EIGEN_BUILD_PKGCONFIG OFF)
  FetchContent_Declare(
    eigen
    GIT_REPOSITORY https://gitlab.com/libeigen/eigen.git
    GIT_TAG 5.0.0)
  FetchContent_MakeAvailable(eigen)
endif()

add_library(autd3_common INTERFACE)
target_include_directories(autd3_common INTERFACE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(autd3_common INTERFACE Eigen3::Eigen)
target_compile_definitions(autd3_common INTERFACE EIGEN_MPL2_ONLY)
if(WIN32)
  target_link_directories(autd3_common INTERFACE ${PROJECT_SOURCE_DIR}/lib)
else()
  target_link_directories(autd3_common INTERFACE ${PROJECT_SOURCE_DIR}/bin)
endif()
if(MSVC)
  target_compile_options(autd3_common INTERFACE /wd4819)
  target_compile_options(autd3_common INTERFACE /wd4324)
endif()
if(WIN32)
  target_compile_definitions(autd3_common INTERFACE NOMINMAX)
endif()
add_library(autd3::common ALIAS autd3_common)

macro(autd_target_link_libraries target lib)
  if(WIN32)
    target_link_libraries(${target} INTERFACE "${lib}.dll.lib")
  elseif(APPLE)
    target_link_libraries(${target} INTERFACE ${lib})
  else()
    target_link_libraries(${target} INTERFACE ${lib})
  endif()
endmacro()

add_library(autd3 INTERFACE)
target_link_libraries(autd3 INTERFACE autd3::common)
autd_target_link_libraries(autd3 autd3capi)
if(WIN32)
  target_link_libraries(autd3 INTERFACE ntdll.lib Winmm.lib bcrypt.lib userenv.lib RuntimeObject.lib Propsys.lib)
  set_property(GLOBAL PROPERTY USE_FOLDERS ON)
  add_custom_target(autd3capi)
  add_custom_command(TARGET autd3capi POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/$<CONFIG>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${PROJECT_SOURCE_DIR}/bin/autd3capi.dll" ${CMAKE_BINARY_DIR}/$<CONFIG>
    VERBATIM
  )
  set_property(TARGET autd3capi PROPERTY FOLDER "autd3capi") 
  add_dependencies(autd3 autd3capi)
elseif(APPLE)
  target_link_libraries(autd3 INTERFACE "-framework CoreFoundation")
endif()
add_library(autd3::autd3 ALIAS autd3)

add_library(autd3_gain_holo INTERFACE)
target_link_libraries(autd3_gain_holo INTERFACE autd3::common)
autd_target_link_libraries(autd3_gain_holo autd3capi_gain_holo)
if(WIN32)
  add_custom_target(autd3capi_gain_holo)
  add_custom_command(TARGET autd3capi_gain_holo POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/$<CONFIG>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${PROJECT_SOURCE_DIR}/bin/autd3capi_gain_holo.dll" ${CMAKE_BINARY_DIR}/$<CONFIG>
    VERBATIM
  )
  set_property(TARGET autd3capi_gain_holo PROPERTY FOLDER "autd3capi") 
  add_dependencies(autd3_gain_holo autd3capi_gain_holo)
endif()
add_library(autd3::gain::holo ALIAS autd3_gain_holo)

add_library(autd3_link_twincat INTERFACE)
target_link_libraries(autd3_link_twincat INTERFACE autd3::common)
autd_target_link_libraries(autd3_link_twincat autd3capi_link_twincat)
if(WIN32)
  target_link_libraries(autd3_link_twincat INTERFACE ws2_32)
  add_custom_target(autd3capi_link_twincat)
  add_custom_command(TARGET autd3capi_link_twincat POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/$<CONFIG>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${PROJECT_SOURCE_DIR}/bin/autd3capi_link_twincat.dll" ${CMAKE_BINARY_DIR}/$<CONFIG>
    VERBATIM
  )
  set_property(TARGET autd3capi_link_twincat PROPERTY FOLDER "autd3capi") 
  add_dependencies(autd3_link_twincat autd3capi_link_twincat)
endif()
add_library(autd3::link::twincat ALIAS autd3_link_twincat)

add_library(autd3_link_remote INTERFACE)
target_link_libraries(autd3_link_remote INTERFACE autd3::common)
autd_target_link_libraries(autd3_link_remote autd3capi_link_remote)
if(WIN32)
  target_link_libraries(autd3_link_remote INTERFACE ws2_32)
  add_custom_target(autd3capi_link_remote)
  add_custom_command(TARGET autd3capi_link_remote POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/$<CONFIG>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${PROJECT_SOURCE_DIR}/bin/autd3capi_link_remote.dll" ${CMAKE_BINARY_DIR}/$<CONFIG>
    VERBATIM
  )
  set_property(TARGET autd3capi_link_remote PROPERTY FOLDER "autd3capi") 
  add_dependencies(autd3_link_remote autd3capi_link_remote)
endif()
add_library(autd3::link::remote ALIAS autd3_link_remote)
