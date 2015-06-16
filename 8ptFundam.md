# Introduction #

In computer vision, the fundamental matrix is a 3`*`3 matrix which relates corresponding points in stereo images. In epipolar geometry, with homogeneous image coordinates, x and x′, of corresponding points in a stereo image pair, Fx describes a line (an epipolar line) on which the corresponding point x′ on the other image must lie.

## Implementation idea: ##
Like the [5 points method](5ptRelative.md), this method should inherit from a class like **"TwoViewsEstimator"**. This could help Level2 algorithm to handle such algorithm.

# How it's done #
## Opencv ##

We can use `FindFundamentalMat` to compute fundamental matrix. We can also choose to use the 7-point, 8-point algorithm and use RANSAC or LMedS minimisation very easily!
## Libmv ##

I found only the 8-points algorithm and some useful methods:

`void ProjectionsFromFundamental(const Mat3 &F, Mat34 *P1, Mat34 *P2);`

Can be used to estimate two camera matrix using a fundamental matrix. The inverse operation is also possible, which can be useful for unitary test if needed.
`void EnforceFundamentalRank2Constraint(Mat3 *F);`

If needed, the fundamental matrix can be improved... I think the OpenCV function already use this constraint, but if not, we know where to find a good implementation!