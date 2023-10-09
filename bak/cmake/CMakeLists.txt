aux_source_directory(. CUR_DIR_SRCS)
add_library(Cal ${CUR_DIR_SRCS})

set(LINK_FLAGS "-lstdc++")
set(CMAKE_CXX_COMPILER g++)


target_include_directories(Cal PUBLIC "../include/")
