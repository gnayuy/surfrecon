// objsplit: to split a 3D object by a given point cloud
// Yang Yu (gnayuy@gmail.com)

// usage:
//
//

//
#include "objsplit.h"

// command line interface

DEFINE_string(obj, "", "object input ");
DEFINE_string(pc, "", "point cloud input .off");
DEFINE_string(o, "", "output");
DEFINE_int64(minx, 0, "bouding box minx");
DEFINE_int64(miny, 0, "bouding box miny");
DEFINE_int64(minz, 0, "bouding box minz");


// OpenVDB I/O class
VDBIO::VDBIO()
{
    openvdb::initialize ();
    openvdb::Grid<NeuronMaskTree>::registerGrid();
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
    
    NeuronMaskGrid::Ptr grid = openvdb::gridPtrCast<NeuronMaskGrid>(baseGrids->at(0));
    NeuronMaskGrid::Accessor accessor = grid->getAccessor();
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
    cout<<"objsplit: to split a 3D object by a given point cloud\n";
    
    //
    gflags::SetUsageMessage("objsplit -obj <object> -pc <pointcloud> -o <output>");
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    
    std::cout<<"Parameters:"<<std::endl;
    std::cout<<"inputs: \n  <Object>: "<<FLAGS_obj<<" \n  <PointCloud>: "<<FLAGS_pc<<" \noutput: \n  <splitObjects>: "<<FLAGS_o<<std::endl;
    
    
    //
    double bx = FLAGS_minx;
    double by = FLAGS_miny;
    double bz = FLAGS_minz;
    
//    ApproxMVBB::Vector3 origin = ApproxMVBB::Vector3(bx, by, bz);
//    
//    cout<<"origin: "<<origin.transpose()<<endl;
//    
//
    // 3d object
    VDBIO vdb;
    vdb.read(const_cast<char*>(FLAGS_obj.c_str()));
    
    // points
    Point_collection points;
    std::ifstream in(FLAGS_pc);
    std::cerr << "Reading " << std::flush;
    if( !in || !CGAL::read_off_points( in, std::back_inserter( points ) ) ) {
        std::cerr << "Error: cannot read file" << std::endl;
        return -1;
    }
    
    std::cerr << "done: " << points.size() << " points." << std::endl;
    
//    for(int i; i<points.size(); i++)
//    {
//        cout<<" "<<points[i].x()<<" "<<points[i].y()<<" "<<points[i].z()<<endl;
//    }
    
    // obb
//    ApproxMVBB::Matrix3Dyn pointSet(3,points.size());
    
//    for(int i=0; i<points.size(); i++)
//    {
//        pointSet.col(i) = ApproxMVBB::Vector3(points[i].x(),points[i].y(),points[i].z()) - origin;
//        
//        cout<<" "<<pointSet.col(i).transpose()<<endl;
//    }
    

//    ApproxMVBB::Matrix3Dyn pointSet(3,3);
//    
//    pointSet.col(0) = ApproxMVBB::Vector3(25, 25, 25);
//    pointSet.col(1) = ApproxMVBB::Vector3(25, 26, 25);
//    pointSet.col(2) = ApproxMVBB::Vector3(26, 25, 26);
//    
//    ApproxMVBB::OOBB oobb = ApproxMVBB::approximateMVBB(pointSet,0.001,500,5,0,5);
//    
//    std::cout << "Computed OOBB: " << std::endl
//    << "---> lower point in OOBB frame: " << oobb.m_minPoint.transpose() << std::endl
//    << "---> upper point in OOBB frame: " << oobb.m_maxPoint.transpose() << std::endl
//    << "---> coordinate transformation A_IK matrix from OOBB frame K to world frame I" << std::endl
//    << oobb.m_q_KI.matrix() << std::endl
//    << "---> this is also the rotation matrix R_KI  which turns the world frame I into the OOBB frame K" <<std::endl << std::endl;
//    
//    
//    // To make all points inside the OOBB :
//    ApproxMVBB::Matrix33 A_KI = oobb.m_q_KI.matrix().transpose(); // faster to store the transformation matrix first
//    auto size = pointSet.cols();
//    for( unsigned int i=0;  i<size; ++i ) {
//        oobb.unite(A_KI*pointSet.col(i));
//    }
//    std::cout << "OOBB with all point included: " << std::endl
//    << "---> lower point in OOBB frame: " << oobb.m_minPoint.transpose() << std::endl
//    << "---> upper point in OOBB frame: " << oobb.m_maxPoint.transpose() << std::endl;
//    
//
//    ApproxMVBB::Vector3 p = oobb.m_q_KI * oobb.m_minPoint;
//    ApproxMVBB::Vector3 q = oobb.m_q_KI * oobb.m_maxPoint;
//    
//    cout<<"lower point: "<<p.transpose()<<"; upper point: "<<q.transpose()<<endl;
    
    
    
    
    
    
    Timer t;
    t.start();
    
    // Construct the mesh in a scale space.
    Reconstruction reconstruct( 10, 200 );
    
    reconstruct.reconstruct_surface( points.begin(), points.end(), 4,
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
    
    
    
    t.reset();
    std::ofstream out(FLAGS_o);
    for( Triple_iterator it = reconstruct.surface_begin( ); it != reconstruct.surface_end(  ); ++it )
        out << "3 "<< *it << '\n'; // We write a '3' in front so that it can be assembled into an OFF file
    
    std::cerr << "Writing result in " << t.time() << " sec." << std::endl;
    
    out.close();
    
//    t.reset();
//    std::ofstream garbage ("garbage.off");
//    // Write facets that were removed to force manifold output
//    for( Triple_iterator it = reconstruct.garbage_begin( ); it != reconstruct.garbage_end(  ); ++it )
//        garbage << "3 "<< *it << '\n'; // We write a '3' in front so that it can be assembled into an OFF file
//    std::cerr << "Writing garbage facets in " << t.time() << " sec." << std::endl;
//    
//    garbage.close ();
    
    std::cerr << "Done." << std::endl;

    
    //
    return 0;
}
