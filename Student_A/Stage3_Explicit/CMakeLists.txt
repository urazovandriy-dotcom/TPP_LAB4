cmake_minimum_required(VERSION 3.20)
project(ExplicitClient LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if(MSVC)
    add_compile_options(/utf-8)
endif()

# тут не підключаємо ні заголовок, ні .lib - тільки dll
add_executable(ExplicitApp main.cpp)

# dll копіюємо поруч з exe, щоб LoadLibrary її знайшла
add_custom_command(TARGET ExplicitApp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/FileComparatorDLL.dll
        $<TARGET_FILE_DIR:ExplicitApp>
)
