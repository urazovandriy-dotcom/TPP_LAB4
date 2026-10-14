cmake_minimum_required(VERSION 3.15)
project(Stage3_Integration CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Важливо для коректної кирилиці у рядках коду (згідно з контрактом Студента А)
if (MSVC)
    add_compile_options(/utf-8)
endif()

# Шлях до SDK Студента А
set(STUDENT_A_SDK_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../Student_A/Stage2_Shared_SDK/SDK")

# 1. Проєкт з НЕЯВНИМ зв'язуванням (Implicit Linking)
add_executable(ImplicitApp
    implicit/main_implicit.cpp
)

target_include_directories(ImplicitApp PRIVATE
    "${STUDENT_A_SDK_DIR}/include"
)

target_link_directories(ImplicitApp PRIVATE
    "${STUDENT_A_SDK_DIR}/lib"
)

target_link_libraries(ImplicitApp PRIVATE
    TextAnalyzerDLL
)

# Автоматичне копіювання DLL у вихідну папку запуску
add_custom_command(TARGET ImplicitApp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${STUDENT_A_SDK_DIR}/bin/TextAnalyzerDLL.dll"
    $<TARGET_FILE_DIR:ImplicitApp>
)

# 2. Проєкт з ЯВНИМ зв'язуванням (Explicit Linking / Run-time)
add_executable(ExplicitApp
    explicit/main_explicit.cpp
)

# Автоматичне копіювання DLL поруч з ExplicitApp.exe
add_custom_command(TARGET ExplicitApp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${STUDENT_A_SDK_DIR}/bin/TextAnalyzerDLL.dll"
    $<TARGET_FILE_DIR:ExplicitApp>
)