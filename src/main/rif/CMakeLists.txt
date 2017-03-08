#
# Submodules
#

# Base

set(${PROJECT_NAME}_BASE_OBJECTS

    base/rif_bool.c
    base/rif_double.c
    base/rif_int.c
    base/rif_null.c
    base/rif_pair.c
    base/rif_string.c
    base/rif_val.c

)

add_library("${PROJECT_NAME}_base" OBJECT ${${PROJECT_NAME}_BASE_OBJECTS})

# Base

set(${PROJECT_NAME}_COLLECTION_OBJECTS

    collection/rif_list.c
    collection/rif_map.c

    collection/rif_arraylist.c
    collection/rif_arraylist_hooks.c
    collection/rif_arraylist_iterator.c
    collection/rif_arraylist_iterator_hooks.c

    collection/rif_hashmap.c
    collection/rif_hashmap_hooks.c
    collection/rif_hashmap_iterator.c
    collection/rif_hashmap_iterator_hooks.c

    collection/rif_iterator.c
    collection/rif_linkedlist.c
    collection/rif_linkedlist_hooks.c
    collection/rif_linkedlist_iterator.c
    collection/rif_linkedlist_iterator_hooks.c

)

add_library("${PROJECT_NAME}_collection" OBJECT ${${PROJECT_NAME}_COLLECTION_OBJECTS})

# Util

set(${PROJECT_NAME}_UTIL_OBJECTS

    util/rif_alloc.c
    util/rif_hash.c
    util/rif_version.c

)

add_library("${PROJECT_NAME}_util" OBJECT ${${PROJECT_NAME}_UTIL_OBJECTS})

#
# Set build options.
#

option(BUILD_STATIC "whether or not to build ${PROJECT_NAME} as a static library" ON)
option(BUILD_SHARED "whether or not to build ${PROJECT_NAME} as a shared library" ON)

#
# Set shared linker flags.
#

set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fPIC")

#
# List library objects.
#

set(RIF_OBJECTS

    $<TARGET_OBJECTS:rif_base>
    $<TARGET_OBJECTS:rif_collection>
    $<TARGET_OBJECTS:rif_util>

)

#
# Use tcmalloc.
#

#option(USE_TCMALLOC "Use Google Tcmalloc library" ON)
#if(USE_TCMALLOC)
#  find_package(Tcmalloc)
#endif(USE_TCMALLOC)

#
# Create libraries.
#

# Static library.

if(BUILD_STATIC)
  add_library(${PROJECT_NAME}_static STATIC ${RIF_OBJECTS})
  set_target_properties(${PROJECT_NAME}_static PROPERTIES OUTPUT_NAME ${PROJECT_NAME})
  target_link_libraries(${PROJECT_NAME}_static tinycthread)
#  target_link_libraries(${PROJECT_NAME}_static ${Tcmalloc_LIBRARIES})
endif(BUILD_STATIC)

# Shared library.

if(BUILD_SHARED)
  add_library(${PROJECT_NAME}_shared SHARED ${RIF_OBJECTS})
  set_target_properties(${PROJECT_NAME}_shared PROPERTIES OUTPUT_NAME ${PROJECT_NAME})
  target_link_libraries(${PROJECT_NAME}_shared tinycthread)
#  target_link_libraries(${PROJECT_NAME}_static ${Tcmalloc_LIBRARIES})
endif(BUILD_SHARED)
