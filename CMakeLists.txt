project(origin CXX)
cmake_minimum_required(VERSION 3.1)

find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)
find_package(Boost COMPONENTS program_options REQUIRED)

set(WARNINGS "-Wall -Werror -Wextra")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${WARNINGS}")

set(SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set(GENERATED_DIR ${SOURCE_DIR}/generated)

add_custom_target(create-generated ALL COMMAND ${CMAKE_COMMAND} -E make_directory ${GENERATED_DIR})
BISON_TARGET(parser ${SOURCE_DIR}/parser.yy ${GENERATED_DIR}/parser.tab.cc)
FLEX_TARGET(lexer ${SOURCE_DIR}/lexer.l ${GENERATED_DIR}/lexer.yy.cc)
ADD_FLEX_BISON_DEPENDENCY(lexer parser create-generated)

add_executable(origin tags 
	${SOURCE_DIR}/main.cpp
	${SOURCE_DIR}/driver.cpp
	${SOURCE_DIR}/compilerargs.cpp
	${FLEX_lexer_OUTPUTS} 
	${BISON_parser_OUTPUTS})
set_property(TARGET origin PROPERTY CXX_STANDARD 17)
set_source_files_properties(tags PROPERTIES GENERATED true)
add_custom_command(OUTPUT tags
	COMMAND ctags -R --c++-kinds=+p --fields=+iaS --extra=+q . 
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
set_property(DIRECTORY APPEND PROPERTY ADDITIONAL_MAKE_CLEAN_FILES 
	${GENERATED_DIR} 
	${CMAKE_SOURCE_DIR}/tags)
include_directories(${Boost_INCLUDE_DIR})
target_link_libraries(origin LINK_PUBLIC ${Boost_LIBRARIES})

