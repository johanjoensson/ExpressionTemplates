cmake_minimum_required(VERSION 3.14)

project(
        ExpressionTemplate
        VERSION 0.1
        DESCRIPTION "C++ 23 multidimensional expression template library"
        LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

add_library(ExpressionTemplate INTERFACE)
find_package(mdspan REQUIRED HINTS "external/mdspan/install/lib64/cmake/")
target_link_libraries(ExpressionTemplate INTERFACE std::mdspan)


if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        set(CLANGBUG On)
        add_compile_options(-DCLANG_COMPILER)
endif()

if(DEFINED CLANGBUG)
        add_compile_options(-DCLANGBUG -DMDSPAN_USE_PAREN_OPERATOR)
endif()

add_compile_options(-Werror -Wall -Wextra -pedantic -Wshadow -Wnon-virtual-dtor -Wold-style-cast -Wcast-align -Wunused -Woverloaded-virtual -Wpedantic -Wconversion -Wsign-conversion -Wmisleading-indentation -Wnull-dereference -Wdouble-promotion -Wformat=2 -Weffc++)
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU") 
        add_compile_options(-Wduplicated-cond -Wduplicated-branches -Wlogical-op
                -Wuseless-cast)
endif()

message( STATUS "-------- BUILD-TYPE: ${CMAKE_BUILD_TYPE} -------------")

if(CMAKE_BUILD_TYPE STREQUAL "Release")
        add_compile_options(-Ofast -march=native)
endif()

target_include_directories(ExpressionTemplate INTERFACE 
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)
target_include_directories(ExpressionTemplate SYSTEM INTERFACE
        ${MDSPAN_INCLUDE_DIR}
)

# Only do these if this is the main project, and not if it is included through add_subdirectory
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
        set(CMAKE_CXX_EXTENSIONS OFF)
        set_property(GLOBAL PROPERTY USE_FOLDERS ON)

        set(WFLAGS -Werror -Wall -Wextra -pedantic -Wshadow -Wnon-virtual-dtor
                -Wold-style-cast -Wcast-align -Wunused -Woverloaded-virtual -Wpedantic
                -Wconversion -Wsign-conversion -Wmisleading-indentation
                -Wnull-dereference -Wdouble-promotion -Wformat=2 -Weffc++)
        if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
                set(WFLAGS ${WFLAGS} -Wduplicated-cond -Wduplicated-branches
                        -Wlogical-op -Wuseless-cast)
        endif()
endif() # Main project only part ends here

if(BUILD_DOCS)
        find_package(Doxygen)
        if(Doxygen_FOUND)
                add_subdirectory(docs)
        else()
                message(STATUS "Doxygen not found, unable to build docs")
        endif()
endif()

# IDEs should put the headers in a nice place
source_group(
        TREE "${PROJECT_SOURCE_DIR}/include"
        PREFIX "Header Files"
        FILES ${HEADER_LIST}
)

if((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME OR MODERN_CMAKE_BUILD_TESTING)
    AND BUILD_TESTING)
        enable_testing()
        add_subdirectory(test)
endif()

install(DIRECTORY include/ DESTINATION include/expression_template 
	FILES_MATCHING PATTERN "*.h")
install(TARGETS ExpressionTemplate
        EXPORT ExpressionTemplate-targets
	INCLUDES DESTINATION include/expression_template)
install(EXPORT ExpressionTemplate-targets
        NAMESPACE ExprTemp::
	DESTINATION share/expression_template
	)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/expression_template/expression_template/-config-version.cmake"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY AnyNewerVersion
)
export(EXPORT ExpressionTemplate-targets
  FILE "${CMAKE_CURRENT_BINARY_DIR}/expression_template/expression_template-targets.cmake"
  NAMESPACE ExprTemp::
)
configure_file(cmake/expression_template-config.cmake
  "${CMAKE_CURRENT_BINARY_DIR}/expression_template/expression_template-config.cmake"
  COPYONLY
)

set(ConfigPackageLocation share/cmake/expression_template)
install(EXPORT ExpressionTemplate-targets
  FILE
   expression_template-targets.cmake
  NAMESPACE
  ExprTemp::
  DESTINATION
    ${ConfigPackageLocation}
)
install(
  FILES
    cmake/expression_template-config.cmake
    "${CMAKE_CURRENT_BINARY_DIR}/expression_template/expression_template-config-version.cmake"
  DESTINATION
    ${ConfigPackageLocation}
  COMPONENT
    Devel
)
