# CMakeLists.txt file
# Author: Piotr Palucki
# This file allows to generate cmake files for building project with simple "make"

cmake_minimum_required(VERSION 2.8.9)
project(STEGANO)

set(CMAKE_CXX_FLAGS "-Ofast -Wall -std=c++11")

#Bring the headers
include_directories(include)

#Sources are added by wildcard matching all .cpp files
#in the future probably better would be to add_subdirectory
file(GLOB SOURCES "src/*.cpp")

#create target
add_executable(SavGol.o ${SOURCES})
