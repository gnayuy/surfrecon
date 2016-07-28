# surfrecon

To reconstruct a surface (green shown in the following figure) from a point cloud (red). The surface reconstruction method we use Digne's scale space meshing approch [[1][]] implemented in CGAL. The voxelization method we use Zlatanova's 3D raster engine [[2][]]. The following figure is visulized in [NeuTu][].

![Picture](data/surfrecon1.png)

## compile with cmake

Install [CGAL][] by following the instructions on their [web site][CGAL]. This is required for the reconstruction of the surface.

Install [gflags][] for using the command lines. This is an option for compiling the example code.

## api

To use surfrecon library, you need include surfrecon.h and call the function Surf.surfrecon(PointCloud pcIn, Voxels voxelOut).

Other parameters: Surf.surfrecon(PointCloud pcIn, Voxels voxelOut, int co, int num_threads). By default, co=26 and num_threads=8 (if OpenMP is supported by your compiler).

## test

./surfvol -pc <$PointCloud> -o <$Voxels>

For example, copy point.swc from data folder to your surfvol folder and then type "./surfvol -pc points.swc -o voxels.swc".

## file formats

[OFF][]: object file format

[SWC][]: neuron file format

## references

[1]. Digne, Julie, et al. "Scale space meshing of raw data point sets." Computer Graphics Forum. Vol. 30. No. 6. Blackwell Publishing Ltd, 2011.

[2]. Zlatanova, Sisi, et al. "Towards 3D raster GIS: On developing a raster engine for spatial DBMS." ISPRS WG IV/2 Workshop" Global Geospatial Information and High Resolution Global Land Cover/Land Use Mapping" Novosibirsk, Russian Federation, 21 April 2016.

[NeuTu]:http://www.neutracing.com
[CGAL]: http://www.cgal.org
[gflags]: https://github.com/gflags/gflags
[OFF]: http://www.geomview.org/docs/html/OFF.html#OFF
[SWC]: http://www.neuronland.org/NLMorphologyConverter/MorphologyFormats/SWC/Spec.html
[1]: http://www.ingentaconnect.com/content/bpl/cgf/2011/00000030/00000006/art00003
[2]: http://www.sciencedirect.com/science/article/pii/S2215016116000029
