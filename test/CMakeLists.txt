enable_testing()
include(GoogleTest)
find_package(GTest REQUIRED)

add_executable(test_quot test_quot.cpp demangle.cpp)
target_link_libraries(test_quot PRIVATE ctquot GTest::gtest GTest::gtest_main)

add_executable(test_prime test_prime demangle.cpp)
target_link_libraries(test_prime PRIVATE ctquot GTest::gtest GTest::gtest_main)

gtest_discover_tests(test_quot)
gtest_discover_tests(test_prime)
