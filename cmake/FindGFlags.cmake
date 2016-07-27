# - Find GFLAGS
#
# Find the CGAL includes and libraries
# This module defines
#  GFLAGS_FOUND           - TRUE if found, FALSE otherwise
#  GFLAGS_INCLUDE_DIR     - Include directories for CGAL
#  GFLAGS_LIBRARIES       - The libraries to link against to use CGAL

FIND_PACKAGE(PackageHandleStandardArgs)

SET(GFLAGS_SEARCHPATH
    /usr/include/
    /usr/local/include/
    /opt/include/
    /opt/local/include/
    )

FIND_PATH(GFLAGS_INCLUDE_DIR
    NAMES gflags/gflags.h
    PATHS ${GFLAGS_SEARCHPATH}
    DOC "The gflags include directory")

FIND_LIBRARY(GFLAGS_LIBRARIES
    NAMES gflags libgflags
    PATHS
    /usr/lib
    /usr/local/lib
    /usr/lib/gflags
    /usr/lib64
    /usr/local/lib64
    /usr/lib64/gflags
    DOC "The gflags libraries")

IF(GFLAGS_INCLUDE_DIR AND GFLAGS_LIBRARIES)
    SET(GFLAGS_FOUND TRUE)
ELSE(GFLAGS_INCLUDE_DIR AND GFLAGS_LIBRARIES)
    SET(GFLAGS_FOUND FALSE)
ENDIF(GFLAGS_INCLUDE_DIR AND GFLAGS_LIBRARIES)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GFLAGS REQUIRED_VARS GFLAGS_INCLUDE_DIR GFLAGS_LIBRARIES)

MARK_AS_ADVANCED( GFLAGS_INCLUDE_DIR GFLAGS_LIBRARIES )
