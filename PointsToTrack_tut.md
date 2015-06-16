# Introduction #

This tutorial is designed to introduce the user to the **`PointsToTrack`** class. The purpose of this class is to allow the user to detect keypoints from images for the purpose of tracking them as the viewpoint is varied. Extracted features are later used as the basis for solving the geometry between the image viewpoints, and ultimately performing Structure-from-Motion.

This tutorial will cover the following concepts:

  1. Selecting and applying a feature detector to an image.
  1. Selecting and applying a descriptor extractor to a feature vector.
  1. Performing feature matching between images.

# Details #

## Background ##

OpenCV provides several popular feature detectors which can be useful for structure from motion. Each of these detectors has changeable parameters which affect the quantity and nature of returned keypoints. Several of the detectors can also be interfaced through an **`AdjusterAdapter`** which allows the parameters to be tuned in order to return a particular number of features for a given image. Default parameters should give decent results in most cases, but are unlikely to be effective for unusual environments, or different imaging modalities.

The **`PointsToTrack`** class contains a protected member called **`keypoints_`** where the keypoints from a chosen detector can be stored. Depending on the nature of the detector used, each keypoint in the vector may store additional information beyond simply its X and Y co-ordinates in the image. Such information may include scale, response (strength), angle or octave.

The class also contains a matrix variable called **`descriptors_`** of which each row corresponds to the descriptor vector for a single keypoint.

## Example 1: Walkthrough of testPointsToTrack.cpp ##
(File found in tutorials folder - may need to uncomment first line in tutorials/CMakeLists.txt to compile.)

  1. This initial code should be explained in other tutorials:
```
MotionProcessor mp;
vector<Mat> masks;

//first load images:
//Here we will a folder with a lot of images, but we can do the same thing with any other type of input
mp.setInputSource( FROM_SRC_ROOT( "Medias/temple/" ),IS_DIRECTORY );

//Configure input ( not needed, but show how we can do
mp.setProperty( CV_CAP_PROP_CONVERT_RGB,0 );//Only greyscale, due to SIFT
mp.setProperty( CV_CAP_PROP_FRAME_WIDTH,640 );//for test
mp.setProperty( CV_CAP_PROP_FRAME_HEIGHT,480 );//idem...

//universal method to get the current image:
Mat firstImage=mp.getFrame( );
Mat secondImage=mp.getFrame( );


if( firstImage.empty( )||secondImage.empty( ) )
{
    cout<<"test can not be run... can't find different images..."<<endl;
}
else
{
```
  1. Then, if the images are effectively loaded, a SURF feature detector and SURF descriptor extractor are created:
```
    //if the images are loaded, find the points:

    cout<<"creation of two detection algorithm..."<<endl;
    Ptr<FeatureDetector> fastDetect;
    fastDetect=Ptr<FeatureDetector>( new SurfFeatureDetector( ) );
    Ptr<DescriptorExtractor> SurfDetect;
    SurfDetect=Ptr<DescriptorExtractor>( new SurfDescriptorExtractor( ) );
```
  1. Two pointers (one for each image) are then created for the PointsToTrack class. The objects are constructed with the respective image indices, the images themselves, and pointers to the chosen feature detector and descriptor extractor.
```
    cout<<"now create the two set of points with features..."<<endl;
    Ptr<PointsToTrack> ptt1;
    ptt1=Ptr<PointsToTrack>( new PointsToTrackWithImage ( 0, firstImage,Mat( ),fastDetect,SurfDetect ));
    Ptr<PointsToTrack> ptt2;
    ptt2=Ptr<PointsToTrack>( new PointsToTrackWithImage ( 1, secondImage,Mat( ),fastDetect,SurfDetect ));
```

## Example 2: Implementing different keypoint detectors ##
OpenCV provides several different interfaces for creating feature detectors. For this example, the following pair of lines from the original **testPointsToTrack.cpp** tutorial will be replaced with various single-line alternatives:
```
Ptr<FeatureDetector> fastDetect;
fastDetect=Ptr<FeatureDetector>( new SurfFeatureDetector( ) );
```

To use a detector with its default settings, the following syntax can be used:

```
Ptr<FeatureDetector> fastDetect = FeatureDetector::create("CODE");
```

Where **`CODE`** can be replaced with any one of the following strings:

  * `FAST`
  * `STAR`
  * `SIFT`
  * `SURF`
  * `ORB`
  * `MSER`
  * `GFTT`
  * `HARRIS`

Alternatively, combined detectors (also with default settings) can be implemented by preceding the detector code with either **`Grid`** or **`Pyramid`**. For example:

  * `GridFAST`, `PyramidSTAR`, ...

When the parameters of a combined/adapted feature detector need to be customized, the following syntax can be used:

```
Ptr<FeatureDetector> fastDetect = new PyramidAdaptedFeatureDetector(FeatureDetector::create("FAST"), 10);
```

This includes for dynamic-adapted feature detectors, which tune themselves to return a specified number of features:

```
Ptr<FeatureDetector> fastDetect = new DynamicAdaptedFeatureDetector(AdjusterAdapter::create("FAST"), 3500, 3550, 100);
```

When the specific parameters of a feature detector are required to be modified, try the following syntax:

```
Ptr<FeatureDetector> fastDetect = new SurfFeatureDetector( 100.00 );
```

## Example 3: Changing the descriptor scheme ##
In OpenCV, every detector is compatibile with every descriptor. However, there is some incompatibility between certain descriptors and certain matching schemes. For example, certain descriptors are in the form of binary strings, while others are in the form of vectors of floating point values. Methods of matching descriptors are generally only designed for one category of descriptor.

The descriptor declaration and matcher declaration in the original **testPointsToTrack.cpp** tutorial appear as the following:
```
// Descriptor declaration
Ptr<DescriptorExtractor> SurfDetect;
SurfDetect=Ptr<DescriptorExtractor>( new SurfDescriptorExtractor( ) );
```
An alternative single-line format is:
```
// Descriptor declaration
Ptr<DescriptorExtractor> SurfDetect = DescriptorExtractor::create("DESC_CODE");
```
Where **`DESC_CODE`** can be replaced by any one of the following options:

  * `SURF`
  * `SIFT`
  * `ORB`
  * `BRIEF`