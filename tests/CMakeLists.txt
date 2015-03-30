cmake_minimum_required (VERSION 2.8)

include_directories(cutest-1.5)

enable_testing()

add_library(cutest-1.5
	cutest-1.5/CuTest.c
	cutest-1.5/CuTest.h
)


add_executable(BasicTest
	01-Basic.cpp
)

target_link_libraries(BasicTest GPGoapLib cutest-1.5)

