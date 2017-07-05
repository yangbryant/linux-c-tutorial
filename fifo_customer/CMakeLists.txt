cmake_minimum_required (VERSION 2.8)

project(fifo_customer)

aux_source_directory(. DIR_SRCS)

add_executable(fifo_customer ${DIR_SRCS})