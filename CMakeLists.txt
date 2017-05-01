cmake_minimum_required(VERSION 3.7)
project(HUFFMAN)

set(CMAKE_C_STANDARD 11)

set(SOURCE_FILES main.c treeQueue.h treeQueue.c listFrequence.c listFrequence.h dictionaryTable.c dictionaryTable.h compressModules.c compressModules.h decompressModules.c decompressModules.h stack.c stack.h)
add_executable(HUFFMAN ${SOURCE_FILES} treeQueue.h listFrequence.h)
target_link_libraries(HUFFMAN)