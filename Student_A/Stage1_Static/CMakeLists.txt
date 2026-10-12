cmake_minimum_required(VERSION 3.20)
project(TextAnalyzerStatic LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if(MSVC)
    add_compile_options(/utf-8)
endif()

# статична бібліотека
add_library(TextAnalyzer STATIC
    src/TextAnalyzer.cpp
    include/TextAnalyzer.h
)
target_include_directories(TextAnalyzer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# тестова програма
add_executable(TestApp app/main.cpp)
target_link_libraries(TestApp PRIVATE TextAnalyzer)
