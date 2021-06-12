
include_directories(${TEST_SOURCE_DIR}/src)

add_executable(Test test.cpp)

target_link_libraries(Test freeini)

