
#
SET(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

#Add source directory to project.
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src)

# sources
SET( surfvol_sources

# header files
    ${PROJECT_SOURCE_DIR}/src/surfrecon.h

# source files 
    ${PROJECT_SOURCE_DIR}/test/surfvol.cpp

)

# gflags
FIND_PACKAGE( GFlags REQUIRED)
IF(GFLAGS_FOUND)
    SET(CMAKE_REQUIRED_LIBRARIES ${GFLAGS_LIBRARIES})
    INCLUDE_DIRECTORIES(${GFLAGS_INCLUDE_DIR})
ENDIF(GFLAGS_FOUND)

#Rule to build executable program
ADD_EXECUTABLE(surfvol ${surfvol_sources})
TARGET_LINK_LIBRARIES(surfvol ${SurfRecon_LIBRARY} )

