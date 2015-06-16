# Introduction #

Euclidean re-sectioning is the estimation of relative position of camera when correspondences between 3D points and 2D points are available.
## Implementation idea: ##
As this method needs 2D / 3D points correspondences, it should inherit from a class like **"PoseEstimator"**. This could help Level2 algorithm to handle such algorithms.
# How it's done #
## Opencv ##

We can use FindExtrinsicCameraParams2 to compute the object pose given a set of object points, their corresponding image projections, as well as the camera matrix and the distortion coefficients!

## Libmv ##

They uses  two methods for this operation... A classic one (probably the same OpenCV uses) and the version of  Ansar et al. : Linear Pose Estimation from Points or Lines. See `euclidean_resection.cc`