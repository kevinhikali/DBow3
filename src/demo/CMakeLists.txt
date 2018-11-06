cmake_minimum_required(VERSION 2.8)

add_executable(train train.cpp)
target_link_libraries(train ${LINK_LIBS})
