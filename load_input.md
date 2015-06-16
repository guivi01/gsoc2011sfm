# Introduction #

This tutorial aims to help user to load a list of pictures. As most of algorithms of SfM work with a list of pictures (from video files or directories), this API implements a class whose try to help you : [MotionProcessor](http://code.google.com/p/gsoc2011sfm/source/browse/trunk/src/MotionProcessor.cpp).

You can of course skip this tutorial if you have your own way to load cv::Mat, but if you are interrested into an easy way to handle input sequences, this tutorial is probably made for you! As you will see, the same object can be used to load picture from webcam, AVI, list of file's name, directory or name's prefix.

# Init the object #

In order to load files, you will have to initialize the loader from your wanted source. Next sections are devoted to the various way to do this.

## Init from a webcam ##
```
  MotionProcessor input_motion;
  input_motion.setInputSource( 0 );
```
This will open webcam 0 but of course, if you have multiple cameras, you can create the motion processor using the correct index of webcam...

## Init from a vector of image's names ##
```
  std::vector< std::string > list_of_pictures;
  list_of_pictures.push_back("my_first_one.jpg");
  list_of_pictures.push_back("my_second.png");
  list_of_pictures.push_back("my_third.bmp");
  list_of_pictures.push_back("my_last.gif");
  MotionProcessor input_motion;
  input_motion.setInputSource( list_of_pictures );
```
As you noticed, the list can contain heterogeneous file type. See below if you want the motion processor to set the same resolution and color type for each picture. By default, the loaded picture will have the default resolution / color type of the image format.

## Init from a matching pattern ##
Sometimes you have a list of picture whose names are like this:
`img7.jpg ; img8.jpg ; img9.jpg ; img10.jpg ; img11.jpg ; ...`
You can the load the entire sequence using following code:
```
  MotionProcessor input_motion;
  input_motion.setInputSource( "img", ".jpg", 7);
```
This will load the pictures with following pattern : `imgXXXX.jpg` where `XXXX` start from `7`.

## Init from a video file ##
```
  MotionProcessor input_motion;
  input_motion.setInputSource( "my_video.avi", IS_VIDEO);
```
Notice the second parameter: it's here to help method to understand what you want to load. Indeed, you can also load an entire directory:

## Init from a directory ##
```
  MotionProcessor input_motion;
  input_motion.setInputSource( "/path/to/directory", IS_DIRECTORY);
```
This will load every pictures in the directory. You need [Boost library](http://www.boost.org/) to use this feature! Each file is tested using cv::cvHaveImageReader to know if it's a picture or not. The order of files is sorted using alphabetical order.

## Init using a single file ##
Sometimes you want to load a single file but not changing the way you processed the sequence... You can then use the MotionProcessor to load a single file:
```
  MotionProcessor input_motion;
  input_motion.setInputSource( "my_picture.jpg", IS_SINGLE_FILE);
```

# Setting output #
You sometimes want to have input in a particular form. For example, when using some points detector, you have to work with grayscale images. You can then constrain the frames to be greyscaled using following code:
```
  input_motion.setProperty( CV_CAP_PROP_CONVERT_RGB, 0);
```
In the same spirit, you can constrain the images dimensions using:
```
  input_motion.setProperty( CV_CAP_PROP_FRAME_WIDTH, 640);
  input_motion.setProperty( CV_CAP_PROP_FRAME_HEIGHT, 480);
```
You can also change the position of the current frame in the sequence using:
```
  input_motion.setProperty( CV_CAP_PROP_POS_AVI_RATIO, 0.5);//middle of sequence
  input_motion.setProperty( CV_CAP_PROP_POS_FRAMES, 2);//3th image (start from 0)
```

# Getting a frame #
Once initialized you can then grab a picture using something like this:
```
  cv::Mat my_frame = input_motion.getFrame();
  cv::Mat my_next_frame = input_motion.getFrame();
```
Be careful that each call will move the current frame index.
Usually you will use it in a loop:
```
  cv::Mat current_frame = input_motion.getFrame();
  while( !current_frame.empty() )
  {
    //do something with current_frame

    current_frame = input_motion.getFrame();
  }
```

# Finally #
To conclude, here is a working example where we load each picture from a directory and show them:

```
  MotionProcessor input_motion;
  input_motion.setInputSource( "/path/to/directory", IS_DIRECTORY);
  cv::Mat current_frame = input_motion.getFrame();
  while( !current_frame.empty() )
  {
    imshow( "show sequence", current_frame );
    cv::waitKey( 25 );

    current_frame = input_motion.getFrame();
  }
```