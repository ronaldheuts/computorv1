cmake_minimum_required(VERSION 3.16)

project(computorv1)

add_executable(computorv1)

add_subdirectory(src)

# compile configuration

add_library(compiler_flags INTERFACE)

target_compile_features(compiler_flags INTERFACE cxx_std_14)

set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:ARMClang,AppleClang,Clang,GNU,LCC>")
set(msvc_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")

target_compile_options(compiler_flags INTERFACE 
  "$<${gcc_like_cxx}:$<BUILD_INTERFACE:-Wall;-Wextra;-Wshadow;-Wformat=2;-Wunused>>"
  "$<${msvc_cxx}:$<BUILD_INTERFACE:-W3>>")
  
include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY "https://github.com/google/googletest"
  GIT_TAG main
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

enable_testing()

add_subdirectory(tests)


