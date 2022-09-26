cmake_minimum_required(VERSION 3.4.3)
project(pm-wrapper-compiler)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ===============================================================================
# 1. LOAD LLVM CONFIGURATION
# ===============================================================================
# Set this to a valid LLVM installation dir
set(CT_Clang_INSTALL_DIR "" CACHE PATH "LLVM installation directory")

# Add the location of ClangConfig.cmake to CMake search paths (so that
# find_package can locate it)
list(APPEND CMAKE_PREFIX_PATH "${CT_Clang_INSTALL_DIR}/lib/cmake/clang/")

find_package(Clang REQUIRED CONFIG)

# PmWrapperClangPlugin includes headers from LLVM and Clang - update the include paths
# accordingly
include_directories(SYSTEM "${LLVM_INCLUDE_DIRS};${CLANG_INCLUDE_DIRS}")

# ===============================================================================
# 2. LLVM-TUTOR BUILD CONFIGURATION
# ===============================================================================
# Use the same C++ standard as LLVM does
set(CMAKE_CXX_STANDARD 14 CACHE STRING "")

# LLVM is normally built without RTTI. Be consistent with that.
if(NOT LLVM_ENABLE_RTTI)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")
endif()

# ===============================================================================
# 3. ADD THE TARGET
# ===============================================================================
file(GLOB SOURCES "./*.cpp")

# add_library(PmWrapperClangPlugin SHARED ${SOURCES})
# add_executable(PmWrapperClangPlugin ${SOURCES})

# Allow undefined symbols in shared objects on Darwin (this is the default
# behaviour on Linux)
# target_link_libraries(PmWrapperClangPlugin
# "$<$<PLATFORM_ID:Darwin>:-undefined dynamic_lookup>" "stdc++fs")
add_executable(
  PmWrapperCompiler
  ${SOURCES}
)

# Link with libclangTooling
target_link_libraries(
  PmWrapperCompiler
  "$<$<PLATFORM_ID:Darwin>:-undefined dynamic_lookup>" "stdc++fs"
  clangTooling
)