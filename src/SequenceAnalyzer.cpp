
#include <boost/thread/thread.hpp>

#include <iostream>
#include <sstream>

#include "SequenceAnalyzer.h"
#include "Boost_Matching.h"
#include "Camera.h"

#include "config_SFM.h"  //SEMAPHORE

using cv::Ptr;
using cv::Mat;
using cv::DMatch;
using cv::KeyPoint;
using std::vector;
using cv::Point3d;

namespace OpencvSfM{

  int SequenceAnalyzer::mininum_points_matches = 20;
  int SequenceAnalyzer::mininum_image_matches = 2;


  SequenceAnalyzer::SequenceAnalyzer( 
    cv::Ptr<cv::FeatureDetector> feature_detector,
    cv::Ptr<cv::DescriptorExtractor> descriptor_extractor,
    cv::Ptr<PointsMatcher> match_algorithm )
    :match_algorithm_( match_algorithm ),
    feature_detector_( feature_detector ),
    descriptor_extractor_( descriptor_extractor )
  {

  }

  SequenceAnalyzer::SequenceAnalyzer( MotionProcessor input_sequence,
    cv::Ptr< cv::FeatureDetector > feature_detector,
    cv::Ptr< cv::DescriptorExtractor > descriptor_extractor,
    cv::Ptr< PointsMatcher > match_algorithm )
    :match_algorithm_( match_algorithm ),
    feature_detector_( feature_detector ),
    descriptor_extractor_( descriptor_extractor )
  {
    //only finite sequences can be used:
    CV_DbgAssert( input_sequence.isBidirectional( ) );
    //go back to the begining:
    input_sequence.setProperty( CV_CAP_PROP_POS_FRAMES,0 );

    //load entire sequence! Can be problematic but if a user want to have
    //more controls, he can use the other constructor...

    int nbFrame=0;
    Mat currentImage=input_sequence.getFrame( );
    while ( !currentImage.empty( ) )// && nbFrame<50 )
    {
      addImageToPipeline( currentImage );
      nbFrame++;
      currentImage=input_sequence.getFrame( );
    }
  }

  SequenceAnalyzer::SequenceAnalyzer(
    std::vector< cv::Ptr< PointsToTrack > > &points_to_track,
    std::vector< cv::Mat > *images,
    cv::Ptr<PointsMatcher> match_algorithm )
    :points_to_track_( points_to_track )
  {
    if( match_algorithm.empty() )
      match_algorithm_ = new PointsMatcher( 
      Ptr<cv::DescriptorMatcher>( new cv::FlannBasedMatcher( ) ) );
    else
      match_algorithm_ = match_algorithm;
    if( images != NULL)
      images_ = (*images);
  }

  //by default, use flann based matcher
  SequenceAnalyzer::SequenceAnalyzer( cv::FileNode file,
    std::vector<cv::Mat> *images,
    cv::Ptr<PointsMatcher> match_algorithm )
  {
    if( match_algorithm.empty() )
      match_algorithm_ = new PointsMatcher(
      Ptr<cv::DescriptorMatcher>( new cv::FlannBasedMatcher( ) ) );
    else
      match_algorithm_ = match_algorithm;
    if( images != NULL)
      images_ = (*images);
    read( file,*this );
  }

  SequenceAnalyzer::~SequenceAnalyzer( void )
  {
  }

  void SequenceAnalyzer::addImageToPipeline( cv::Mat image, cv::Ptr<PointsToTrack> points )
  {
    if( points.empty( ) )
    {
      CV_DbgAssert( !feature_detector_.empty( ) &&
        !descriptor_extractor_.empty( ) );
      int nbFrame = points_to_track_.size( );
      Ptr<PointsToTrack> ptrPoints_tmp( new PointsToTrackWithImage (
        nbFrame, image, feature_detector_, descriptor_extractor_ ));
      ptrPoints_tmp->computeKeypointsAndDesc( );

      points_to_track_.push_back( ptrPoints_tmp );
    }
    else
      points_to_track_.push_back( points );

    images_.push_back( image );
  }

