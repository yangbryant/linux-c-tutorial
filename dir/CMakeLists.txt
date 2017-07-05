cmake_minimum_required (VERSION 2.8)

project(dir)

aux_source_directory(. DIR_SRCS)

add_executable(dir ${DIR_SRCS})
