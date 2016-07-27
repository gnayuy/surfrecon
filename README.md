# surfrecon

To reconstruct a surface from a point cloud

The voxelization method we use Zlatanova's 3D raster engine [1][].

## compile using cmake

Install [CGAL][] by following the instructions on their [web site][CGAL].

Install [gflags][] for using the command lines.

## usage

./surfrecon -pc <$PointCloud> -o <$Out>

## file formats

[OFF][]: object file format

[CGAL]: http://www.cgal.org
[gflags]: https://github.com/gflags/gflags
[OFF]: http://www.geomview.org/docs/html/OFF.html#OFF
[1]: http://www.sciencedirect.com/science/article/pii/S2215016116000029

## references

[1] Zlatanova, Sisi, et al. "Towards 3D raster GIS: On developing a raster engine for spatial DBMS." ISPRS WG IV/2 Workshop" Global Geospatial Information and High Resolution Global Land Cover/Land Use Mapping" Novosibirsk, Russian Federation, 21 April 2016. 2016.