  void SequenceAnalyzer::addImageToTracks( cv::Mat image, cv::Ptr<PointsToTrack> points )
  {
    if( points.empty( ) )
    {
      CV_DbgAssert( !feature_detector_.empty( ) &&
        !descriptor_extractor_.empty( ) );
      int nbFrame = points_to_track_.size( );
      points = new PointsToTrackWithImage (
        nbFrame, image, feature_detector_, descriptor_extractor_ );
      points->computeKeypointsAndDesc( );

      points_to_track_.push_back( points );
    }
    else
      points_to_track_.push_back( points );

    images_.push_back( image );

    //////////////////////////////////////////////////////////////////////////
    //First compute missing features descriptors:
    vector< Ptr< PointsToTrack > >::iterator matches_it =
      points_to_track_.begin( ),
      end_matches_it = points_to_track_.end( );

    Ptr<PointsMatcher> point_matcher = match_algorithm_->clone( true );
    point_matcher->add( points );
    point_matcher->train( );
    int i = 0;
    while ( matches_it != end_matches_it )
    {
      Ptr<PointsMatcher> point_matcher1 = match_algorithm_->clone( true );
      point_matcher1->add( points_to_track_[i] );
      point_matcher1->train( );


      std::vector< cv::DMatch > matches = SequenceAnalyzer::simple_matching(
        point_matcher, point_matcher1, 10 );

      i++;
      matches_it++;
    }

      //compute the color of each matches:
      unsigned int max_tracks = tracks_.size();
      for(unsigned int t=0;t<max_tracks; t++)
      {
        TrackOfPoints& tmp = tracks_[t];
        if(tmp.color == 0)
        {
          unsigned int max_points = tmp.point_indexes_.size();
          int R = 0, G = 0, B = 0;
          for(unsigned int j=0; j<max_points; ++j)
          {
            unsigned int img_idx = tmp.images_indexes_[j];
            unsigned int pt_idx = tmp.point_indexes_[j];

            unsigned int packed_color = points_to_track_[ img_idx ]->getColor( pt_idx );
            R += (packed_color>>16) & 0x000000FF;
            G += (packed_color>>8) & 0x000000FF;
            B += (packed_color) & 0x000000FF;
          }
          R /= max_points;
          G /= max_points;
          B /= max_points;
          tmp.color = (unsigned int)(
            ((R<<16) & 0x00FF0000) | ((R<<8) & 0x0000FF00)| (B & 0x000000FF));
        }
      }
      TrackOfPoints::fusionDuplicates( tracks_ );
      //////////////////////////////////////////////////////////////////////////
  }

  void SequenceAnalyzer::computeMatches( uchar nbMaxThread, bool printProgress )
  {
    //First compute missing features descriptors:
    vector< Ptr< PointsToTrack > >::iterator matches_it =
      points_to_track_.begin( ),
      end_matches_it = points_to_track_.end( );
    MatchingThread::size_list = points_to_track_.size();
    MatchingThread::match_algorithm = match_algorithm_;

    MatchingThread::matches_ = &points_to_track_;

    double nbMatches = points_to_track_.size();
    MatchingThread::total_matches = nbMatches * nbMatches / 2.0;
    MatchingThread::current_match_ = 0;
    MatchingThread::print_progress_ = printProgress;

    //then init the fundamental matrix list:
    list_fundamental_.clear();

    for( size_t cpt = 0; cpt<MatchingThread::size_list; ++cpt )
      list_fundamental_.push_back(
      vector< cv::Ptr<Mat> > ( MatchingThread::size_list - cpt + 1 ) );

    //Try to match each picture with other:
    vector<Mat> masks;

    MatchingThread::mininum_points_matches = mininum_points_matches;
    unsigned int nb_proc = MIN( nbMaxThread, boost::thread::hardware_concurrency() );
    INIT_SEMAPHORE( MatchingThread::thread_concurr, nb_proc );
    INIT_MUTEX( MatchingThread::thread_unicity );

    unsigned int i=0;

    while ( matches_it != end_matches_it )
    {
      //can we start a new thread?
      P_MUTEX( MatchingThread::thread_concurr );
      //create local values for the thead:
      MatchingThread match_thread(this, i );
      //start the thread:
      boost::thread myThread(match_thread);

      i++;
      matches_it++;
    }
    for(unsigned int wait_endThread = 0;
      wait_endThread<nb_proc ; ++wait_endThread)
      P_MUTEX( MatchingThread::thread_concurr );//wait for last threads

    //compute the color of each matches:
    unsigned int max_tracks = tracks_.size();
    for(unsigned int t=0;t<max_tracks; t++)
    {
      TrackOfPoints& tmp = tracks_[t];
      unsigned int max_points = tmp.point_indexes_.size();
      int R = 0, G = 0, B = 0;
      for(unsigned int j=0; j<max_points; ++j)
      {
        unsigned int img_idx = tmp.images_indexes_[j];
        unsigned int pt_idx = tmp.point_indexes_[j];

        unsigned int packed_color = points_to_track_[ img_idx ]->getColor( pt_idx );
        R += (packed_color>>16) & 0x000000FF;
        G += (packed_color>>8) & 0x000000FF;
        B += (packed_color) & 0x000000FF;
      }
      R /= max_points;
      G /= max_points;
      B /= max_points;
      tmp.color = (unsigned int)(
        ((R<<16) & 0x00FF0000) | ((R<<8) & 0x0000FF00)| (B & 0x000000FF));
    }
    TrackOfPoints::fusionDuplicates( tracks_ );
  }

