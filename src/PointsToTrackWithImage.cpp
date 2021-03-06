#include "PointsToTrackWithImage.h"
#include "TracksOfPoints.h"


namespace OpencvSfM
{
  using cv::FeatureDetector;
  using cv::DescriptorExtractor;
  using cv::Ptr;
  using cv::SIFT;
  using cv::Mat;
  using std::string;
  using cv::Scalar;
  using cv::KeyPoint;
  using cv::Point;
  using std::vector;
  using cv::line;
  using cv::circle;

  PointsToTrackWithImage::PointsToTrackWithImage( int corresponding_image,
    cv::Mat imageToAnalyse,
    cv::Ptr<cv::FeatureDetector> feature_detector,
    cv::Ptr<cv::DescriptorExtractor> descriptor_detector,
    cv::Mat maskOfAnalyse )
    :PointsToTrack( corresponding_image ),
    maskOfAnalyse_( maskOfAnalyse ),feature_detector_( feature_detector ),
    descriptor_detector_( descriptor_detector )
  {
    imageToAnalyse_ = imageToAnalyse;
  }

  PointsToTrackWithImage::PointsToTrackWithImage( int corresponding_image,
    cv::Mat imageToAnalyse,std::string feature_detector,
    std::string descriptor_detector/*=""SIFT""*/, cv::Mat maskOfAnalyse )
    :PointsToTrack( corresponding_image ),
    maskOfAnalyse_( maskOfAnalyse ),
    feature_detector_( FeatureDetector::create( feature_detector )),
    descriptor_detector_( DescriptorExtractor::create( descriptor_detector ))
  {
    imageToAnalyse_ = imageToAnalyse;
  }

  void PointsToTrackWithImage::setFeatureDetector( cv::Ptr<cv::FeatureDetector> feature_detector )
  {
    feature_detector_=feature_detector;
  }

  void PointsToTrackWithImage::setDescriptorExtractor( cv::Ptr<cv::DescriptorExtractor> descriptor_detector )
  {
    descriptor_detector_=descriptor_detector;
  }

  PointsToTrackWithImage::~PointsToTrackWithImage( void )
  {
    imageToAnalyse_.release( );
    maskOfAnalyse_.release( );
  }
  
  void PointsToTrackWithImage::computeColorOfPoints()
  {
    //find color of points:
    RGB_values_.clear();
    unsigned int size_max = this->keypoints_.size( );
    unsigned int colorFinal;
    char elemSize = imageToAnalyse_.elemSize();
    for(unsigned int i=0; i<size_max; ++i )
    {
      //as we don't know type of image, we use a different processing:
      switch ( elemSize )
      {
      case 1://char
        {
          uchar color = imageToAnalyse_.at<uchar>( keypoints_[i].pt );
          colorFinal = (unsigned int)((((int)color)<<16)|((int)color<<8)|((int)color));
          break;
        }
      case 2://short???
        {
          unsigned short color = imageToAnalyse_.at<unsigned short>( keypoints_[i].pt );
          colorFinal = (unsigned int)((((int)color)<<16)|((int)color));
          break;
        }
      case 3://RGB
        {
          cv::KeyPoint& kp = keypoints_[i];
          uchar* ptr = (imageToAnalyse_.data +
            (imageToAnalyse_.step*(int)kp.pt.y) +
            (int)kp.pt.x * imageToAnalyse_.elemSize());
          colorFinal = (unsigned int)((((int)ptr[2])<<16)|((int)ptr[1]<<8)|((int)ptr[0]));
          break;
        }
      default://RGBA
        {
          colorFinal = imageToAnalyse_.at<unsigned int>( keypoints_[i].pt );
          break;
        }
      }
      RGB_values_.push_back(colorFinal);
    }
  }

  int PointsToTrackWithImage::impl_computeKeypoints_( )
  {
    this->keypoints_.clear();
    feature_detector_->detect( imageToAnalyse_,this->keypoints_,maskOfAnalyse_ );
    computeColorOfPoints();
    return this->keypoints_.size();
  }

  void PointsToTrackWithImage::impl_computeDescriptors_( )
  {
    this->descriptors_.release();//in case some descriptors were already found...
    descriptor_detector_->compute( imageToAnalyse_,this->keypoints_,this->descriptors_ );
    computeColorOfPoints();//keypoints_ may have changed!
  }
}