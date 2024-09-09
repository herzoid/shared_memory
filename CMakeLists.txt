cmake_minimum_required(VERSION 3.5)
project(shm_utils VERSION 0.1.0 LANGUAGES C CXX)

#include(CTest)
#enable_testing()

add_subdirectory(shm_writer)
set(CMAKE_PROJECT1_NAME "write_shm" CACHE INTERNAL "")

add_subdirectory(shm_reader)
set(CMAKE_PROJECT2_NAME "read_shm" CACHE INTERNAL "")

add_subdirectory(shmlib)
set(CMAKE_PROJECT3_NAME "shmutils" CACHE INTERNAL "")

#set(CPACK_PROJECT_NAME ${PROJECT_NAME})
#set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
#include(CPack)