  void SequenceAnalyzer::keepOnlyCorrectMatches(
    std::vector<TrackOfPoints>& tracks,
    unsigned int min_matches, unsigned int min_consistance )
  {
    unsigned int tracks_size = tracks.size( );
    unsigned int index=0;

    while ( index < tracks_size )
    {
      if( ( tracks[ index ].getNbTrack( ) < min_matches ) ||
        ( tracks[ index ].track_consistance < (int)min_consistance ) )
      {
        //problem with this track, too small to be consistent
        // or inconsistant...
        tracks_size--;
        tracks[ index ]=tracks[ tracks_size ];
        tracks.pop_back( );
        index--;
      }
      index++;
    }
  }

  void SequenceAnalyzer::addMatches( std::vector< cv::DMatch > &newMatches,
    unsigned int img1, unsigned int img2 )
  {
    //add to tracks_ the new matches:

    vector<DMatch>::iterator match_it = newMatches.begin( );
    vector<DMatch>::iterator match_it_end = newMatches.end( );

    while ( match_it != match_it_end )
    {
      DMatch &point_matcher = ( *match_it );

      bool is_found=false;
      vector<TrackOfPoints>::iterator tracks_it = tracks_.begin( );
      while ( tracks_it != tracks_.end( ) && !is_found )
      {
        TrackOfPoints& track = ( *tracks_it );

        if( track.containPoint( img1,point_matcher.trainIdx ))
        {
          track.addMatch( img2,point_matcher.queryIdx );
          is_found=true;
        }
        else
        {
          if( track.containPoint( img2,point_matcher.queryIdx ))
          {
            track.addMatch( img1,point_matcher.trainIdx );
            is_found=true;
          }
        }
        tracks_it++;
      }
      if( !is_found )
      {
        //it's a new point match, create a new track:
        TrackOfPoints newTrack;
        newTrack.addMatch( img1,point_matcher.trainIdx );
        newTrack.addMatch( img2,point_matcher.queryIdx );
        tracks_.push_back( newTrack );
      }

      match_it++;
    }
  }

  void SequenceAnalyzer::addTracks( std::vector< TrackOfPoints > &newTracks )
  {

    vector<TrackOfPoints>::iterator match_it = newTracks.begin( ),
      match_it_end = newTracks.end( );

    while ( match_it != match_it_end )
    {
      tracks_.push_back( *match_it );

      match_it++;
    }
  }

  void SequenceAnalyzer::showTracks( int timeBetweenImg )
  {
    if( points_to_track_.size( ) == 0 )
      return;//nothing to do...

    unsigned int it=0,it1=0;
    unsigned int end_iter = points_to_track_.size( ) - 1 ;
    if( images_.size( ) - 1 < end_iter )
      end_iter = images_.size( ) - 1;
    while ( it < end_iter )
    {
      it1=it+1;
      while ( it1 < end_iter )
      {
        vector<DMatch> matches_to_print;
        //add to matches_to_print only points of img it and it+1:

        vector<TrackOfPoints>::iterator match_it = tracks_.begin( );
        vector<TrackOfPoints>::iterator match_it_end = tracks_.end( );

        while ( match_it != match_it_end )
        {
          if( match_it->containImage( it ) &&
            match_it->containImage( it1 ) )
          {
            matches_to_print.push_back( match_it->toDMatch( it,it1 ));
          }
          match_it++;
        }

        if( matches_to_print.size()>0 )
        {
          Mat firstImg=images_[ it ];
          Mat outImg;

          PointsMatcher::drawMatches( firstImg, points_to_track_[ it ]->getKeypoints( ),
            points_to_track_[ it1 ]->getKeypoints( ),
            matches_to_print, outImg,
            cv::Scalar::all( -1 ), cv::Scalar::all( -1 ), vector<char>( ),
            cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );

          imshow( "showTracks",outImg );
          cv::waitKey( timeBetweenImg );
        }

        it1++;
      }
      it++;
    }
    cvDestroyWindow( "showTracks" );
  }

