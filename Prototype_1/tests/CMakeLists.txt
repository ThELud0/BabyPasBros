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
target_link_libraries(tests GTest::gtest_main lib_simpleGame sfml-graphics pugixml sfml-audio)

add_custom_target(copy-resources-tests ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/resources)
file(GLOB RESOURCES CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/resources/*.*)
add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/resources
                   DEPENDS ${CMAKE_SOURCE_DIR}/resources
                   COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/resources
                   COMMAND ${CMAKE_COMMAND} -E copy_if_different
                           ${RESOURCES}
                           ${CMAKE_CURRENT_BINARY_DIR}/resources)
add_dependencies(tests copy-resources-tests)

# The next line enables CMake’s test runner to discover the tests included in the binary,
# using the GoogleTest CMake module (which was included in root CMakeLists.txt).
gtest_discover_tests(tests)
