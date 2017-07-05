cmake_minimum_required (VERSION 2.8)

project (com_host)

aux_source_directory(. DIR_SRCS)

add_subdirectory(set_com_config)

add_executable(com_host ${DIR_SRCS})

target_link_libraries(com_host set_com_config)