  void SequenceAnalyzer::showTracks( int img_to_show, int timeBetweenImg )
  {
    unsigned int it=0,it1=0;
    unsigned int end_iter = points_to_track_.size( ) - 1 ;
    if( images_.size( ) - 1 < end_iter )
      end_iter = images_.size( ) - 1;
    vector< vector<DMatch> > matches_to_print;
    matches_to_print.assign( points_to_track_.size( ), vector<DMatch>() );
    //add to matches_to_print only points of img it and it+1:

    vector<TrackOfPoints>::iterator match_it = tracks_.begin( );
    vector<TrackOfPoints>::iterator match_it_end = tracks_.end( );
    unsigned int i = 0;
    while ( match_it != match_it_end )
    {
      if( match_it->containImage( img_to_show ) )
      {
        for(i = 0; i<match_it->images_indexes_.size(); i++)
        {
          if(match_it->images_indexes_[i] != img_to_show)
          {
            matches_to_print[ match_it->images_indexes_[i] ].
              push_back( match_it->toDMatch( img_to_show, match_it->images_indexes_[i] ));
          }
        }
      }
      match_it++;
    }

    for(i = 0; i<matches_to_print.size(); i++)
    {
      if( matches_to_print[i].size()>0 )
      {
        Mat firstImg=images_[ it ];
        Mat outImg;

        PointsMatcher::drawMatches( firstImg, points_to_track_[ img_to_show ]->getKeypoints( ),
          points_to_track_[ i ]->getKeypoints( ),
          matches_to_print[i], outImg,
          cv::Scalar::all( -1 ), cv::Scalar::all( -1 ), vector<char>( ),
          cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );

        imshow( "showTracks",outImg );
        cv::waitKey( timeBetweenImg );
      }
    }
    cvDestroyWindow( "showTracks" );
  }

  cv::Mat SequenceAnalyzer::showTracksBetween( unsigned int img1, unsigned int img2,
    cv::Mat img, bool should_print )
  {
    CV_Assert( points_to_track_.size( ) != 0 );
    CV_Assert( images_.size( ) > img1 && images_.size( ) > img2 );
    CV_Assert( 0 <= img1 &&  0 <= img2 );

    vector<DMatch> matches_to_print,matches_to_print1;
    //add to matches_to_print only points of img1 and img2:

    vector<TrackOfPoints>::iterator match_it = tracks_.begin( );
    vector<TrackOfPoints>::iterator match_it_end = tracks_.end( );

    while ( match_it != match_it_end )
    {
      if( match_it->containImage( img1 ) &&
        match_it->containImage( img2 ) )
      {
        matches_to_print.push_back( match_it->toDMatch( img1,img2 ));
        matches_to_print1.push_back( match_it->toDMatch( img2,img1 ));
      }
      match_it++;
    }
    if( img.empty() )
      img = images_[ img1 ];
    Mat outImg,outImg1;
    PointsMatcher::drawMatches( img, points_to_track_[ img1 ]->getKeypoints( ),
      points_to_track_[ img2 ]->getKeypoints( ),
      matches_to_print, outImg,
      cv::Scalar::all( -1 ), cv::Scalar::all( -1 ), vector<char>( ),
      cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );
    if(should_print)
      imshow( "showTracks Img1-Img2",outImg );

    if(should_print)
    {
    PointsMatcher::drawMatches( images_[ img2 ], points_to_track_[ img2 ]->getKeypoints( ),
      points_to_track_[ img1 ]->getKeypoints( ),
      matches_to_print1, outImg1,
      cv::Scalar::all( -1 ), cv::Scalar::all( -1 ), vector<char>( ),
      cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );

      imshow( "showTracks Img2-Img1",outImg1 );
      cv::waitKey( 0 );
    }

    cvDestroyAllWindows();
    return outImg;
  }

