# This API will be designed to: #
  * Load a vector of pictures and find a list of points easy to track
  * Create _tracks_, where a track is a connected set of matching keypoints across multiple images.
  * Use multiples _tracks_ to estimate the intra, distortion and extern cameras parameters
  * Refine the cameras parameters (bundle adjustment)
  * Visualize the 3D points across multiple camera

# What would be great to have in this API? #

  * Load intra parameters of camera using <a href='http://en.wikipedia.org/wiki/Exchangeable_image_file_format'>EXIF informations</a>
  * Simple application: View morphing

# Exchange with other projects #
## ["Collaborating on an SfM API" thread](http://groups.google.com/group/libmv-devel/browse_thread/thread/6a4c895f88df22ef). ##

A proposition of core functionalities (unordered):
  1. 5 pt relative pose (Already in OpenCV)
  1. 8 pt fundamental matrix (Already in OpenCV)
  1. homography (Already in OpenCV)
  1. if we are getting fancy then relative pose with focal length and radial distortion
  1. euclidean resectioning minimal and non-minimal
  1. triangulation, two view and n -view. (part of it in OpenCV)
  1. ransac and its various variants. (use the implementation of libmv sees a good idea)
  1. radial distortion and undistortion (sba routine)
  1. bundle adjustment (sba too)

Segmentation of problems:
  * Level 0: Correspondence finding routines (Already in OpenCV)
  * Level 1: Core solvers such as fundamental 7/8 point, 5-point euclidean relative pose, 5-point euclidean resection, etc. (Already in OpenCV, only need restructuring)
  * Level 2: Routines that can robustly solve Level 1 problems with noisy data. (RANDSAC algo from libmv)
  * Level 3: Any algorithm that involves multiple calls to level 2 routines. This includes general reconstruction from e.g. image collections or video sequences. These require e.g. doing a 2-view solve, then resectioning additional views, then triangulating more points, etc.