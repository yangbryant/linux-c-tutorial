cmake_minimum_required (VERSION 2.8)

project (com_reader)

aux_source_directory(. DIR_SRCS)

add_executable(com_reader ${DIR_SRCS})