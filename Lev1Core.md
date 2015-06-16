# Introduction #

Core solvers such as fundamental 7/8 point, 5-point euclidean relative pose, 5-point euclidean resection, etc. These involve a small, fixed number of points or cameras.

# Details #

As proposed by [Pierre Moulon](http://groups.google.com/group/libmv-devel/msg/6cf46a30d5047e6a), I will create a wiki page for each method of the core problems. This will improve exchange between users and developers.
For now, we have:
  * [5 pt relative pose](5ptRelative.md)
  * [8 pt fundamental matrix](8ptFundam.md)
  * [Homography  estimation](HEstim.md)
  * [Euclidean re-sectioning](Resectioning.md)
  * [Triangulation, two views and n -view.](Triangulation.md)
  * [Radial distortion and undistortion](Radialdist.md)

# Type of data structures #

This is a problem as every project has its own structures so core functions have to deal with something really heterogeneous!

Keir Mierle proposed to use either raw double array or Eigen matrix. While Eigen matrix give us a lot of useful methods and produce a cleaner code, this will add an additional dependency. I think double array is better suited and generic but will need refactoring functions.

Please add / remove / update informations!