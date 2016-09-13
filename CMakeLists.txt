cmake_minimum_required(VERSION 2.6)
project(gpstracker)

add_executable(gpstracker main.cpp)

install(TARGETS gpstracker RUNTIME DESTINATION bin)
