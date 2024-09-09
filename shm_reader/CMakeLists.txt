add_executable(
    ${CMAKE_PROJECT2_NAME}
    src/readobj.cpp 
)

set_target_properties(
    ${CMAKE_PROJECT2_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

target_link_libraries(${CMAKE_PROJECT2_NAME} ${CMAKE_PROJECT3_NAME})

#set(CPACK_PROJECT_NAME ${PROJECT_NAME})
#set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
#include(CPack)
