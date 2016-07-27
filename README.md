# surfrecon

To reconstruct a surface (green shown in the following figure) from a point cloud (red). The voxelization method we use Zlatanova's 3D raster engine [[1][]]. The following figure is visulized in [NeuTu][].

![Picture](data/surfrecon1.png)

## compile using cmake

Install [CGAL][] by following the instructions on their [web site][CGAL].

Install [gflags][] for using the command lines.

## API

To use surfrecon library, you need include surfrecon.h and call the function Surf.surfrecon(PointCloud p, Voxels v).

## test

./surfvol -pc <$PointCloud> -o <$Voxels>

## file formats

[OFF][]: object file format

[SWC][]: neuron file format

## references

[1]. Zlatanova, Sisi, et al. "Towards 3D raster GIS: On developing a raster engine for spatial DBMS." ISPRS WG IV/2 Workshop" Global Geospatial Information and High Resolution Global Land Cover/Land Use Mapping" Novosibirsk, Russian Federation, 21 April 2016. 2016.

[NeuTu]:http://www.neutracing.com
[CGAL]: http://www.cgal.org
[gflags]: https://github.com/gflags/gflags
[OFF]: http://www.geomview.org/docs/html/OFF.html#OFF
[SWC]: http://www.neuronland.org/NLMorphologyConverter/MorphologyFormats/SWC/Spec.html
[1]: http://www.sciencedirect.com/science/article/pii/S2215016116000029
