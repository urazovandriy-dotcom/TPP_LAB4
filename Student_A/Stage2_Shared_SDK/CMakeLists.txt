cmake_minimum_required(VERSION 3.20)
project(TextAnalyzerShared LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if(MSVC)
    add_compile_options(/utf-8)
endif()

# динамічна бібліотека
add_library(TextAnalyzerDLL SHARED
    src/TextAnalyzerDLL.cpp
    include/TextAnalyzerDLL.h
)
target_compile_definitions(TextAnalyzerDLL PRIVATE TEXTANALYZER_EXPORTS)
target_include_directories(TextAnalyzerDLL PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# тестова програма
add_executable(TestApp app/main.cpp)
target_link_libraries(TestApp PRIVATE TextAnalyzerDLL)

# після збирання складаємо SDK-пакунок для напарника (тільки з Release)
if(CMAKE_BUILD_TYPE STREQUAL "Release")
add_custom_command(TARGET TextAnalyzerDLL POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_SOURCE_DIR}/SDK/include
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_SOURCE_DIR}/SDK/lib
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_SOURCE_DIR}/SDK/bin
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/include/TextAnalyzerDLL.h ${CMAKE_CURRENT_SOURCE_DIR}/SDK/include/
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:TextAnalyzerDLL> ${CMAKE_CURRENT_SOURCE_DIR}/SDK/bin/
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_LINKER_FILE:TextAnalyzerDLL> ${CMAKE_CURRENT_SOURCE_DIR}/SDK/lib/
)
endif()
