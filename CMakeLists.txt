cmake_minimum_required(VERSION 3.14)

project(
    ExpressionTemplate
	VERSION 0.1
	DESCRIPTION "C++ 23 expression template library"
	LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)

set(MDSPAN_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/external/mdspan/include/)


if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(CLANGBUG On)
    add_compile_options(-DCLANG_COMPILER)
endif()

if(DEFINED CLANGBUG)
    add_compile_options(-DCLANGBUG -DMDSPAN_USE_PAREN_OPERATOR)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    include(CheckIPOSupported)
    check_ipo_supported(RESULT ipo_supported OUTPUT error)
    if( supported )
        set_propoerty(TARGET expr-template-lib INTERPROCEDURAL_OPTIMIZATION
            TRUE)
    endif()
endif()
add_compile_options(-Werror -Wall -Wextra -pedantic -Wshadow -Wnon-virtual-dtor -Wold-style-cast -Wcast-align -Wunused -Woverloaded-virtual -Wpedantic -Wconversion -Wsign-conversion -Wmisleading-indentation -Wnull-dereference -Wdouble-promotion -Wformat=2 -Weffc++)
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU") 
    add_compile_options(-Wduplicated-cond -Wduplicated-branches -Wlogical-op
        -Wuseless-cast)
endif()

message( STATUS "-------- BUILD-TYPE: ${CMAKE_BUILD_TYPE} -------------")

add_library(expr-template-lib INTERFACE)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_options(-Ofast -march=native)
endif()

target_include_directories(expr-template-lib INTERFACE 
	$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:include>
)
target_include_directories(expr-template-lib SYSTEM INTERFACE
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
