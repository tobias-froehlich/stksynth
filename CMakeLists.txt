cmake_minimum_required(VERSION 3.13)
project(stksynth VERSION 0.1)



set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)



set (PROJECT_SOURCE_DIR "${CMAKE_SOURCE_DIR}/src")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/bin")


include_directories(/usr/include/stk)
set(CMAKE_CXX_FLAGS "-O3 -Wall -D__LITTLE_ENDIAN__ -DHAVE_GETTIMEOFDAY -D__LINUX_ALSA__")
set(CMAKE_CXX_STANDARD_LIBRARIES "-lstk -lpthread -lasound -lm -lrtaudio -lrtmidi")
#add_definitions(-D__LITTLE_ENDIAN__)
#add_definitions(-D__LINUX_ALSA__)

configure_file(src/stksynthConfig.h.in stksynthConfig.h)

add_subdirectory(test)
#add_subdirectory(stk)
add_subdirectory(src)

#target_link_libraries(stksynth stk)
#target_link_libraries(stksynth rtaudio)
#include(./stk/include/Stk.h)
#include(./stk/include/RtAudio.h)

set(CMAKE_CXX_FLAGS "-Wall")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lstk -lrtaudio")
target_include_directories(stksynth PUBLIC "${PROJECT_BINARY_DIR}")


