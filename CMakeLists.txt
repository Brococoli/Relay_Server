cmake_minimum_required(VERSION 2.6)

add_compile_options(-std=c++11)

project(RelayServer)

#add .h file localtion
include_directories(include)

#add .cc file dir
aux_source_directory(./src/ SRC_LIST)

#user ${..} ./main.cc to pruduce relay_server
add_executable(relay_server ${SRC_LIST} ./main.cc)


