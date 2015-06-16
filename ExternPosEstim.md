This wiki page is not the API documentation. Indeed, Doxygen will do it for us. It's only for sharing ideas...

# Role #

Using different [FieldOfView](FieldOfView.md), this class will try to find the extern parameters (or more) of the differents [Camera](Camera.md).

TODO:
See other projects to understand how methods are implemented (which methods should be implemented here or in child class)

## How other projects do this ##

SBA (again ;) uses the Levenberg-Marquardt minimization to find what is missing... They construct a vector of parameters (can be points, intra/extern camera parameters, distortion parameters...) and try to find the best set of model parameters.

For that, they use two functions: One to compute error prediction, the other to compute the Jacobian matrix.
The error prediction is computed like that:
> Given a parameter vector **p** made up of the 3D coordinates of **n** points and the parameters of **m** cameras, compute the prediction of the measurements, i.e. the projections of 3D points in the m images. The measurements are returned in the order (hx<sub>11</sub><sup>T</sup>, .. hx<sub>1m</sub><sup>T</sup>, ..., hx<sub>n1</sub><sup>T</sup>, .. hx<sub>nm</sub><sup>T</sup>)<sup>T</sup>, where hx<sub>ij</sub> is the predicted projection of the i<sup>th</sup> point on the j<sup>th</sup> camera.
The Jacobian like that:
> Given a parameter vector **p** made up of the 3D coordinates of **n** points and the parameters of **m** cameras, compute the jacobian of the predicted measurements, i.e. the jacobian of the projections of 3D points in the m images. The jacobian is returned in the order (A<sub>11</sub>, ..., A<sub>1m</sub>, ..., A<sub>n1</sub>, ..., A<sub>nm</sub>, B<sub>11</sub>, ..., B<sub>1m</sub>, ..., B<sub>n1</sub>, ..., B<sub>nm</sub>), where A<sub>ij</sub>=dx<sub>ij</sub>/db<sub>j</sub> and B<sub>ij</sub>=dx<sub>ij</sub>/db<sub>i</sub> (see HZ).

## What will we have to do ##
Unfortunately, the CvLevMarq class from OpenCV doesn't seem to be able to handle such a process. We will probably have to rewrite this or create a wrapper (seems a better idea)!

Fortunately, OpenCV have a lot of useful functions we will use, like [cvUndistortPoints](http://opencv.willowgarage.com/documentation/camera_calibration_and_3d_reconstruction.html#undistortpoints), [cvProjectPoints2](http://opencv.willowgarage.com/documentation/camera_calibration_and_3d_reconstruction.html#projectpoints2) (can be used to compute part of the Jacobian matrix!)...