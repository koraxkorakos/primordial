enable_testing()
include(GoogleTest)
find_package(GTest REQUIRED)

add_library(testsupport demangle.cpp)

add_executable(test_primordial test_prime.cpp test_nq.cpp test_details.cpp test_powermap.cpp test_meta.cpp)
target_link_libraries(test_primordial PRIVATE testsupport primordial GTest::gtest GTest::gtest_main GTest::gmock)

gtest_discover_tests(test_primordial)
