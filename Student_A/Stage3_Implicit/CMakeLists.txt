cmake_minimum_required(VERSION 3.20)
project(ImplicitClient LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if(MSVC)
    add_compile_options(/utf-8)
endif()

set(FILECOMP_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/FileComparator)

add_executable(ImplicitApp main.cpp)

# заголовок і бібліотека імпорту з SDK напарника
target_include_directories(ImplicitApp PRIVATE ${FILECOMP_DIR}/include)
target_link_libraries(ImplicitApp PRIVATE ${FILECOMP_DIR}/lib/FileComparatorDLL.lib)

# dll копіюємо поруч з exe, інакше програма не запуститься
add_custom_command(TARGET ImplicitApp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${FILECOMP_DIR}/bin/FileComparatorDLL.dll
        $<TARGET_FILE_DIR:ImplicitApp>
)
