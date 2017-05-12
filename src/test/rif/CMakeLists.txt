#
# Set runner.
#

set(RIF_TEST_GENERIC_RUNNER_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../support/generic_runner.cc")

#
# Set tets files.
#

# Base

set(${PROJECT_NAME}_TEST_BASE_OBJECTS

    ${RIF_TEST_GENERIC_RUNNER_PATH}

    base/support/pool_conformity.cc

    base/test_bool.cc
    base/test_double.cc
    base/test_int.cc
    base/test_null.cc
    base/test_pair.cc
    base/test_string.cc
    base/test_val.cc

    base/test_paged_pool.cc

)

add_executable("${PROJECT_NAME}_test_base" ${${PROJECT_NAME}_TEST_BASE_OBJECTS})
target_link_libraries("${PROJECT_NAME}_test_base" ${PROJECT_NAME}_static gtest gtest_main)
add_test("${PROJECT_NAME}_test_base" "${EXECUTABLE_OUTPUT_PATH}/${PROJECT_NAME}_test_base")

# Collection

set(${PROJECT_NAME}_TEST_COLLECTION_OBJECTS

    ${RIF_TEST_GENERIC_RUNNER_PATH}

    collection/support/list_conformity.cc
    collection/support/map_conformity.cc

    collection/test_arraylist.cc
    collection/test_hashmap.cc
    collection/test_linkedlist.cc

)

add_executable("${PROJECT_NAME}_test_collection" ${${PROJECT_NAME}_TEST_COLLECTION_OBJECTS})
target_link_libraries("${PROJECT_NAME}_test_collection" ${PROJECT_NAME}_static gtest gtest_main)
add_test("${PROJECT_NAME}_test_collection" "${EXECUTABLE_OUTPUT_PATH}/${PROJECT_NAME}_test_collection")

# Concurrent

set(${PROJECT_NAME}_TEST_CONCURRENT_OBJECTS

    ${RIF_TEST_GENERIC_RUNNER_PATH}

    base/support/pool_conformity.cc

    concurrent/test_concurrent_pool.cc

)

add_executable("${PROJECT_NAME}_test_concurrent" ${${PROJECT_NAME}_TEST_CONCURRENT_OBJECTS})
target_link_libraries("${PROJECT_NAME}_test_concurrent" ${PROJECT_NAME}_static gtest gtest_main)
add_test("${PROJECT_NAME}_test_concurrent" "${EXECUTABLE_OUTPUT_PATH}/${PROJECT_NAME}_test_concurrent")

# Util

set(${PROJECT_NAME}_TEST_UTIL_OBJECTS

    ${RIF_TEST_GENERIC_RUNNER_PATH}

    util/test_alloc.cc
    util/test_version.cc

)

add_executable("${PROJECT_NAME}_test_util" ${${PROJECT_NAME}_TEST_UTIL_OBJECTS})
target_link_libraries("${PROJECT_NAME}_test_util" ${PROJECT_NAME}_static gtest gtest_main)
add_test("${PROJECT_NAME}_test_util" "${EXECUTABLE_OUTPUT_PATH}/${PROJECT_NAME}_test_util")