  void SequenceAnalyzer::read( const cv::FileNode& node, SequenceAnalyzer& me )
  {
    std::string myName=node.name( );
    if( myName != "SequenceAnalyzer" )
    {
      std::string error = "FileNode is not correct!\nExpected \"SequenceAnalyzer\", got ";
      error += node.name();
      CV_Error( CV_StsError, error.c_str() );
    }
    if( node.empty( ) || !node.isMap( ) )
      CV_Error( CV_StsError, "SequenceAnalyzer FileNode is not correct!" );

    int nb_pictures = ( int ) node[ "nbPictures" ];
    //initialisation of all empty vectors
    for( int i=0; i<nb_pictures; i++ )
    {
      Ptr<PointsToTrack> ptt;
      if( i<me.images_.size() )
      {
        ptt = Ptr<PointsToTrack>( 
          new PointsToTrackWithImage( i, me.images_[i] ));
      }
      else
      {
        ptt = Ptr<PointsToTrack>( new PointsToTrack( i ));
      }
      me.points_to_track_.push_back( ptt );

      Ptr<PointsMatcher> p_m = Ptr<PointsMatcher>( new PointsMatcher(
        *me.match_algorithm_ ) );
      p_m->add( ptt );

      me.matches_.push_back( p_m );
    }

    cv::FileNode node_TrackPoints = node[ "TrackPoints" ];

    //tracks are stored in the following form:
    //list of track where a track is stored like this:
    // nbPoints idImage1 point1  idImage2 point2 ...
    if( node_TrackPoints.empty( ) || !node_TrackPoints.isSeq() )
      CV_Error( CV_StsError, "SequenceAnalyzer FileNode is not correct!" );
    cv::FileNodeIterator it = node_TrackPoints.begin( ),
      it_end = node_TrackPoints.end( );
    while( it != it_end )
    {
      cv::FileNode it_track = ( *it )[ 0 ];
      int nbPoints,track_consistance;
      it_track[ "nbPoints" ] >> nbPoints;
      it_track[ "track_consistance" ] >> track_consistance;
      bool has_3d_point = false;
      it_track[ "has_3d_position" ] >> has_3d_point;
      TrackOfPoints track;
      if( has_3d_point )
      {
        cv::Vec3d point;
        point[ 0 ] = it_track[ "point3D_triangulated" ][ 0 ];
        point[ 1 ] = it_track[ "point3D_triangulated" ][ 1 ];
        point[ 2 ] = it_track[ "point3D_triangulated" ][ 2 ];
        track.point3D = Ptr<cv::Vec3d>( new cv::Vec3d( point ) );
      }
      int color;
      it_track[ "color" ] >> color;
      track.setColor( *((unsigned int*)&color) );
      cv::FileNodeIterator itPoints = it_track[ "list_of_points" ].begin( ),
        itPoints_end = it_track[ "list_of_points" ].end( );
      while( itPoints != itPoints_end )
      {
        int idImage;
        cv::KeyPoint kpt;
        idImage = ( *itPoints )[ 0 ];
        itPoints++;
        kpt.pt.x = ( *itPoints )[ 0 ];
        kpt.pt.y = ( *itPoints )[ 1 ];
        kpt.size = ( *itPoints )[ 2 ];
        kpt.angle = ( *itPoints )[ 3 ];
        kpt.response = ( *itPoints )[ 4 ];
        kpt.octave = ( *itPoints )[ 5 ];
        kpt.class_id = ( *itPoints )[ 6 ];

        unsigned int point_index = me.points_to_track_[ idImage ]->
          addKeypoint( kpt );
        track.addMatch( idImage,point_index );

        itPoints++;
      }
      track.track_consistance = track_consistance;
      me.tracks_.push_back( track );
      it++;
    }
  }

