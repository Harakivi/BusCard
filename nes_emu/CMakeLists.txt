cmake_minimum_required(VERSION 3.20)

file(GLOB NES_EMU_SRC 
            source/*.c)

add_library(NES_EMU STATIC nes.cpp ${NES_EMU_SRC})

target_include_directories(NES_EMU PRIVATE include/
                                   PUBLIC ./)

target_compile_options(NES_EMU PRIVATE -Ofast)

target_link_options(NES_EMU PRIVATE -nostdlib)