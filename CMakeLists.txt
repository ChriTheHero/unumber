cmake_minimum_required(VERSION 3.22)
cmake_policy(SET CMP0144 NEW)
project(unumber VERSION 0.1.0 LANGUAGES CXX)

include_directories(${unumber_SOURCE_DIR})
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

find_package(GMP REQUIRED)
include(CMakePackageConfigHelpers)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0" CACHE STRING "Debug flags" FORCE)
set(CMAKE_C_FLAGS_DEBUG "-g -O0" CACHE STRING "Debug flags" FORCE)

file(GLOB_RECURSE UNUMBER_HEADERS CONFIGURE_DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/include/unumber/*.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/unumber/*.hpp
)
file(GLOB_RECURSE UNUMBER_SOURCES CONFIGURE_DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/src/unumber/*.cpp
)

add_library(unumber STATIC ${UNUMBER_SOURCES} ${UNUMBER_HEADERS})
add_library(usub::unumber ALIAS unumber)

target_include_directories(unumber
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/unumber>
)

target_link_libraries(unumber
        PUBLIC
        PUBLIC ${GMP_LIBRARIES}
        PUBLIC ${GMPXX_LIBRARIES}
)

set_target_properties(unumber PROPERTIES
        EXPORT_NAME unumber
        VERSION ${PROJECT_VERSION}
        SOVERSION ${PROJECT_VERSION_MAJOR}
)

option(UNUMBER_BUILD_EXAMPLES "Build UNUMBER example executables" OFF)
if (UNUMBER_BUILD_EXAMPLES)
    add_executable(unumber_example examples/main.cpp)
    target_link_libraries(unumber_example PRIVATE unumber)
    target_compile_definitions(unumber_example PRIVATE DEV_STAGE=${DEV_STAGE})
endif ()

install(TARGETS unumber
        EXPORT unumberTargets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/unumber
)

install(DIRECTORY include/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/unumber
        FILES_MATCHING
        PATTERN "*.h"
        PATTERN "*.hpp"
)

configure_package_config_file(
        cmake/unumberConfig.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/unumberConfig.cmake
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/unumber
        NO_CHECK_REQUIRED_COMPONENTS_MACRO
)

write_basic_package_version_file(
        ${CMAKE_CURRENT_BINARY_DIR}/unumberConfigVersion.cmake
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion
)

install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/unumberConfig.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/unumberConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/unumber
)

install(EXPORT unumberTargets
        NAMESPACE unumber::
        FILE unumberTargets.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/unumber
)