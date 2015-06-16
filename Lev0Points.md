# Introduction #

Correspondence finding routines (SIFT/SURF/DAISY, KLT/NCC).
Here we will discuss about what we want for this core functionalities.

I think this can be done correctly using OpenCV as the number of algorithm are huge, both for features detection and matching. The number of dependences are not as much because we only need **opencv\_core** and **opencv\_features2d** shared libraries (if we want file loading, we should add **opencv\_highgui**)

So this Level0 problem is done for now using only OpenCV functions. Please leave comments if something needs to be more generic or rebuild. I know eve, b! But OpenCV is a library we can't do without!

# Proposition #

Please add / remove / update ideas!

## File processor ##
_?Probably not in Core level 0 functionality (much a level3)?_

As input can be of different types (movie, webcam, folder of pictures...), we should have a common interface to load files. With this, we don't take time to think about how can we get frames...
For now, a first implementation can be found in [MotionProcessor.h](http://code.google.com/p/gsoc2011sfm/source/browse/trunk/src/MotionProcessor.h).
## Feature detection ##
As OpenCV has a lot of algorithm, I propose to have only a wrapper of the OpenCV algorithms.
The feature detection and extraction are two separate process, we should keep this separation (some feature detection algorithm can't extract features for example).
For now, a first implementation can be found in [PointsToTrack.h](http://code.google.com/p/gsoc2011sfm/source/browse/trunk/src/PointsToTrack.h) (a more useful implementation inherit from this class: [PointsToTrackWithImage.h](http://code.google.com/p/gsoc2011sfm/source/browse/trunk/src/PointsToTrackWithImage.h))
## Feature matching ##
Same as before, OpenCV as lot of good algorithm (FLANN for example) so a first shot can just be a wrapper of the [DescriptorMatcher](http://opencv.willowgarage.com/documentation/cpp/common_interfaces_for_feature_detection_and_descriptor_extraction.html#descriptormatcher) class.
For now, a first implementation can be found in [PointsMatcher.h](http://code.google.com/p/gsoc2011sfm/source/browse/trunk/src/PointsMatcher.h).

This implementation is really close to nRobustViewMatching class of libmv library!
## Camera structures ##
Instead of having a camera class which have both intra parameters **and** 3D position and orientation, I think we should separate this in to classes: one for camera device modelisation and one for camera position and orientation. Indeed, this add an abstraction very useful: projection of points doesn't need any assumption on camera type (with/without distortion, fisheyes/pinhole...). For implementation of this idea, see [Camera.h](http://code.google.com/p/gsoc2011sfm/source/browse/trunk/src/Camera.h), [CameraPinhole.h](http://code.google.com/p/gsoc2011sfm/source/browse/trunk/src/CameraPinhole.h), [CameraPinholeDistor.h](http://code.google.com/p/gsoc2011sfm/source/browse/trunk/src/CameraPinholeDistor.h) and [PointOfView.h](http://code.google.com/p/gsoc2011sfm/source/browse/trunk/src/PointOfView.h).
I hope this implementation will start a lot of discussions and I'm ready to debate :)
```
//As we don't know what type of camera we use (with/without disportion, fisheyes...)
//we can't use classic projection matrix P = K . [R|t]
//Instead, we first compute points transformation into camera's system and then compute
//pixel coordinate using camera device function.

    
vector<Vec2d> pointsOut;
vector<Vec3d>::iterator point=points.begin();
while(point!=points.end())
{
  Vec4d pointNorm << (*point) , 1.0;

  //transform points into camera's coordinates:
  Vec3d mat2DNorm = ( projection_matrix_ * pointNorm );

  pointsOut.push_back( Vec2d(point2DNorm[0]/point2DNorm[2], point2DNorm[1]/point2DNorm[2]) );

  point++;
}

//transform points into pixel coordinates using camera intra parameters:
return device_->normImageToPixelCoordinates(pointsOut);
```