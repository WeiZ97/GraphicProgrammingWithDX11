
cmake_minimum_required(VERSION 3.9)

set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "whatever" FORCE)

project(GraphWithDX)

if(MSVC)
    add_compile_options(
        $<$<CONFIG:>:/MT> #---------|
        $<$<CONFIG:Debug>:/MTd> #---|-- Statically link the runtime libraries
        $<$<CONFIG:Release>:/MT> #--|
    )
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
    add_definitions(-DUNICODE -D_UNICODE)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")

endif()

if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/src/hlsl)
	file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/src/hlsl DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endif()

find_library(D3D11_LIB d3d11.lib PATHS "$ENV{D3D_DIR}")
find_library(D3DCOMPILER_LIB d3dcompiler.lib PATHS "$ENV{D3D_DIR}")
find_library(DXGI_LIB dxgi.lib PATHS "$ENV{D3D_DIR}")
find_library(DXGUID_LIB dxguid.lib PATHS "$ENV{D3D_DIR}")
find_library(WINMM_LIB winmm.lib PATHS "$ENV{D3D_DIR}")




##############Common################


file(GLOB COMMON_SOURCES "${PROJECT_SOURCE_DIR}/src/Common/*.cpp")

INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/src/Common")
INCLUDE_DIRECTORIES("$ENV{WINSDK_INCLUDES}")
 
add_library(Common ${COMMON_SOURCES})
target_link_libraries(Common "${D3D11_LIB}" "${D3DCOMPILER_LIB}" "${DXGI_LIB}" "${DXGUID_LIB}" "${WINMM_LIB}")


####################################




##############Ch4/InitDriect3D######

file(GLOB InitDX3DSOURCES "${PROJECT_SOURCE_DIR}/src/Ch4/InitDirect3D/*.cpp")

add_executable(InitDX3D ${InitDX3DSOURCES})
target_link_libraries(InitDX3D Common)

############Triangle################
file(GLOB DrawTriangleSOURCES "${PROJECT_SOURCE_DIR}/src/Ch6/Triangle/*.cpp")

add_executable(DrawTriangle ${DrawTriangleSOURCES})
target_link_libraries(DrawTriangle Common)


####BoxDemo#####

file(GLOB BoxDemoSOURCES "${PROJECT_SOURCE_DIR}/src/Ch6/Box/*.cpp")

#add_executable(BoxDemo ${BoxDemoSOURCES})
#target_link_libraries(BoxDemo Common)

################