  void SequenceAnalyzer::write( cv::FileStorage& fs, const SequenceAnalyzer& me )
  {
    vector<TrackOfPoints>::size_type key_size = me.tracks_.size( );
    int idImage=-1, idPoint=-1;

    fs << "SequenceAnalyzer" << "{";
    fs << "nbPictures" << ( int )me.points_to_track_.size( );
    fs << "nbPoints" << ( int )key_size;
    fs << "TrackPoints" << "[";
    for ( vector<TrackOfPoints>::size_type i=0; i < key_size; i++ )
    {
      const TrackOfPoints &track = me.tracks_[ i ];
      unsigned int nbPoints = track.getNbTrack( );
      if( nbPoints > 0 )
      {
        fs << "{" << "nbPoints" << ( int )nbPoints;
        fs << "track_consistance" << track.track_consistance;
        fs << "has_3d_position" << ( !track.point3D.empty( ) );
        if( !track.point3D.empty( ) )
          fs << "point3D_triangulated" << *(track.point3D);

        unsigned int real_color = track.getColor();
        int color = *((int*)&real_color);
        fs << "color" << color;

        fs << "list_of_points" << "[:";
        nbPoints = track.images_indexes_.size();
        for ( unsigned int j = 0; j < nbPoints ; j++ )
        {
          if( track.good_values[j] )
          {
            idImage = track.images_indexes_[ j ];
            idPoint = track.point_indexes_[ j ];
            if( idImage>=0 && idPoint>=0 )
            {
              fs << idImage;
              fs  << "[:";

              const cv::KeyPoint kpt = me.points_to_track_[ idImage ]->
                getKeypoints( )[ idPoint ];
              cv::write( fs, kpt.pt.x );
              cv::write( fs, kpt.pt.y );
              cv::write( fs, kpt.size );
              cv::write( fs, kpt.angle );
              cv::write( fs, kpt.response );
              cv::write( fs, kpt.octave );
              cv::write( fs, kpt.class_id );
              fs << "]" ;
            }
          }
        }
        fs << "]" << "}" ;
      }
    }
    fs << "]" << "}";
  }

  void SequenceAnalyzer::constructImagesGraph( )
  {
    images_graph_.initStructure( points_to_track_.size( ) );

    //for each points:
    vector<TrackOfPoints>::size_type key_size = tracks_.size( );
    vector<TrackOfPoints>::size_type i;

    for ( i=0; i < key_size; i++ )
    {
      TrackOfPoints &track = tracks_[ i ];
      unsigned int nviews = track.images_indexes_.size( );

      for( unsigned int cpt=0;cpt<nviews;cpt++ )
      {
        unsigned int imgSrc = track.images_indexes_[ cpt ];
        for( unsigned int cpt1=cpt+1;cpt1<nviews;cpt1++ )
        {
          images_graph_.addLink( imgSrc, track.images_indexes_[ cpt1 ] );
        }
      }
    }
  }

  std::vector< cv::Vec3d > SequenceAnalyzer::get3DStructure( )
  {
    vector<cv::Vec3d> out_vector;
    vector<TrackOfPoints>::iterator itTrack=tracks_.begin( );
    while ( itTrack != tracks_.end( ) )
    {
      if( !itTrack->point3D.empty( ) )
        out_vector.push_back( ( cv::Vec3d )( *itTrack ) );
      itTrack++;
    }
    return out_vector;
  }

  std::vector< unsigned int > SequenceAnalyzer::getColors( )
  {
    vector<unsigned int> out_vector;
    vector<TrackOfPoints>::iterator itTrack=tracks_.begin( );
    while ( itTrack != tracks_.end( ) )
    {
      if( !itTrack->point3D.empty( ) )
        out_vector.push_back( itTrack->getColor() );
      itTrack++;
    }
    return out_vector;
  }

  void SequenceAnalyzer::showPointsOnImage(unsigned int i,
    const std::vector<cv::Vec2d>& pixelProjection)
  {
    CV_Assert( i < images_.size() );
    //convert Vec2D into Keypoints:
    std::vector<KeyPoint> keypoints;
    for(size_t cpt = 0; cpt<pixelProjection.size(); ++cpt)
    {
      keypoints.push_back( cv::KeyPoint( (float)pixelProjection[ cpt ][0],
        (float)pixelProjection[ cpt ][1], 1.0 ) );
    }
    cv::Mat outImg;
    cv::drawKeypoints( images_[i], keypoints, outImg );
    cv::imshow( "Keypoints", outImg );
    cv::waitKey( 0 );
    cv::destroyWindow( "Keypoints" );
  }

