// surfrecon: to reconstruct a surface from a point cloud
// Yang Yu (gnayuy@gmail.com)

//
#include "surfrecon.h"

// command line interface
DEFINE_string(pc, "", "point cloud input .off");
DEFINE_string(o, "", "output");
DEFINE_double(t, 25.0, "threshold");

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

void Surf::setPoints(VoxelSet pointcloud)
{
    points.clear();
    
    for(int i=0; i<pointcloud.size(); i++)
    {
        points.push_back(Point(pointcloud[i].x, pointcloud[i].y, pointcloud[i].z));
    }
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

void Surf::getSurfaceInVoxels(VoxelSet &voxels, float thresh)
{
    //
    long bx, by, bz, ex, ey, ez; // bounding box
    Point P,Q,R;
    Plane plane;
    
    //
    voxels.clear();
    
    //
    for(int i=0; i<planes.size(); i++)
    {
        P = points[faces[i].p];
        Q = points[faces[i].q];
        R = points[faces[i].r];
        
        bx = fmin( fmin( P.x(), Q.x() ), R.x() ) + 0.5;
        ex = fmax( fmax( P.x(), Q.x() ), R.x() ) + 0.5;
        
        by = fmin( fmin( P.y(), Q.y() ), R.y() ) + 0.5;
        ey = fmax( fmax( P.y(), Q.y() ), R.y() ) + 0.5;
        
        bz = fmin( fmin( P.z(), Q.z() ), R.z() ) + 0.5;
        ez = fmax( fmax( P.z(), Q.z() ), R.z() ) + 0.5;
        
        plane = planes[i];
        
        for(long z=bz; z<=ez; z++)
        {
            for(long y=by; y<=ey; y++)
            {
                for(long x=bx; x<=ex; x++)
                {
                    if(fabs( plane.a*x + plane.b*y + plane.c*z + plane.d ) <= thresh)
                        voxels.push_back(VoxelType(x,y,z));
                }
            }
        }
        
    }
}

void Surf::surfrecon(VoxelSet pcIn, VoxelSet &pcOut)
{
    // init
    float thresh = 25;
    
    setPoints(pcIn);
    
    // reconstruct
    Timer t;
    t.start();
    
    Reconstruction reconstruct( 10, 200 );
    
    reconstruct.reconstruct_surface( points.begin(), points.end(), 4,
                                    false, // Do not separate shells
                                    true // Force manifold output
                                    );
    
    std::cerr << "Reconstruction done in " << t.time() << " sec." << std::endl;

    //
    faces.clear();
    for( Triple_iterator it = reconstruct.surface_begin( ); it != reconstruct.surface_end(  ); ++it )
    {
        int c=0;
        Vert v;
        for (auto i:*it)
        {
            switch (c++)
            {
                case 0:
                    v.p = i;
                    break;
                    
                case 1:
                    v.q = i;
                    break;
                    
                case 2:
                    v.r = i;
                    break;
                    
                default:
                    break;
            }
        }
        faces.push_back(v);
    }
    
    t.reset();
    getPlanes();
    cout<<" planes: "<<planes.size()<<" in "<< t.time() << " sec." <<endl;
    
    
    t.reset();
    getSurfaceInVoxels(pcOut, thresh);
    cout<<"voxels: "<<pcOut.size()<< " in "<<t.time()<<" sec."<<endl;
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
    Surf surface;
    
    // Threshold
    float thresh = float(FLAGS_t);
    
    //
    std::ifstream in(FLAGS_pc);
    std::cerr << "Reading " << std::flush;
    if( !in || !CGAL::read_off_points( in, std::back_inserter( surface.points ) ) ) {
        std::cerr << "Error: cannot read file" << std::endl;
        return -1;
    }
    
    std::cerr << "done: " << surface.points.size() << " points." << std::endl;

    //
    Timer t;
    t.start();
    
    // Construct the mesh in a scale space.
    Reconstruction reconstruct( 10, 200 );
    
    reconstruct.reconstruct_surface( surface.points.begin(), surface.points.end(), 4,
                                    false, // Do not separate shells
                                    true // Force manifold output
                                    );
    
    std::cerr << "Reconstruction done in " << t.time() << " sec." << std::endl;
    
    //
    surface.faces.clear();
    for( Triple_iterator it = reconstruct.surface_begin( ); it != reconstruct.surface_end(  ); ++it )
    {
        //cout << "3 "<< *it << '\n';
        
        int c=0;
        Vert v;
        for (auto i:*it)
        {
            //std::cout << ' ' << points[i].x();

            switch (c++)
            {
                case 0:
                    v.p = i;
                    break;
                    
                case 1:
                    v.q = i;
                    break;
                    
                case 2:
                    v.r = i;
                    break;
                    
                default:
                    break;
            }
        }
        //cout<<endl;
        
        surface.faces.push_back(v);
    }
    
    t.reset();
    surface.getPlanes();
    
    cout<<"faces: "<<surface.faces.size()<<" planes: "<<surface.planes.size()<<" in "<< t.time() << " sec." <<endl;
    
    
    t.reset();
    VoxelSet pcOut;
    surface.getSurfaceInVoxels(pcOut, thresh);
    cout<<"voxels: "<<pcOut.size()<< " in "<<t.time()<<" sec."<<endl;
    
    // Output
    t.reset();
    std::ofstream out(FLAGS_o);
    for(long i=0; i<pcOut.size(); i++)
    {
        out << pcOut[i][0] <<" "<< pcOut[i][1] <<" "<<pcOut[i][2]<< '\n';
    }
    std::cerr << "Writing "<<pcOut.size()<<" voxels in " << t.time() << " sec." << std::endl;

    out.close();
    
    //
    std::cerr << "Done." << std::endl;

    
    //
    return 0;
}
