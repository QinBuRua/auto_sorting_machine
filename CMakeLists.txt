cmake_minimum_required(VERSION 4.0)
project(auto_sorting_machine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

aux_source_directory(includes/classes CLASSES_SRC)
add_library(CommonCore STATIC
        ${CLASSES_SRC}
)
target_include_directories(CommonCore PUBLIC
        includes
        utils
)

add_executable(auto_sorting_machine
        src/main.cpp
)
target_link_libraries(auto_sorting_machine PUBLIC CommonCore)

add_executable(app_test
        src/test.cpp
)
target_link_libraries(app_test PUBLIC CommonCore)