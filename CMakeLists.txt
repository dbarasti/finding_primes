cmake_minimum_required(VERSION 3.11 FATAL_ERROR)
project(finding_primes VERSION 1.0
        DESCRIPTION "Second Assignment"
        LANGUAGES CXX)

# find package needed by the project
find_package(Threads REQUIRED)
find_package(OpenMP REQUIRED)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

# different build requires different flags
set(CMAKE_CXX_FLAGS_DEBUG "-Wall -Wextra -O0 -g -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -fopt-info-vec")

# creating the target SecondAssignmentParallel
add_executable(finding_primes main.cpp ParallelForExec.hpp)
add_library(master_worker STATIC MasterWorker.hpp)

# set compiler
target_compile_features(finding_primes PUBLIC cxx_std_17)
target_compile_features(master_worker PUBLIC cxx_std_17)

set_target_properties(master_worker PROPERTIES LINKER_LANGUAGE CXX)

# link libraries
target_link_libraries(finding_primes master_worker)
target_link_libraries(finding_primes
        OpenMP::OpenMP_CXX
        Threads::Threads
        )