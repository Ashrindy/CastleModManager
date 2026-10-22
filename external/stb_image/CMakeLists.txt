cmake_minimum_required(VERSION 3.28)
project(stb_image LANGUAGES CXX)

add_library(stb_image INTERFACE)
target_include_directories(stb_image INTERFACE stb)
