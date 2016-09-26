cmake_minimum_required(VERSION 2.6)
project(gpstracker)

add_executable(gpstracker main.c cmdparam.c settings.c sockutils.c fifo.c threadFunc.c devices/fm1100.c)
target_link_libraries(gpstracker event pthread config)

install(TARGETS gpstracker RUNTIME DESTINATION bin)
