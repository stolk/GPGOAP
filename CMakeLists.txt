cmake_minimum_required(VERSION 2.8)
cmake_policy(SET CMP0011 NEW)

PROJECT(GPGOAG)

set(SOURCE_CODE astar.c goap.c)
set(HEADER_CODE astar.h goap.h)
IF(MSVC)
    set_source_files_properties(${SOURCE_CODE} ${HEADER_CODE} main.c PROPERTIES COMPILE_FLAGS "/TP")
ENDIF()
include_directories(.)
add_library(GPGOAG STATIC ${SOURCE_CODE} ${HEADER_CODE})
add_executable(test main.c)
target_link_libraries(test GPGOAG)
install(    TARGETS GPGOAG test
			RUNTIME DESTINATION bin
			LIBRARY DESTINATION lib
			ARCHIVE DESTINATION lib)
install(    FILES ${HEADER_CODE}
            DESTINATION "include/GPGOAG")
