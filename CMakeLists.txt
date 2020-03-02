cmake_minimum_required(VERSION 3.10)
project(fsbench)

set(CMAKE_CXX_STANDARD 14)
link_libraries(pthread)
add_definitions("-O2")

aux_source_directory(src/core/ SOURCE_CORE)
aux_source_directory(src/http/ SOURCE_HTTP)

add_executable(fsbench
    ${SOURCE_CORE}
    ${SOURCE_HTTP}
)