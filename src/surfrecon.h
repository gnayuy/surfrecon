// objsplit: to split a 3D object by a given point cloud
// Yang Yu (gnayuy@gmail.com)


#ifndef __OBJSPLIT_H__
#define __OBJSPLIT_H__

//
#include <gflags/gflags.h>

#include <CGAL/Scale_space_surface_reconstruction_3.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/IO/read_off_points.h>
#include <CGAL/Timer.h>

//#include <ApproxMVBB/ComputeApproxMVBB.hpp>

#include <openvdb/openvdb.h>
#include <openvdb/Types.h>
#include <openvdb/util/Util.h>
#include <openvdb/math/Math.h>
#include <openvdb/math/Transform.h>
#include <openvdb/tools/LevelSetUtil.h>
#include <openvdb/tools/MeshToVolume.h>
#include <openvdb/tools/VolumeToMesh.h>
#include <openvdb/tools/ParticlesToLevelSet.h>
#include <openvdb/tools/Prune.h>
#include <openvdb/tools/MaskToLevelSet.h>
#include <openvdb/tools/Interpolation.h>
#include <openvdb/tools/GridTransformer.h>
#include <openvdb/io/Stream.h>

#include <deque>
#include <queue>
#include <cstdlib>
#include <cstdio>
#include <fstream>
#include <sstream>
#include <cstring>
#include <algorithm>
#include <functional>
#include <vector>
#include <cmath>
#include <ctime>
#include <limits>
#include <complex>
#include <iostream>

#include <boost/foreach.hpp>

// c++11
//#include <thread>
//#include <mutex>
//#include <future>

//
using namespace std;

// types
typedef CGAL::Exact_predicates_inexact_constructions_kernel     Kernel;

typedef CGAL::Scale_space_surface_reconstruction_3< Kernel >    Reconstruction;

typedef Reconstruction::Point                                   Point;
typedef std::vector< Point >                                    Point_collection;

typedef Reconstruction::Triple_const_iterator                   Triple_iterator;
typedef CGAL::Timer Timer;


// classes

typedef openvdb::tree::Tree5<bool, 5, 2, 2, 3>::Type  NeuronMaskTree;
typedef openvdb::Grid<NeuronMaskTree>                 NeuronMaskGrid;

// OpenVDB I/O
class VDBIO
{
public:
    VDBIO();
    ~VDBIO();
    
public:
    void read(char const *filename);
    void write(char const *filename);
    
public:
    openvdb::GridPtrVec grids;
    
};


#endif // __OBJSPLIT_H__
