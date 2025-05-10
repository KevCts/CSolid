cmake_minimum_required(VERSION 3.30)

SET(CMAKE_BUILD_TYPE Debug)

project(CSolid)

add_subdirectory(lib/csolid)

set(SRCS
	main.c
    lib/vm.c
    lib/chunk.c
    lib/compiler.c
    lib/lexer.c
    lib/memory.c
)

set(HEADERS
    lib/vm.h
    lib/chunk.h
    lib/compiler.h
    lib/lexer.h
    lib/memory.h
)

add_executable(CSolid ${SRCS} ${HEADERS})

target_link_libraries(CSolid libcsolid)
