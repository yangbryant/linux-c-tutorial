cmake_minimum_required (VERSION 2.8)

# project info
project (com_writer)

configure_file (
    "${PROJECT_SOURCE_DIR}/config.h.in"
    "${PROJECT_BINARY_DIR}/config.h"
    )

option (USE_COM_CONFIG
    "Use provided com config" ON)

if (USE_COM_CONFIG)
    include_directories ("${PROJECT_SOURCE_DIR}/set_com_config")
    add_subdirectory(set_com_config)
    set (EXTRA_LIBS ${EXTRA_LIBS} set_com_config)
endif (USE_COM_CONFIG)

aux_source_directory(. DIR_SRCS)

add_executable(com_writer ${DIR_SRCS})

target_link_libraries(com_writer ${EXTRA_LIBS})