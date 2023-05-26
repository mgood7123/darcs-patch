cmake_minimum_required(VERSION 3.4.1)

project(DarcsPatch)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_FLAGS "-ferror-limit=1")

if(NOT COMMAND testBuilder_build)
    add_subdirectory(testBuilder)
    testBuilder_set_current_working_directory_to_default_binary_directory()
endif()

testBuilder_add_include(darcs_patch include)
testBuilder_add_source(darcs_patch src/darcs.cpp)
testBuilder_add_source(darcs_patch src/darcs1.cpp)
testBuilder_build_shared_library(darcs_patch)

testBuilder_add_source(darcs_patch_exe src/darcs_exe.cpp)
testBuilder_add_library(darcs_patch_exe darcs_patch)
testBuilder_build(darcs_patch_exe EXECUTABLES)

set(DARCS_PATCH_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/include PARENT_SCOPE)