  std::vector< cv::DMatch > SequenceAnalyzer::simple_matching(
    cv::Ptr<PointsMatcher> point_matcher,
    cv::Ptr<PointsMatcher> point_matcher1,
    unsigned int mininum_points_matches)
  {
    vector< cv::DMatch > matches_i_j;
    point_matcher->crossMatch( point_matcher1, matches_i_j );

    //First compute points matches:
    unsigned int size_match=matches_i_j.size( );
    vector<cv::Point2f> srcP;
    vector<cv::Point2f> destP;
    vector<uchar> status;

    //vector<KeyPoint> points1 = point_matcher->;
    for( size_t cpt = 0; cpt < size_match; ++cpt ){
      const cv::KeyPoint &key1 = point_matcher1->getKeypoint(
        matches_i_j[ cpt ].queryIdx );
      const cv::KeyPoint &key2 = point_matcher->getKeypoint(
        matches_i_j[ cpt ].trainIdx );
      srcP.push_back( cv::Point2f( key1.pt.x,key1.pt.y ) );
      destP.push_back( cv::Point2f( key2.pt.x,key2.pt.y ) );
      status.push_back( 1 );
    }

    //free some memory:
    point_matcher->clear();
    point_matcher1->clear();

    if( srcP.size()< mininum_points_matches )
      return matches_i_j;
    cv::Mat fundam = cv::findFundamentalMat( srcP, destP, status, cv::FM_RANSAC, 1 );

    unsigned int nbErrors = 0, nb_iter=0;
    //refine the mathing :
    size_match = status.size( );
    for( size_t cpt = 0; cpt < size_match; ++cpt ){
      if( status[ cpt ] == 0 )
      {
        size_match--;
        status[ cpt ] = status[ size_match ];
        status.pop_back( );
        srcP[ cpt ] = srcP[ size_match ];
        srcP.pop_back( );
        destP[ cpt ] = destP[ size_match ];
        destP.pop_back( );
        matches_i_j[ cpt ] = matches_i_j[ size_match ];
        matches_i_j.pop_back( );
        cpt--;
        ++nbErrors;
      }
    }

    if( srcP.size()< mininum_points_matches )
      return matches_i_j;

    //refine the mathing:
    fundam = cv::findFundamentalMat( srcP, destP, status, cv::FM_LMEDS );

    size_match = status.size( );
    for( size_t cpt = 0; cpt < size_match; ++cpt ){
      if( status[ cpt ] == 0 )
      {
        size_match--;
        status[ cpt ] = status[ size_match ];
        status.pop_back( );
        srcP[ cpt ] = srcP[ size_match ];
        srcP.pop_back( );
        destP[ cpt ] = destP[ size_match ];
        destP.pop_back( );
        matches_i_j[ cpt ] = matches_i_j[ size_match ];
        matches_i_j.pop_back( );
        cpt--;
        ++nbErrors;
      }
    }
    return matches_i_j;
  };


  void SequenceAnalyzer::removePointsWithoutProjection(
    SequenceAnalyzer &motion_estim )
  {
    vector<TrackOfPoints>::size_type key_size = motion_estim.tracks_.size( );
    int idImage=-1, idPoint=-1;
    vector< Ptr<PointsToTrack> > new_ptt;
    //initialisation of empty vector
    int before = 0, after = 0;
    for( size_t i=0; i<motion_estim.points_to_track_.size(); i++ )
    {
      Ptr<PointsToTrack> ptt = Ptr<PointsToTrack>(
        new PointsToTrackWithImage( i, motion_estim.images_[i],
        motion_estim.feature_detector_, motion_estim.descriptor_extractor_ ));
      new_ptt.push_back( ptt );
      before += motion_estim.points_to_track_[i]->getKeypoints().size();
    }
    for ( size_t i=0; i < key_size; i++ )
    {
      TrackOfPoints &track = motion_estim.tracks_[ i ];
      unsigned int nbPoints = track.getNbTrack( );
      if( nbPoints > 0 && !track.point3D.empty( ) )
      {
        nbPoints = track.images_indexes_.size();
        for ( unsigned int j = 0; j < nbPoints ; j++ )
        {
          if( track.good_values[j] )
          {
            idImage = track.images_indexes_[ j ];
            idPoint = track.point_indexes_[ j ];
            if( idImage>=0 && idPoint>=0 )
            {

              const cv::KeyPoint kpt = 
                motion_estim.points_to_track_[ idImage ]->
                getKeypoints( )[ idPoint ];
              track.point_indexes_[ j ] = new_ptt[ idImage ]->addKeypoint( kpt );
            }
          }
        }
      }
    }
    motion_estim.points_to_track_ = new_ptt;
    for( size_t i=0; i<motion_estim.points_to_track_.size(); i++ )
    {
      Ptr<PointsToTrack> ptt = Ptr<PointsToTrack>(
        new PointsToTrackWithImage( i, motion_estim.images_[i] ));
      new_ptt.push_back( ptt );
      after += motion_estim.points_to_track_[i]->getKeypoints().size();
    }
    std::cout<<" before "<<before<<", after "<<after<<std::endl;
  }
}
