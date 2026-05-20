include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.14.0
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

option(ENABLE_COVERAGE "Add gcov instrumentation to test objects" OFF)

if(ENABLE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(shopping_obj_test PRIVATE --coverage -O0 -g)
    target_link_options(shopping_core_test PUBLIC --coverage)
endif()

add_executable(unit_tests
    test_complaint.cpp
    test_person_flows.cpp
    test_admin_employee.cpp
    test_thread_smoke.cpp
    test_server_thread.cpp
)
target_include_directories(unit_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(unit_tests PRIVATE shopping_core_test GTest::gtest_main ws2_32)
target_compile_definitions(unit_tests PRIVATE OSHOP_TEST_BUILD)

if(ENABLE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_link_options(unit_tests PRIVATE --coverage)
endif()

include(GoogleTest)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/test_workdir)

gtest_discover_tests(unit_tests PROPERTIES WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/test_workdir)
