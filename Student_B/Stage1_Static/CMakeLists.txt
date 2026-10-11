add_library(FileCompareLib STATIC
    src/FileComparator.cpp
    include/FileComparator.h
)

target_include_directories(FileCompareLib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(TestApp
    app/main.cpp
)

target_link_libraries(TestApp PRIVATE FileCompareLib)