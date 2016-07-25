// surfrecon: to reconstruct a surface from a point cloud
// Yang Yu (gnayuy@gmail.com)

// usage:
//
//

//
#include "surfrecon.h"

// command line interface

DEFINE_string(pc, "", "point cloud input .off");
DEFINE_string(o, "", "output");
DEFINE_int64(minx, 0, "bouding box minx");
DEFINE_int64(miny, 0, "bouding box miny");
DEFINE_int64(minz, 0, "bouding box minz");

//
Vert::Vert()
{
}

Vert::~Vert()
{
}

//
Plane::Plane()
{
}

Plane::~Plane()
{
}

//
Surf::Surf()
{
}

Surf::~Surf()
{
}

void Surf::getPlanes()
{
    Point P,Q,R;
    Plane plane;
    
    //
    planes.clear();
    
    //
    for(int i=0; i<faces.size(); i++)
    {
        P = points[faces[i].p];
        Q = points[faces[i].q];
        R = points[faces[i].r];
        
        // cross product
        plane.a = (Q.y()-P.y())*(R.z()-P.z()) - (Q.z()-P.z())*(R.y()-P.y());
        plane.b = (Q.z()-P.z())*(R.x()-P.x()) - (Q.x()-P.x())*(R.z()-P.z());
        plane.c = (Q.x()-P.x())*(R.y()-P.y()) - (Q.y()-P.y())*(R.x()-P.x());
        
        plane.d = -( plane.a*P.x() + plane.b*P.y() + plane.c*P.z() );
        
        planes.push_back(plane);
    }
}

// OpenVDB I/O class
VDBIO::VDBIO()
{
    openvdb::initialize ();
}

VDBIO::~VDBIO()
{
}

void VDBIO::read(char const *filename)
{
    openvdb::io::File file(filename);
    file.open();
    
    openvdb::GridPtrVecPtr baseGrids = file.getGrids();
    if (baseGrids->empty()) {
        OPENVDB_LOG_WARN(filename << " is empty");
    }
    
    openvdb::BoolGrid::Ptr grid = openvdb::gridPtrCast<openvdb::BoolGrid>(baseGrids->at(0));
    openvdb::BoolGrid::Accessor accessor = grid->getAccessor();
    std::cout << "Leaf Nodes: " << grid->tree().leafCount() << std::endl;
}

void VDBIO::write(char const *filename)
{
    openvdb::io::File file (filename);
    file.write (grids);
    file.close ();
}


// main func
int main(int argc, char *argv[])
{
    cout<<"surfrecon: to reconstruct a surface from a point cloud\n";
    
    //
    gflags::SetUsageMessage("surfrecon -pc <PointCloud> -o <Output>");
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    
    std::cout<<"Parameters:"<<std::endl;
    std::cout<<"input: \n  <PointCloud>: "<<FLAGS_pc<<" \noutput: \n  <Output>: "<<FLAGS_o<<std::endl;
    
    
    //
    double bx = FLAGS_minx;
    double by = FLAGS_miny;
    double bz = FLAGS_minz;
    
    //
    Surf surface;
    
    //
    std::ifstream in(FLAGS_pc);
    std::cerr << "Reading " << std::flush;
    if( !in || !CGAL::read_off_points( in, std::back_inserter( surface.points ) ) ) {
        std::cerr << "Error: cannot read file" << std::endl;
        return -1;
    }
    
    std::cerr << "done: " << surface.points.size() << " points." << std::endl;
    
//    for(int i; i<points.size(); i++)
//    {
//        cout<<" "<<points[i].x()<<" "<<points[i].y()<<" "<<points[i].z()<<endl;
//    }

    
    Timer t;
    t.start();
    
    // Construct the mesh in a scale space.
    Reconstruction reconstruct( 10, 200 );
    
    reconstruct.reconstruct_surface( surface.points.begin(), surface.points.end(), 4,
                                    false, // Do not separate shells
                                    true // Force manifold output
                                    );
    
    std::cerr << "Reconstruction done in " << t.time() << " sec." << std::endl;
    
    
//    for( Triple_iterator it = reconstruct.surface_begin( ); it != reconstruct.surface_end(  ); ++it )
//    {
//        //cout << "3 "<< *it << '\n';
//        
//        for (auto i:*it)
//            std::cout << ' ' << points[i].x();
//        cout<<endl;
//        
//        
//    }
    
    
    
//    t.reset();
//    std::ofstream out(FLAGS_o);
//    for( Triple_iterator it = reconstruct.surface_begin( ); it != reconstruct.surface_end(  ); ++it )
//        out << "3 "<< *it << '\n'; // We write a '3' in front so that it can be assembled into an OFF file
//    
//    std::cerr << "Writing result in " << t.time() << " sec." << std::endl;
//    
//    out.close();
    
//    t.reset();
//    std::ofstream garbage ("garbage.off");
//    // Write facets that were removed to force manifold output
//    for( Triple_iterator it = reconstruct.garbage_begin( ); it != reconstruct.garbage_end(  ); ++it )
//        garbage << "3 "<< *it << '\n'; // We write a '3' in front so that it can be assembled into an OFF file
//    std::cerr << "Writing garbage facets in " << t.time() << " sec." << std::endl;
//    
//    garbage.close ();
    
    
    //
    VDBIO vdb;
    
    openvdb::FloatGrid::Ptr grid = openvdb::FloatGrid::create();
    openvdb::FloatGrid::Accessor accessor = grid->getAccessor();
    
    grid->setTransform (openvdb::math::Transform::createLinearTransform (0.5));
    grid->setGridClass (openvdb::GRID_FOG_VOLUME);
    grid->setName ("surface");
    
    openvdb::Coord ijk;
    
    ijk[0] = 100; // x
    ijk[1] = 100; // y
    ijk[2] = 100; // z
    
    accessor.setValue(ijk, 1.0);
    
    vdb.grids.push_back(grid);
    
    vdb.write(const_cast<char*>(FLAGS_o.c_str()));
    
    
    
    
    //
    std::cerr << "Done." << std::endl;

    
    //
    return 0;
}
