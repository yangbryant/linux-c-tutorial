cmake_minimum_required (VERSION 2.8)

aux_source_directory(. DIR_LIB_SRCS)

add_library(set_com_config ${DIR_LIB_SRCS})