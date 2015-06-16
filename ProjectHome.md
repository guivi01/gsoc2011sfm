<img src='http://charlie-soft.com/Structure%20from%20motion/opencvLogoFinal.png' alt='Structure from motion' align='left' width='100px' />
<h1>OpenCV & Structure from motion</h1>
<h2>Overall presentation</h2>
<p><img src='http://charlie-soft.com/Structure%20from%20motion/degrad.png' alt='Structure from motion' align='right' />
This project was started in 2011 during the "google summer of code", <a href='http://opencv.willowgarage.com/wiki/OpenCVandSFM'>in partnership with OpenCV</a>.<br>
<br>
<br>
For more information, please use the <a href='ProjectPresentation.md'>Wiki</a> which try to present the different choices we make when building this API!<br>
<br>
Also you will use <a href='http://www.charlie-soft.com/Structure%20from%20motion/Doc/annotated.html'>the documentation of each classes</a> generated thanks to <a href='http://www.stack.nl/~dimitri/doxygen/index.html'>doxygen</a>.<br>
<br>
I made a little video to show current reconstruction progress. This is not really a structure from motion as the cameras are fully parameterized, but it's a start:<br>
<br>
<a href='http://www.youtube.com/watch?feature=player_embedded&v=9M4KWgRGNa0' target='_blank'><img src='http://img.youtube.com/vi/9M4KWgRGNa0/0.jpg' width='425' height=344 /></a><br>
<br>
The dependencies of this API are for now:<br>
<ul>
<li> <a href='http://opencv.willowgarage.com'>Opencv</a>
</li><li> <a href='http://pointclouds.org'>PCL (Point Cloud Library)</a>
</li><li> <a href='http://code.google.com/p/libmv/'>The libmv project</a>
</li><li> <a href='http://eigen.tuxfamily.org/'>The Eigen library (Needed by PCL and LibMV)</a>
</li><li> <a href='http://www.boost.org/'>The Boost libraries</a>
</li><li> <a href='http://www.netlib.org/clapack/'>clapack</a>
</li><li> <a href='http://www.ics.forth.gr/~lourakis/sba/'>lourakis' sba</a>
</li></ul>