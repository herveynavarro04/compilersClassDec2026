cmake_minimum_required(VERSION 3.16)
project(BabyDuck LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---------- Find Flex and Bison ----------
find_package(BISON 3.0 REQUIRED)
find_package(FLEX 2.6 REQUIRED)

# ---------- Generate the parser and the scanner ----------
# Outputs go to the build folder with a .cpp extension so they compile as C++.
BISON_TARGET(Parser
    ${CMAKE_CURRENT_SOURCE_DIR}/parser.y
    ${CMAKE_CURRENT_BINARY_DIR}/parser.tab.cpp
    DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/parser.tab.h)

FLEX_TARGET(Lexer
    ${CMAKE_CURRENT_SOURCE_DIR}/lexer.l
    ${CMAKE_CURRENT_BINARY_DIR}/lex.yy.cpp)

# The scanner includes parser.tab.h, so Bison must run first
ADD_FLEX_BISON_DEPENDENCY(Lexer Parser)

# ---------- Executable ----------
add_executable(babyduck
    main.cpp
    semantic_cube/semantic_cube.cpp
    ${BISON_Parser_OUTPUTS}
    ${FLEX_Lexer_OUTPUTS})

# So main.cpp and lex.yy.cpp can find the generated parser.tab.h
target_include_directories(babyduck PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_compile_options(babyduck PRIVATE -Wall)

# ---------- Test plan (run with: ctest --output-on-failure) ----------
enable_testing()

# Valid programs: must be ACCEPTED (exit code 0)
file(GLOB VALID_TESTS ${CMAKE_CURRENT_SOURCE_DIR}/tests/valid/*.bd)
foreach(f ${VALID_TESTS})
    get_filename_component(name ${f} NAME_WE)
    add_test(NAME valid_${name} COMMAND babyduck ${f})
endforeach()

# Invalid programs: must be REJECTED (exit code != 0)
file(GLOB INVALID_TESTS ${CMAKE_CURRENT_SOURCE_DIR}/tests/invalid/*.bd)
foreach(f ${INVALID_TESTS})
    get_filename_component(name ${f} NAME_WE)
    add_test(NAME invalid_${name} COMMAND babyduck ${f})
    set_tests_properties(invalid_${name} PROPERTIES WILL_FAIL TRUE)
endforeach()