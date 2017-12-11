cmake_minimum_required (VERSION 2.8)
project (recomob)
set (CMAKE_CXX_STANDARD 11)

if(VCPKG_CHAINLOAD_TOOLCHAIN_FILE)
    include("${VCPKG_CHAINLOAD_TOOLCHAIN_FILE}")
endif()

if(CMAKE_COMPILER_IS_GNUCXX)
    add_definitions(-std=gnu++0x)
endif()

find_package(Boost COMPONENTS system filesystem REQUIRED)

set(JSONCONS_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/src/jsoncons/include)
set(PHYSYCOM_UTILS_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/src/utils)

include_directories(${JSONCONS_INCLUDE_DIR} ${PHYSYCOM_UTILS_INCLUDE_DIR} ${Boost_INCLUDE_DIR})

add_executable(recomob
	${CMAKE_SOURCE_DIR}/src/main.cpp
	${CMAKE_SOURCE_DIR}/src/mob_classes.hpp
	${CMAKE_SOURCE_DIR}/src/points_polygon.h
)

target_link_libraries(recomob ${Boost_LIBRARIES}) 
