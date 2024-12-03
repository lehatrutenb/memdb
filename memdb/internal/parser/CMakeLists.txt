#add_library(parser lexer.cpp assignments.cpp condition.cpp createColumns.cpp insertIndex.cpp selectValues.cpp parser.cpp)
add_library(parser lexer.cpp assignments.cpp condition.cpp createColumns.cpp insertIndex.cpp selectValues.cpp parser.cpp)
#add_executable(parser lexer.cpp)
add_dependencies(parser type column table)

option(EnableCoverage "Set on if want to collect coverage info" OFF)

if (EnableCoverage)
    SET(GCC_COVERAGE_COMPILE_FLAGS "-fprofile-arcs -ftest-coverage")
    SET(GCC_COVERAGE_LINK_FLAGS "-lgcov")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")
endif()

#add_executable(Memdb_parserTest test.cpp)
#target_link_libraries(Memdb_parserTest GTest::gtest_main)

#gtest_discover_tests(Memdb_parserTest)