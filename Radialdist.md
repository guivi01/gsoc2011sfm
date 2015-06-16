# Introduction #

A good description of such distortion can be found on [wikipedia](http://en.wikipedia.org/wiki/Distortion_(optics)#Radial_distortion). A good SfM library should include methods to estimate and remove such perturbation.

## Ideas of implementation ##
As [proposed before](http://code.google.com/p/gsoc2011sfm/wiki/Lev0Points#Camera_structures), the camera class is devoted to device transformation. So the inherited class [CameraPinholeDistor](http://code.google.com/p/gsoc2011sfm/source/browse/trunk/src/CameraPinholeDistor.h) implement the transformation (and back-transformation) of 2D points with radial and tangential distortion.

# How it's done #
## OpenCV ##
The distortion parameters are stored into a vector of various size:
(k<sub>1</sub>, k<sub>2</sub>, p<sub>1</sub>, p<sub>2</sub>`[`, k<sub>3</sub>`[`, k<sub>4</sub>, k<sub>5</sub>, k<sub>6</sub>`]]`) where k<sub>1</sub> -- k<sub>6</sub> are the first six radial distortion parameters and p<sub>1</sub>, p<sub>2</sub> are the tangential distortion parameters.