cmake_minimum_required(VERSION 3.7)
project(HUFFMAN)

set(CMAKE_C_STANDARD 11)

set(SOURCE_FILES main.c treeQueue.h treeQueue.c listFrequence.c listFrequence.h)
add_executable(HUFFMAN ${SOURCE_FILES} treeQueue.h treeQueue.c listFrequence.c listFrequence.h)