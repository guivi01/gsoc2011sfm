# Introduction #

This tutorial is designed to introduce the user to the **`PointsMatcher`** class. The purpose of this class is to allow the user to match keypoints from images for the purpose of tracking them as the viewpoint is varied.

This tutorial will only cover the feature matching problem. You will probably have to read the [tutorial about feature detector and descriptor extractor](PointsToTrack_tut.md) before read this one.

# Details #

## Background ##

When dealing with sequence, you often have to [load input files](load_input.md), [find features](PointsToTrack_tut.md) and the match them across the entire sequence. The two first steps are detailed in other tutorials, we will focus here on the matching part.

## Example 1: Walkthrough of testPointsToTrack.cpp ##

  1. This initial code should be explained in other tutorials:
```
MotionProcessor mp;
//...skip some initialisations...
vector< Ptr<PointsToTrack> > my_points;
//...skip some initialisations... See previous tuto!
//The matches vector will be:
vector<DMatch> matchesVector;
```
  1. The matcher is then created using Flann algorithm
```
//The point matcher will now be created like this:
PointsMatcher matches( Ptr<DescriptorMatcher>(
    new FlannBasedMatcher( ) ) );
//We choose the flann matcher which is really fast, but you can use any type you want!
```
  1. If we just want to match points between two images, we can do:
```
    //We want to find points in img1 which are also in img2.
    cout<<"Add points of image 2 as references points"<<endl;
    matches.add( my_points[0] );

    Ptr<PointsMatcher> matches2= matches.clone( );//use the same algorithm
    matches2->add( my_points[1] );
    //cross check matches:
    matches.crossMatch( matches2, matchesVector );
```
  1. Initial matches are then stored into `matchesVector`. We can draw them:
```
    Mat outImg;
    cout<<"Displaying the "<<matchesVector.size( )<<"points..."<<endl;
    drawMatches( firstImage, my_points[0]->getKeypoints( ), secondImage,
      my_points[1]->getKeypoints( ), matchesVector, outImg );
    imshow( "PointsMatcher key points",outImg );
    cv::waitKey( 0 );
```

## Example 2: Changing the matching scheme ##
In OpenCV, every detector is compatibile with every descriptor. However, there is some incompatibility between certain descriptors and certain matching schemes. For example, certain descriptors are in the form of binary strings, while others are in the form of vectors of floating point values. Methods of matching descriptors are generally only designed for one category of descriptor.

When creating the matcher, you can do it this way:
```
// Matcher declaration
PointsMatcher matches( DescriptorMatcher::create("MATCH_CODE") );
```
**`MATCH_CODE`** can be replaced by any one of the following options:

  * `BruteForce`
  * `BruteForce-L1`
  * `FlannBased`
  * `BruteForce-Hamming`
  * `BruteForce-HammingLUT`

The current compatibility table (as of 1 August 2011) is the following:

| | **`BruteForce`** | **`BruteForce-L1`** | **`FlannBased`** | **`BruteForce-Hamming`** | **`BruteForce-HammingLUT`** |
|:|:-----------------|:--------------------|:-----------------|:-------------------------|:----------------------------|
| **`SURF`** | YES              | YES                 | YES              | NO                       | NO                          |
| **`SIFT`** | YES              | YES                 | YES              | NO                       | NO                          |
| **`ORB`** | NO               | NO                  | NO               | YES                      | YES                         |
| **`BRIEF`** | NO               | NO                  | NO               | YES                      | YES                         |

The **`ORB`** and **`BRIEF`** descriptors and **`BruteForce-Hamming`** and **`BruteForce-HammingLUT`** matching schemes generate or accept binary-string descriptors (CV\_8U), while the other descriptors and matching schemes generate or accept vectors of floating point values (CV\_32F). It is possible to mix and match the descriptors more by converting the data format of the descriptor matrix prior to matching, but this can sometimes lead to very poor matching results, so attempt it with caution.

## Exemple 3: automatic matching across entire sequence ##
As you will probably need to match each picture against each other, the class SequenceAnalyzer will probably be better suited. The goal of this class is to find points, features and matches between each pictures.

You can then create a sequence analyser using a motion processor ([see this tutorial to details](load_input.md)), a detector, a descriptor and a matcher:
```
MotionProcessor mp;
//...skip some initialisations...
SequenceAnalyzer s_a( mp,
       FeatureDetector::create("FAST"),
       DescriptorExtractor::create("SIFT"),
       PointsMatcher::create("FlannBased") );
```
If you don't want to use the motion processor or you already have detected the features in images, you can use the following constructor:
```
MotionProcessor mp;
vector< cv::Mat > images;
//...skip some initialisations...
vector< Ptr<PointsToTrack> > my_points;
//...skip some initialisations... See previous tuto!
SequenceAnalyzer s_a( my_points,
       PointsMatcher::create("FlannBased"),
       images );
```
### Then you will be able to compute matches and much more ###
```
s_a.computeMatches();
s_a.keepOnlyCorrectMatches();
s_a.showTracks( 25 );
FileStorage fsOutMotion( "saved.yml", FileStorage::WRITE );
SequenceAnalyzer::write( fsOutMotion,s_a );
fsOutMotion.release( );
```
Here we compute matches, remove matches who are probably noise (based on the number of matches found), show them and finally save them in a YML file.