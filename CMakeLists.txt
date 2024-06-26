cmake_minimum_required(VERSION 3.22)
project(initial-setup
    VERSION "0.0.1"
    DESCRIPTION "Initial Setup Tool"
    LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(APPLICATION_ID "dev.rlxos.InitialSetup")
string(REPLACE "." "/" APPLICATION_PREFIX "/${APPLICATION_ID}/")
list(INSERT CMAKE_MODULE_PATH 0 ${PROJECT_SOURCE_DIR}/cmake)

set(GENERATED_FILES_DIR ${PROJECT_BINARY_DIR}/generated)
set(GENERATED_SOURCES_DIR ${GENERATED_FILES_DIR}/src)
set(GENERATED_DATA_DIR ${GENERATED_FILES_DIR}/data)

configure_file(config.hxx.in ${GENERATED_SOURCES_DIR}/config.hxx)
include_directories(${GENERATED_SOURCES_DIR})

include(GLibUtils)
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM gtkmm-3.0 IMPORTED_TARGET REQUIRED)

add_subdirectory(src)
add_subdirectory(data)