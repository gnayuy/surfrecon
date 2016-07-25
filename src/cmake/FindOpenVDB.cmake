# - find OpenVDB
#
# Find the OpenVDB includes and libraries
# This module defines
#  OpenVDB_INCLUDE_DIRS - where to find openvdb includes.
#  OpenVDB_LIBRARIES    - List of libraries when using openvdb.
#  OpenVDB_FOUND        - True if openvdb found.

# Look for the header file.
FIND_PATH( OPENVDB_INCLUDE_DIR NAMES openvdb/openvdb.h)

# Look for the library.
FIND_LIBRARY( OPENVDB_LIBRARY NAMES openvdb)

# handle the QUIETLY and REQUIRED arguments and set OPENVDB_FOUND to TRUE if
# all listed variables are TRUE
INCLUDE( FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS( OPENVDB DEFAULT_MSG OPENVDB_LIBRARY OPENVDB_INCLUDE_DIR)

# Half
FIND_LIBRARY(HALF_LIBRARIES
    NAMES Half libHalf
    PATHS
    ${GMP_ROOT}/lib
    /usr/lib
    /usr/local/lib
    /usr/lib/Half
    /usr/lib64
    /usr/local/lib64
    /usr/lib64/Half
    $ENV{ProgramFiles}/Half/*/lib
    $ENV{SystemDrive}/Half/*/lib
    )

# TBB
FIND_LIBRARY(TBB_LIBRARIES
    NAMES tbb libtbb
    PATHS
    ${GMP_ROOT}/lib
    /usr/lib
    /usr/local/lib
    /usr/lib/tbb
    /usr/lib64
    /usr/local/lib64
    /usr/lib64/tbb
    $ENV{ProgramFiles}/tbb/*/lib
    $ENV{SystemDrive}/tbb/*/lib
)

# compilation flags
SET( OPENVDB_COMPILE_FLAGS)

# Copy the results to the output variables.
SET( OPENVDB_LIBRARIES ${OPENVDB_LIBRARY} ${HALF_LIBRARIES} ${TBB_LIBRARIES})
SET( OPENVDB_INCLUDE_DIRS ${OPENVDB_INCLUDE_DIR} )

MARK_AS_ADVANCED( OPENVDB_INCLUDE_DIRS OPENVDB_LIBRARIES )
