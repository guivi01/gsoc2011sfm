# Introduction #
Thank's to [wikipedia](http://en.wikipedia.org/wiki/Triangulation), a triangulation is the process of determining the location of a point by measuring angles to it from known points at either end of a fixed baseline, rather than measuring distances to the point directly (trilateration). The point can then be fixed as the third point of a triangle with one known side and two known angles.
## Implementation idea: ##
As this method needs 2D points correspondences and camera parameters, it should inherit from a class like "StructureEstimator". This could help Level2 algorithm to handle such algorithms.

# How it's done #
## Opencv ##

Neil Cavan explained how Triangulation is done with OpenCV: `cvCorrectMatches()` can tweak the 2D points to minimize reprojection error (via fundamental matrix) and the 3D positions can then be estimated using `cvTriangulatePoints()`.

We can use `CalibrateCamera2` to find the camera intrinsic and extrinsic parameters from several views of a calibration pattern. This function can estimate the distortions parameters too.
## Libmv ##
They use the classic linear triangulation method (see `triangulation.cc`). They propose an other implementation of this problem using triangulation by plan with minimization of the re-projection error in the first image only.