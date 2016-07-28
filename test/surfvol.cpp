// surfvol: to test surfrecon library with test data
// Yang Yu (gnayuy@gmail.com)

//
#include <gflags/gflags.h>
#include "surfrecon.h"

// command line interface
DEFINE_string(pc, "", "PointCloud <.off>");
DEFINE_string(o, "", "Voxels <.swc>");

// SWC node
class Node
{
public:
    float x,y,z;
    int n, parent, type, radius;
};

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
    VoxelSet pcIn, voxelOut;
    string line;
    
    //
    Timer t;
    t.start();
    
    // load input point cloud (.swc)
    std::ifstream in(FLAGS_pc);
    
    pcIn.clear();
    if (in.is_open())
    {
        while ( getline (in,line) )
        {
            if(strstr(line.c_str(),"#"))
            {
                //cout << " skip comments" << endl;
            }
            else
            {
                istringstream iss(line);
                
                if(iss.fail() || iss.eof())
                    continue;
                
                Node node;
                
                iss >> node.n;
                iss >> node.type;
                
                iss >> node.x;
                iss >> node.y;
                iss >> node.z;
                
                iss >> node.radius;
                iss >> node.parent;
                
                pcIn.push_back(VoxelType(node.x, node.y, node.z));
            }
        }
        in.close();
    }
    else
    {
        cout << "Unable to open file"<<FLAGS_pc<<endl;
    }
    cout<<" load " << FLAGS_pc << " in " <<t.time()<<" sec. "<<endl;

    //
    surface.surfrecon(pcIn, voxelOut);

    // Output
    t.reset();
    std::ofstream out(FLAGS_o);
    for(long i=0; i<voxelOut.size(); i++)
    {
        out << i << " 0 "<< voxelOut[i].x <<" "<< voxelOut[i].y <<" "<<voxelOut[i].z<< " 0.5 -1\n";
    }
    std::cerr << " save " << FLAGS_o <<" in "<< t.time() << " sec." << std::endl;
    
    out.close();
    
    //
    std::cerr << "Done." << std::endl;
    
    
    //
    return 0;
}