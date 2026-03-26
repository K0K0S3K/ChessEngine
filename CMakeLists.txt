cmake_minimum_required(VERSION 3.10)
project(ChessEngine)

# Ustawienie standardu C++ (zgodnie z README.md wspierasz C++17/20)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Dodanie folderu z nagłówkami
include_directories(include)

# Automatyczne znalezienie wszystkich plików źródłowych
file(GLOB SOURCES "engine/src/*.cpp")

# Tworzenie pliku wykonywalnego
add_executable(ChessEngine ${SOURCES})
