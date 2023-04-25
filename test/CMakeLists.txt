set(TEST_FILES
	main_test.cpp
    matrix_test.cpp
    mdarray_test.cpp
    reduce_test.cpp
    matmul_test.cpp
    transpose_test.cpp
)

find_package(GTest REQUIRED)
# include_directories(${GTEST_INCLUDE_DIRS})

add_executable(ExpressionTemplate-test ${TEST_FILES})
target_link_libraries(ExpressionTemplate-test GTest::gtest_main pthread
        ExpressionTemplate)
GTEST_ADD_TESTS(ExpressionTemplate-test "" ${TEST_FILES})
if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        target_compile_options(ExpressionTemplate-test PRIVATE -fsanitize=address)
        target_link_options(ExpressionTemplate-test PRIVATE -fsanitize=address)
endif()

find_package(Boost REQUIRED)
find_package(Eigen3 REQUIRED)
add_executable(perf-check perf_test.cpp)
target_link_libraries(perf-check PUBLIC ExpressionTemplate Eigen3::Eigen Boost::boost)
