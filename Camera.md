This wiki page is not the API documentation. Indeed, Doxygen will do it for us. It's only for sharing ideas...

# Role #

This class represent the physical device which take the pictures. It is not related to a 3D position (see [FieldOfView](FieldOfView.md)). This class owns a vector of FieldOfView which can be used to estimate the different parameters.

## How this is done in LibMV ##
They use various structures to store parameters of cameras. The first one is inherited from SSBA lib :
`CameraMatrix` store both intra and extern parameters :
  * `_K` and `_invK` : store intra parameters(3\*3 matrix)
  * `_R` and `_Rt`   : rotation matrix (3\*3)
  * `_T`           : translation vector (vector of 3 double)
  * `_center`      : center of image (`_K(0,2`) and `_K(1,2)`)

They also propose to use various camera classes :
  * `Camera`        : Base class, only define an abstract method `Ray(Vect2f)`

  * `PinholeCamera` : classical camera structure with following members :
    * `projection_matrix_` : Projection matrix (3\*4)
    * `intrinsic_matrix_` : intra parameters (same as `_K`)
    * `orientation_matrix_` : rotation matrix (same as `_R`)
    * `position_` : translation vector (same as `_T`)
    * `focal_x_`,`focal_y_` : `_K(0,0)` and `_K(1,1)`
    * `principal_point_` : vector of the center of image (same as `_center`)
    * `skew_factor_` : skew of axis (`_K(0,1)`)
    * `image_size_` : size in pixels of image

  * `PinholeCameraDistortion` : inherited from `PinholeCamera`, add:
    * `radial_distortion_` : vector of rad dist coefs.
    * `tangential_distortion_` : vector of tang dist coefs.

## How this is done in Bundler ##
As they use SBA too (as LibMV and SSBA), the structures are close to previous ones... I will only post here the other vars :
  * `m_adjusted` : a boolean to know if the camera has been adjusted
  * `m_k` : the radial distortion parameters (only 2 double)
  * `m_horizon` : horizon line **Don't know how they use it for now...**
  * `m_RGB_transform` : affine parameters for RGB space (3\*4 matrix) **Seems not being used...**
  * `m_constraints` : constraints on camera parameters (7 values). They use a boolean vector to know which params are constrained and a weights vector of the same size.

They also use a structure to sore info about cameras (`camera_params_t`). In addition to the previous variables, they add:
  * `fisheye` : boolean to know if the camera model is omni-directional.
  * `f_rad`, `f_angle` : fisheye parameters

# Discussions #

From the previous considerations, we can ask ourself about the approach we should use. All libraries shown here use only one structure to store both intra and extra parameters. This can be explained probably because of speed considerations or easiest bundle adjustment, but I think it's not a good idea.

The intra parameters are related to a device while extra parameters are relative to an external coordinate system. That's why I think we should separate the two parameters (and why not add a 3`*`4 projection matrix to speed up the computation of projections into the FieldOfView class...)
So this class (Camera) should be devoted to 2D distortions estimation and rectification (both radial, tangeant and image\_to\_camera).

I propose to have only following members:
  * `intra_params_` and `inv_intra_params_` : store intra parameters(3\*3 matrix)
  * `radial_dist_` : vector of rad dist coefs (only 6 params, like OpenCV).
  * `tangential_dist_` : vector of tang dist coefs (only 2 params, like OpenCV).