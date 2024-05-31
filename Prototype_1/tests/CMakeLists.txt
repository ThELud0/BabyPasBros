#
# Lines inspired by https://google.github.io/googletest/quickstart-cmake.html
#

# Note: include(GoogleTest)and enable_testing() already done in top CMakeLists.txt

add_executable(tests
        RoundTargetTest.cpp
)
target_include_directories(tests
        PRIVATE
        ../src/Include
)
target_link_libraries(tests GTest::gtest_main lib_simpleGame sfml-graphics)

# The next line enables CMake’s test runner to discover the tests included in the binary,
# using the GoogleTest CMake module (which was included in root CMakeLists.txt).
gtest_discover_tests(tests)
