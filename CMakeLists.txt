cmake_minimum_required(VERSION 3.11) # FetchContent is available in 3.11+
project(dialog_text_language)

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Dependencies for raylib
set(RAYLIB_VERSION 4.0)
find_package(raylib ${RAYLIB_VERSION} REQUIRED)
if (NOT raylib_FOUND) # If there's none, fetch and build raylib
  include(FetchContent)
  FetchContent_Declare(
    raylib
    DOWNLOAD_EXTRACT_TIMESTAMP OFF
    URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
  )
  FetchContent_GetProperties(raylib)
  if (NOT raylib_POPULATED) # Have we downloaded raylib yet?
    set(FETCHCONTENT_QUIET NO)
    FetchContent_Populate(raylib)
    set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
    add_subdirectory(${raylib_SOURCE_DIR} ${raylib_BINARY_DIR})
  endif()
endif()

# Configure Flex and Byacc
find_program(FLEX_PROGRAM flex)
find_program(BYACC_PROGRAM byacc)

# Our Project

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/generated-code")

add_custom_target(LEXER ALL DEPENDS
  "${CMAKE_CURRENT_BINARY_DIR}/generated-code/lexer.c"
  "${CMAKE_CURRENT_BINARY_DIR}/generated-code/y.tab.c"
  "${CMAKE_CURRENT_BINARY_DIR}/generated-code/y.tab.h")
add_custom_command(
  COMMAND ${FLEX_PROGRAM} --outfile=${CMAKE_CURRENT_BINARY_DIR}/generated-code/lexer.c ${CMAKE_CURRENT_SOURCE_DIR}/other/lexicon.lex
  OUTPUT  ${CMAKE_CURRENT_BINARY_DIR}/generated-code/lexer.c
  MAIN_DEPENDENCY "${CMAKE_CURRENT_SOURCE_DIR}/other/lexicon.lex"
  COMMENT "Generating lexicon")
add_custom_command(
  COMMAND ${BYACC_PROGRAM} -d ${CMAKE_CURRENT_SOURCE_DIR}/other/grammer.y
  OUTPUT  ${CMAKE_CURRENT_BINARY_DIR}/generated-code/y.tab.c ${CMAKE_CURRENT_BINARY_DIR}/generated-code/y.tab.h
  MAIN_DEPENDENCY "${CMAKE_CURRENT_SOURCE_DIR}/other/grammer.y"
  WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/generated-code/"
  COMMENT "Generating grammer rules")

file(GLOB SOURCE_FILES "src/*.c" )
include_directories( "${CMAKE_SOURCE_DIR}/header-onlys/" )
include_directories( "${CMAKE_CURRENT_BINARY_DIR}/generated-code/" )

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
#set(raylib_VERBOSE 1)
target_link_libraries(${PROJECT_NAME} raylib)
target_link_libraries (${PROJECT_NAME} m)

# Web Configurations
if ("${PLATFORM}" STREQUAL "Web")
    # Tell Emscripten to build an example.html file.
    set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".html")
endif()

# Checks if OSX and links appropriate frameworks (Only required on MacOS)
if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif()
