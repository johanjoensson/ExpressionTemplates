set(TEST_FILES
	main_test.cpp
    matrix_test.cpp
    mdarray_test.cpp
)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(expr-template-test ${TEST_FILES})
target_link_libraries(expr-template-test ${GTEST_LIBRARIES} pthread
    expr-template-lib)
GTEST_ADD_TESTS(expr-template-test "" ${TEST_FILES})

find_package(Eigen3 REQUIRED)
add_executable(perf-check perf_test.cpp)
target_link_libraries(perf-check PUBLIC expr-template-lib Eigen3::Eigen)

add_executable(clang-crash clang_crash.cpp)
target_link_libraries(clang-crash expr-template-lib)