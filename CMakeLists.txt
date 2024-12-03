cmake_minimum_required (VERSION 3.8)
project (Memdb)

set (CMAKE_CXX_STANDARD 23)
set (CMAKE_CXX_STANDARD_REQUIRED True)

option(SetDebugBuildType "Set on if want not to build as release and use O2" OFF)

if (NOT SetDebugBuildType)
    set(CMAKE_BUILD_TYPE Release)
    add_compile_options(-O2)
endif()

#add_executable(Memdb memdb/internal/dataBase/db.cpp)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/f8d7d77c06936315286eb55f8de22cd23c188571.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()
include(GoogleTest)

add_subdirectory(memdb/internal/type)
add_subdirectory(memdb/internal/column)
add_subdirectory(memdb/internal/table)
add_subdirectory(memdb/internal/parser)
add_subdirectory(memdb/internal/dataBase)