cmake_minimum_required(VERSION 3.10)
project(ChessGame)

set(CMAKE_CXX_STANDARD 17)

# Добавить основную директорию исходного кода
add_executable(ChessGame main.cpp chess.cpp)

# Установить путь включения для Doctest
include_directories(${CMAKE_SOURCE_DIR}/external/doctest)

# Создать исполняемый файл для тестов
add_executable(ChessGameTests tests.cpp chess.cpp)

# Установить путь включения для Doctest для тестов
target_include_directories(ChessGameTests PRIVATE ${CMAKE_SOURCE_DIR}/external/doctest)

# Добавить тесты
enable_testing()
add_test(NAME ChessGameTests COMMAND ChessGameTests)
