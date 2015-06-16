# Introduction #

This tutorial will try to introduce the "euclidean reconstruction" offered by this API. This means that we will see how we can find camera position and geometry automatically!

For ease of reading, in this tuto we expect that the intra parameters don't change across the sequence. It's not a strong requirement as this API allow wariations, but it's easier to explain. We also need an estimation of the intra configuration of the camera. We need theses five parameters:
  * The focal (x) (value usually between 1000 and 2500)
  * The focal\_y_(can be set equal to focal\_x_)
  * The principal\_point\_x_(can be set to image width/2)
  * The principal\_point\_y_ (can be set to image height/2)
  * The skew (can be set to 0)

As the points detection, extraction and matching are fully parametrable, we have to choose the one we will use. For details about detectors, we can [read this tutorial](PointsToTrack_tut.md) or see the [Opencv reference manual](http://opencv.itseez.com/modules/features2d/doc/feature_detection_and_description.html). For the matching, [this tutorial](Matching_tuto.md) explains the correct relation between matcher and descriptors...

And last but not least, you have to set up the input sequence type. Indeed, you can choose to get input images from Webcam, a video file or a directory full of images!

# Details #

So how can we build a little application using theses infos? We can start by building the device camera using intra parameters:
```
  Mat K = Mat::eye(3,3,CV_64F);
  double* data_intra_param=( double* )K.data;
  data_intra_param[0] = 1500;//for example...
  //other parameters...
  Ptr<Camera> my_device = new CameraPinhole( K );
```
As you see, the camera device isn't related to a position.

Then we set the motion input type using MotionProcessor (here it's a directory, but [this tutorial](read.md) to see how change this...)
```
  MotionProcessor mp;
  mp.setInputSource( "/path/to/files/", IS_DIRECTORY );
```

We can then load images and create point of view (a camera with a position and orientation) like this:
```
  Mat currentImage=mp.getFrame( );
  while ( !currentImage.empty( ) )
  {
    images.push_back( currentImage );
    myCameras.push_back( PointOfView( my_device ) );

    //for fun, show loaded picture:
    imshow( "Loaded Image",currentImage);
    waitKey( 25 );
    currentImage=mp.getFrame( );
  }
```

As we don't know where the camera is, we will create at the origin as many cameras as images. We will also compute points for each images:
```
  vector<PointOfView> myCameras;
  vector< Ptr<PointsToTrack> > vec_point_for_track;
  for( size_t i = 0; i<images.size(); ++i )
  {
    myCameras.push_back( PointOfView( my_device ) );
    Ptr<PointsToTrack> ptrPoints_tmp =
      Ptr<PointsToTrack>( new PointsToTrackWithImage ( images.size()-1,
      currentImage, methodDetect, methodExtract ));
    ptrPoints_tmp->computeKeypointsAndDesc();
    vec_point_for_track.push_back( ptrPoints_tmp );
  }
```

We are now ready to find matches between each images:
```
  //create the matcher:
  Ptr<DescriptorMatcher> matcher  = DescriptorMatcher::create( "BruteForce-HammingLUT" );
  //and the sequence analyzer:
  SequenceAnalyzer motion_estim( vec_point_for_track, &images,
    new PointsMatcher( matcher ) );

  motion_estim.computeMatches( );
  SequenceAnalyzer::keepOnlyCorrectMatches(motion_estim,2,0);
```

Finally, compute the 3D estimation:
```
  //now create the euclidean estimator:
  EuclideanEstimator pe( motion_estim, myCameras );
  pe.computeReconstruction( );

  //finally show reconstruction:
  pe.viewEstimation();
```