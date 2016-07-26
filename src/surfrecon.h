// surfrecon: to reconstruct a surface from a point cloud
// Yang Yu (gnayuy@gmail.com)


#ifndef __SURFRECON_H__
#define __SURFRECON_H__

//
#include <gflags/gflags.h>

#include <CGAL/Scale_space_surface_reconstruction_3.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/IO/read_off_points.h>
#include <CGAL/Timer.h>

#include <glm/vec3.hpp>

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
typedef std::vector< Point >                                    PointSet;

typedef Reconstruction::Triple_const_iterator                   Triple_iterator;
typedef CGAL::Timer Timer;

typedef glm::vec3                                               VoxelType;
typedef std::vector< VoxelType >                                VoxelSet;

// classes

// Vert
class Vert
{
public:
    Vert();
    ~Vert();
    
public:
    int p, q, r;
};

typedef std::vector<Vert>    FaceSet;

// Plane
class Plane
{
public:
    Plane();
    ~Plane();
    
public:
    double a,b,c,d;
};

typedef std::vector<Plane>    PlaneSet;

// Surf
class Surf
{
public:
    Surf();
    ~Surf();
    
public:
    void setPoints(VoxelSet pointcloud);
    void getPlanes();
    void getSurfaceInVoxels(VoxelSet &voxels, float thresh);
    void surfrecon(VoxelSet pcIn, VoxelSet &pcOut);
    
public:
    PointSet points;
    FaceSet faces;
    PlaneSet planes;
};


#endif // __SURFRECON_H__
