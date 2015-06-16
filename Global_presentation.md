# Introduction #

In this page, I will present how files and objects are organized in this API. Here is the big picture showing different processes and their relations:
![http://www.charlie-soft.com/Structure%20from%20motion/block_organization.png](http://www.charlie-soft.com/Structure%20from%20motion/block_organization.png)


# Details #

## Input handler ##
Structure from motion starts from a motion and produces a structure as output (yes, you've learn a lot with this definition...). So the first step in this process is to give pictures to feed the algorithms.

In this API, the type of input is not important: we get the current frame from the sequence in the same way when using a folder, a video file... All details can be found in [MotionProcessor.h](http://code.google.com/p/gsoc2011sfm/source/browse/trunk/src/MotionProcessor.h), and an example in [test\_save\_load.cpp](http://code.google.com/p/gsoc2011sfm/source/browse/trunk/tutorials/test_save_load.cpp).
#### Related files: ####
[MotionProcessor.h](http://code.google.com/p/gsoc2011sfm/source/browse/trunk/src/MotionProcessor.h)

## Sequence analyse ##
Once input data are correctly configured, we have to extract information from them.

Features detection, extraction and match are done using Opencv functions. We provide various way to do this but the easiest way is probably to use [SequenceAnalyzer.h](http://code.google.com/p/gsoc2011sfm/source/browse/trunk/src/SequenceAnalyzer.h) as the entire process is automatized. An example can be found in [testMotionEstimator.cpp](http://code.google.com/p/gsoc2011sfm/source/browse/trunk/tutorials/testMotionEstimator.cpp).
#### Related files: ####
[SequenceAnalyzer.h](http://code.google.com/p/gsoc2011sfm/source/browse/trunk/src/SequenceAnalyzer.h)
[PointsMatcher.h](http://code.google.com/p/gsoc2011sfm/source/browse/trunk/src/PointsMatcher.h)
[PointsToTrack.h](http://code.google.com/p/gsoc2011sfm/source/browse/trunk/src/PointsToTrack.h)
[PointsToTrackWithImage.h](http://code.google.com/p/gsoc2011sfm/source/browse/trunk/src/PointsToTrackWithImage.h)
[TracksOfPoints.h](http://code.google.com/p/gsoc2011sfm/source/browse/trunk/src/TracksOfPoints.h)

## Cameras ##
We choose to separate the configuration of a camera (intra parameters and type (pinhole, fisheye, with distortion...) and their position (rotation and translation) to focus on problems of each part.

In practice, when we want to use a camera, we have to set the type of device we use and create a point of view using this device:
```
  Ptr<Camera> my_device = Ptr<Camera>( new CameraPinhole(intra_params) );
  PointOfView p( my_device, rotation, translation) );
```
#### Related files: ####
[Camera.h](http://code.google.com/p/gsoc2011sfm/source/browse/trunk/src/Camera.h)
[CameraPinhole.h](http://code.google.com/p/gsoc2011sfm/source/browse/trunk/src/CameraPinhole.h)
[CameraPinholeDistor.h](http://code.google.com/p/gsoc2011sfm/source/browse/trunk/src/CameraPinholeDistor.h)
[PointOfView.h](http://code.google.com/p/gsoc2011sfm/source/browse/trunk/src/PointOfView.h)
[ProjectiveEstimator.h](http://code.google.com/p/gsoc2011sfm/source/browse/trunk/src/ProjectiveEstimator.h)

## 3D object estimation ##
When we have different points of view of the same object, we can try to estimate the object's 3D shape.

For example of such process, you can see test\_reconstruction.cpp which use the dataset from http://vision.middlebury.edu/mview/data/ to triangulate points of the temple.
#### Related files: ####
[StructureEstimator.h](http://code.google.com/p/gsoc2011sfm/source/browse/trunk/src/StructureEstimator.h)

# Bundle adjustment #
Of course the various SfM technics need refinements which is done by bundle adjustment.

This process is not represented on this schema because it uses every process blocks.