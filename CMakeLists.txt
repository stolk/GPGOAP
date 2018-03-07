
cmake_minimum_required(VERSION 3.4.1)

# build libgpgoap as a static lib
add_library(gpgoap STATIC
	astar.c
	goap.c
)


#LOCAL_CFLAGS += -std=c99


