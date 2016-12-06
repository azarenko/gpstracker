cmake_minimum_required(VERSION 2.6)
project(fm1100)
project(tr151)

add_executable(fm1100 main.c cmdparam.c settings.c sockutils.c fifo.c devices.c devices/fm1100.c)
target_link_libraries(fm1100 event pthread config pq)

add_executable(tr151 main.c cmdparam.c settings.c sockutils.c fifo.c devices.c devices/tr151.c)
target_link_libraries(tr151 event pthread config pq)

include_directories(/usr/include/postgresql)

install(TARGETS fm1100 RUNTIME DESTINATION bin)
install(TARGETS tr151 RUNTIME DESTINATION bin)
