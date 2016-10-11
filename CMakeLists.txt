cmake_minimum_required(VERSION 2.6)
project(gpstracker)

add_executable(gpstracker main.c cmdparam.c settings.c sockutils.c fifo.c devices.c devices/fm1100.c)
target_link_libraries(gpstracker event pthread config pq)
include_directories(/usr/include/postgresql)

install(TARGETS gpstracker RUNTIME DESTINATION bin)
