#
# Include subdirectories.
#

add_subdirectory(main/rif)

if(BUILD_TESTS)
  add_subdirectory(test/rif)
endif(BUILD_TESTS)