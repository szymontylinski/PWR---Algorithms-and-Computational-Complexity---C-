cmake_minimum_required(VERSION 3.30)
project(untitled1)

set(CMAKE_CXX_STANDARD 20)

add_executable(untitled1
        cmake-build-debug/include/ISorter.hpp
        cmake-build-debug/include/InsertionSorter.hpp
        cmake-build-debug/include/MyHeapSorter.hpp
        cmake-build-debug/include/ShellSorter.hpp
        cmake-build-debug/include/QuickSorter.hpp
        cmake-build-debug/include/DataGenerator.hpp
        cmake-build-debug/include/App.hpp
        cmake-build-debug/src/main.cpp)
