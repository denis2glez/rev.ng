find_package(GTest REQUIRED)

# For every source file '<name>.cpp' we create a '<name>' test target.
file(GLOB SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp )

foreach (FILE_NAME ${SOURCES})
    string(REPLACE ".cpp" "" TEST_NAME "${FILE_NAME}")
    # Create a test
    add_executable(${TEST_NAME} ${FILE_NAME})
    target_link_libraries(${TEST_NAME} PRIVATE GTest::GTest GTest::gtest_main math)

    # install(TARGETS ${TEST_NAME} DESTINATION "tests")
endforeach ()