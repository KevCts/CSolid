cmake_minimum_required(VERSION 3.30)

SET(CMAKE_BUILD_TYPE Debug)

project(CSolid)

add_subdirectory(lib/cvide)

set(SRCS
	main.c
    model.c
    node.c
    element.c
    section.c
    material.c
    elements/bar.c
)

set(HEADERS
    model.h
    node.h
    element.h
    section.h
    material.h
    elements/bar.h
)

add_executable(CSolid ${SRCS} ${HEADERS})

target_link_libraries(CSolid libcvide)
