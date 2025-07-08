cmake_minimum_required(VERSION 3.25)
project(search_engine)

set(CMAKE_CXX_STANDARD 20)
set(JSON_BuildTests OFF CACHE INTERNAL "")

# Подключаем JSON-библиотеку
add_subdirectory(nlohmann_json)
include_directories(include)

# GoogleTest
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
enable_testing()

# ================== Основной проект ==================
add_executable(search_engine
        main.cpp
        include/ConverterJson.h
        src/ConverterJson.cpp
        src/InvertedIndex.cpp
        include/InvertedIndex.h
        include/SearchServer.h
        src/SearchServer.cpp)
target_link_libraries(search_engine PRIVATE
        nlohmann_json::nlohmann_json
        )
file(COPY ${CMAKE_SOURCE_DIR}/JSON DESTINATION ${CMAKE_BINARY_DIR})

# ================== Тесты ==================
enable_testing()

add_executable(tests
        tests/test_main.cpp
        src/ConverterJson.cpp
        src/InvertedIndex.cpp
        src/SearchServer.cpp
        )

target_link_libraries(tests PRIVATE
        gtest
        gtest_main
        nlohmann_json::nlohmann_json
        )
include(GoogleTest)
gtest_discover_tests(tests)
