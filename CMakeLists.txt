cmake_minimum_required(VERSION 3.4.1)

project(DarcsPatch)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT COMMAND testBuilder_build)
    add_subdirectory(testBuilder)
    testBuilder_set_current_working_directory_to_default_binary_directory()
endif()

add_subdirectory(StringAdapter)

testBuilder_add_include(darcs_patch include)
testBuilder_add_include(darcs_patch hash-library)
testBuilder_add_source(darcs_patch hash-library/sha1.cpp)
testBuilder_add_source(darcs_patch src/darcs.cpp)
testBuilder_add_library(darcs_patch StringAdapter)
testBuilder_build_shared_library(darcs_patch)

if (NOT TARGET gtest AND NOT TARGET gtest_main)
    # Prevent overriding the parent project's compiler/linker
    # settings on Windows
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

    # Add googletest directly to our build. This defines
    # the gtest and gtest_main targets.
    add_subdirectory(googletest)

    # The gtest/gtest_main targets carry header search path
    # dependencies automatically when using CMake 2.8.11 or
    # later. Otherwise we have to add them here ourselves.
    if (CMAKE_VERSION VERSION_LESS 2.8.11)
        include_directories("${gtest_SOURCE_DIR}/include")
    endif()
endif()

add_subdirectory(tests)

testBuilder_add_source(darcs_patch_exe src/darcs_exe.cpp)
testBuilder_add_library(darcs_patch_exe darcs_patch)
testBuilder_build(darcs_patch_exe EXECUTABLES)

set(DARCS_PATCH_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/include PARENT_SCOPE)
