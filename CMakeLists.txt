cmake_minimum_required(VERSION 4.0)
project(auto_sorting_machine)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

aux_source_directory(utils/Logger UTILS_Logger_SRC)
aux_source_directory(utils/Vector UTILS_Vector_SRC)

aux_source_directory(includes/details/TokenizerTrainer DETAILS_TokenizerTrainer_SRC)
aux_source_directory(includes/details/Tokenizer DETAILS_Tokenizer_SRC)
aux_source_directory(includes/details/MarkovChainModel DETAILS_MarkovChainModel_SRC)
aux_source_directory(includes/details/ModelHeader DETAILS_ModelHeader_SRC)
aux_source_directory(includes/classes CLASSES_SRC)

add_library(CommonCore STATIC
        ${UTILS_Logger_SRC}
        ${UTILS_Vector_SRC}
        ${CLASSES_SRC}
        ${DETAILS_TokenizerTrainer_SRC}
        ${DETAILS_Tokenizer_SRC}
        ${DETAILS_MarkovChainModel_SRC}
        ${DETAILS_ModelHeader_SRC}
)
target_include_directories(CommonCore PUBLIC
        includes
        utils
)

aux_source_directory(includes/test TEST_SRC)
add_library(TestCore STATIC
        ${TEST_SRC}
)
target_include_directories(TestCore PUBLIC
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
target_link_libraries(app_test PUBLIC CommonCore TestCore)