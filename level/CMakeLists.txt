cmake_minimum_required(VERSION 3.22)

project(level VERSION 1.0)

add_library(${PROJECT_NAME} SHARED
        ./source/level_asset.c
        ./source/sublevel_asset.c
        ./include/level/internal/module.h
        )

target_link_libraries(${PROJECT_NAME}
        PUBLIC library
        PUBLIC math
        PUBLIC mesh
        PUBLIC spatial
        PUBLIC elements)

target_include_directories(${PROJECT_NAME} PUBLIC
        "${PROJECT_SOURCE_DIR}/include"
        )