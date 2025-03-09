cmake_minimum_required(VERSION 3.30)

SET(CMAKE_BUILD_TYPE Debug)

project(CSolid)

add_subdirectory(lib/cvide)

set(SRCS
	main.c
    lib/model/model.c
    lib/model/node.c
    lib/model/element.c
    lib/model/section.c
    lib/model/material.c
    lib/model/elements/bar.c
)

set(HEADERS
    lib/model/model.h
    lib/model/node.h
    lib/model/element.h
    lib/model/section.h
    lib/model/material.h
    lib/model/elements/bar.h
)

add_executable(CSolid ${SRCS} ${HEADERS})

target_link_libraries(CSolid libcvide)
