
cmake_minimum_required(VERSION 3.9)

set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "whatever" FORCE)

project(GraphWithDX)

if(MSVC)
    add_compile_options(
        $<$<CONFIG:>:/MT> #---------|
        $<$<CONFIG:Debug>:/MTd> #---|-- Statically link the runtime libraries
        $<$<CONFIG:Release>:/MT> #--|
    )

    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")

endif()

##############Common################

file(GLOB COMMON_SOURCES "${PROJECT_SOURCE_DIR}/src/Common/*.cpp")

INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/src/Common")

add_library(Common ${COMMON_SOURCES})


####################################