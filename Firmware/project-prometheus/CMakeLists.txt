cmake_minimum_required(VERSION 3.20.0)
set(LIB_DIR  ${CMAKE_CURRENT_SOURCE_DIR}/libraries)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(project-prometheus)

target_sources(app PRIVATE src/main.c)
