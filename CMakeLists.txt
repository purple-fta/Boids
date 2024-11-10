cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_COMPILER "C:/mingw64/bin/g++.exe")
project(Roy LANGUAGES CXX)

set(SFML_VERSION 2.6.1)
set(SFML_DIR "C:/SFML/SFML-${SFML_VERSION}/lib/cmake/SFML")

find_package(SFML ${SFML_VERSION} COMPONENTS graphics window system REQUIRED)

set(SOURCES src/main.cpp)

add_executable(main ${SOURCES})

target_link_libraries(main sfml-graphics sfml-window sfml-system opengl32)
