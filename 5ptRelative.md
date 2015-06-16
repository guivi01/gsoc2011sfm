# Introduction #

This method is useful as it can find relative pose of two camera using only 5 points.

## Implementation idea: ##
As this method is related to "two views algorithms", it should inherit from a class like **"TwoViewsEstimator"**. This could help Level2 algorithm to handle such algorithm.

# How it's done #

## Opencv ##

Can’t find implementation of such feature...

## Libmv ##

In file `five_point.cc`, the authors give an implementation based on the parper of H. Stewénius, C. Engels and D. Nistér :  ["Recent Developments on Direct Relative Orientation"](http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.61.9329&rep=rep1&type=pdf). The method shows good behavior, even when in bad conditions!
### Interface: ###
  * x1 Points in the first image.  One per column.
  * x2 Corresponding points in the second image. One per column.
  * E  A list of at most 10 candidate essential matrix solutions.
`void FivePointsRelativePose(const Mat2X &x1, const Mat2X &x2,vector<Mat3> *E);`
### Difficulties to use or special care: ###
To be computed, we need SVD decomposition (they use JacobiSVD of Eigen library...) and [EigenSolver](http://eigen.tuxfamily.org/dox/classEigen_1_1EigenSolver.html). See if this dependence can be removed easily.
All functions use libmv types, like Mat, Mat2X, Mat3, Vec (which are typedef of Eigen matrix)... Again, see how we can remove the dependence to Eigen without to much pain, but some time is still needed!
### Functions useful ###
`void MotionFromEssential(const Mat3 &E,vector<Mat3> *Rs,vector<Vec3> *ts)`

From E, compute Rs and Ts (the 4 possible choices for the second camera matrix). They create the function `MotionFromEssentialChooseSolution` which can be used to find the correct solution (we need the the intra parameters and points match). In file `fundamental.cc`