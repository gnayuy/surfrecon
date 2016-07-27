// surfvol: to test surfrecon library with test data
// Yang Yu (gnayuy@gmail.com)

//
#include <gflags/gflags.h>
#include "surfrecon.h"

// command line interface
DEFINE_string(pc, "", "PointCloud <.off>");
DEFINE_string(o, "", "Voxels <.swc>");
DEFINE_double(t, 1.0, "threshold");

// main func
int main(int argc, char *argv[])
{
    cout<<"surfvol: to reconstruct a surface from a point cloud and voxelize it.\n";
    
    //
    gflags::SetUsageMessage("surfvol -pc <PointCloud> -o <Voxels>");
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    
    std::cout<<"Parameters:"<<std::endl;
    std::cout<<"  <PointCloud>: "<<FLAGS_pc<<" \n  <Voxels>: "<<FLAGS_o<<std::endl;
    
    
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
    //surface.getPlanes();
    
    cout<<"faces: "<<surface.faces.size()<<" planes: "<<surface.planes.size()<<" in "<< t.time() << " sec." <<endl;
    
    
    t.reset();
    VoxelSet pcOut;
    //surface.getSurfaceInVoxels(pcOut, thresh);
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