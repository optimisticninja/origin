cmake_minimum_required(VERSION 3.1)

project(origin CXX)

# Dependencies
find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)
find_package(Boost COMPONENTS program_options REQUIRED)

# Compiler flags
set(WARNINGS "-Wall -Werror -Wextra")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${WARNINGS}")
include_directories(${Boost_INCLUDE_DIR})

# Directories
set(SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set(GENERATED_DIR ${SOURCE_DIR}/generated)
set(TEST_DIR ${PROJECT_SOURCE_DIR}/tests)

# Create src/generated for flex/bison
add_custom_target(create-generated ALL COMMAND ${CMAKE_COMMAND} -E make_directory ${GENERATED_DIR})

BISON_TARGET(parser ${SOURCE_DIR}/grammar/parser.yy ${GENERATED_DIR}/parser.tab.cc)
FLEX_TARGET(lexer ${SOURCE_DIR}/grammar/lexer.l ${GENERATED_DIR}/lexer.yy.cc)
ADD_FLEX_BISON_DEPENDENCY(lexer parser create-generated)

# Origin binary
add_executable(origin tags 
	${SOURCE_DIR}/main.cpp
	${SOURCE_DIR}/grammar/driver.cpp
	${SOURCE_DIR}/compilerargs.cpp
	${FLEX_lexer_OUTPUTS} 
	${BISON_parser_OUTPUTS})
set_property(TARGET origin PROPERTY CXX_STANDARD 17)
target_link_libraries(origin LINK_PUBLIC ${Boost_LIBRARIES})

# Ctags
set_source_files_properties(tags PROPERTIES GENERATED true)
add_custom_command(OUTPUT tags
	COMMAND ctags -R --c++-kinds=+p --fields=+iaS --extra=+q . 
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

# Google Test ##################################################
# Download
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
	RESULT_VARIABLE result
	WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
if(result)
	message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
	RESULT_VARIABLE result
	WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
if(result)
	message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines the gtest and gtest_main targets.
add_subdirectory(${CMAKE_BINARY_DIR}/googletest-src
                 ${CMAKE_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)

# The gtest/gtest_main targets carry header search path dependencies automatically when using 
# CMake 2.8.11 or later. Otherwise we have to add them here ourselves.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
  include_directories("${gtest_SOURCE_DIR}/include")
endif()

# Now simply link against gtest or gtest_main as needed. Eg
add_executable(tests ${TEST_DIR}/ast.cpp)
target_link_libraries(tests gtest_main)
add_test(NAME ast/ast COMMAND tests)
# End Google Test ################################################

# Special cleanup
set_property(DIRECTORY APPEND PROPERTY ADDITIONAL_MAKE_CLEAN_FILES 
	${GENERATED_DIR} 
	${CMAKE_SOURCE_DIR}/tags)
