project(network)

aux_source_directory(. DIR_CORE_SRCS)

add_library(network ${DIR_CORE_SRCS})