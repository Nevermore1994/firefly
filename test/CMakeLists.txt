project(test)

aux_source_directory(. DIR_CORE_SRCS)

add_library(test ${DIR_CORE_SRCS})