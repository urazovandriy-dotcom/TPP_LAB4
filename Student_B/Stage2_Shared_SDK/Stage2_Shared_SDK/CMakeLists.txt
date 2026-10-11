add_library(FileComparatorDLL SHARED
    src/FileComparatorDLL.cpp
    include/FileComparatorDLL.h
)

target_compile_definitions(FileComparatorDLL PRIVATE FILECOMPARATOR_EXPORTS)

target_include_directories(FileComparatorDLL PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(TestApp
    app/main.cpp
)

target_link_libraries(TestApp PRIVATE FileComparatorDLL)

add_custom_command(TARGET FileComparatorDLL POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_SOURCE_DIR}/SDK/include
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_SOURCE_DIR}/SDK/lib
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_SOURCE_DIR}/SDK/bin
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/include/FileComparatorDLL.h ${CMAKE_CURRENT_SOURCE_DIR}/SDK/include/
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:FileComparatorDLL> ${CMAKE_CURRENT_SOURCE_DIR}/SDK/bin/
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_LINKER_FILE:FileComparatorDLL> ${CMAKE_CURRENT_SOURCE_DIR}/SDK/lib/
)