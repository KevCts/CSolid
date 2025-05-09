cmake_minimum_required(VERSION 3.30)

SET(CMAKE_BUILD_TYPE Debug)

project(CSolid)

add_subdirectory(lib/csolid)

set(SRCS
	main.c
    lib/interpreter/vm.c
    lib/interpreter/compiler.c
)

set(HEADERS
    lib/interpreter/vm.h
    lib/interpreter/compiler.h
)

add_executable(CSolid ${SRCS} ${HEADERS})

target_link_libraries(CSolid libcsolid)
