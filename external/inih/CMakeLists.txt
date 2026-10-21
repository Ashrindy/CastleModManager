cmake_minimum_required(VERSION 3.28)
project(inih LANGUAGES CXX)

add_library(inih STATIC)
target_include_directories(inih PUBLIC inih/cpp)
target_sources(inih
    PRIVATE
        inih/ini.c
        inih/cpp/INIReader.cpp
)
