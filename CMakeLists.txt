CMAKE_MINIMUM_REQUIRED( VERSION 2.8 )
PROJECT(load_balance)

set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/install)

find_package(MPI REQUIRED)

if (NOT ${MPI_CXX_FOUND})
  message("\nERROR: MPI_CXX NOT FOUND\n")
else()
  message("\nMPI_INCLUDE_PATH: " ${MPI_INCLUDE_PATH} )
  message("MPI_LIBRARIES: " ${MPI_LIBRARIES} )
  message("")
endif()

find_package(Boost REQUIRED COMPONENTS mpi program_options serialization )
if (NOT ${BOOST_FOUND})
  message("\nERROR: BOOST not found.  Set BOOST_ROOT\n")
else()
  message("\nBoost_INCLUDE_DIR: " ${Boost_INCLUDE_DIR} )
  message("Boost_LIBRARIES:    " ${Boost_LIBRARIES} )
  message("")
endif()


ADD_SUBDIRECTORY( src)

# Make dist
set(CPACK_SOURCE_GENERATOR "TGZ")
set(CPACK_SOURCE_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}")
set(CPACK_SOURCE_IGNORE_FILES "/build/;${CPACK_SOURCE_IGNORE_FILES}")
include(CPack)
add_custom_target(dist COMMAND ${CMAKE_MAKE_PROGRAM} package_source)
