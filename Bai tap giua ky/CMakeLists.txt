cmake_minimum_required(VERSION 3.0)

project(AES_128 VERSION 1.0)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Declare the include folder for custom headers
include_directories(${CMAKE_SOURCE_DIR}/include)
################################################################

# Building custom libraries#####################################
add_library(AES STATIC lib/AES.c)

add_library(encrypt STATIC lib/encrypt.c)
target_link_libraries(encrypt AES)

add_library(decrypt STATIC lib/decrypt.c)
target_link_libraries(decrypt AES)
################################################################

#TOOLS COMPILE##################################################
add_library(utils STATIC lib/utils.c)
################################################################

# MAIN COMPILE##################################################
add_executable(main src/main.c)
target_link_libraries(main encrypt decrypt utils)
################################################################

# Testing Programs Declarations
enable_testing()
################################################################