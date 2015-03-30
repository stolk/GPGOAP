cmake_minimum_required (VERSION 2.8)
project (GPGoap)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11 -ggdb -Wall -Wextra")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -ggdb -Wall -Wextra")

add_subdirectory(tests)

add_library(GPGoapLib
	astar.c
	astar.h
	goap.c
	goap.h
)

add_executable(GoapTest
	main.c
)

target_link_libraries(GoapTest GPGoapLib)
