cmake_minimum_required(VERSION 2.8)
project(solcore C)
set(${PROJECT_NAME}_SOLAR_DIRECTORY "${CMAKE_INSTALL_PREFIX}/lib/sol/${PROJECT_NAME}.solar")

include_directories("${CMAKE_INSTALL_PREFIX}/include")
add_definitions(-std=c99)

if(${UNIX})
    add_definitions(-D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64 -D_POSIX_C_SOURCE=200112)
endif()

if (${CMAKE_C_COMPILER_ID} STREQUAL "Clang")
    set(CMAKE_SHARED_LINKER_FLAGS "-undefined dynamic_lookup")
endif()

set(${PROJECT_NAME}_NATIVES
    Object.c
    Number.c
    List.c
    String.c
    Function.c
    Event.c)
set(${PROJECT_NAME}_SOL_BINARIES
    solcore.solbin)

add_custom_target(descriptor SOURCES descriptor.yml)

foreach(NATIVE ${${PROJECT_NAME}_NATIVES})
    get_filename_component(NATIVE_NAME ${NATIVE} NAME_WE)
    add_library(${NATIVE_NAME} SHARED ${NATIVE})
    set(${PROJECT_NAME}_NATIVE_LIBS ${${PROJECT_NAME}_NATIVE_LIBS} ${NATIVE_NAME})
endforeach()

install(CODE "execute_process(COMMAND rm -rf ${${PROJECT_NAME}_SOLAR_DIRECTORY})")
install(FILES descriptor.yml DESTINATION ${${PROJECT_NAME}_SOLAR_DIRECTORY})
install(TARGETS ${${PROJECT_NAME}_NATIVE_LIBS} LIBRARY DESTINATION ${${PROJECT_NAME}_SOLAR_DIRECTORY}/natives)
install(FILES ${${PROJECT_NAME}_SOL_BINARIES} DESTINATION ${${PROJECT_NAME}_SOLAR_DIRECTORY}/binaries)
