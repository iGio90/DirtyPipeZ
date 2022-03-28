cmake_minimum_required(VERSION 3.18.1)

project("dirtypipez")

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Os -fno-stack-protector")

set(can_use_assembler TRUE)
enable_language(ASM)

find_library(log-lib log)

add_library(dirtypipez SHARED dirtypipez.c)
target_include_directories(dirtypipez PRIVATE arch/${ANDROID_ABI})
target_include_directories(dirtypipez PRIVATE include)
target_link_libraries(dirtypipez ${log-lib})


