<img src='http://charlie-soft.com/Structure%20from%20motion/opencvLogoFinal.png' alt='Structure from motion' align='left' width='100px' />
<h1>OpenCV & Structure from motion</h1>
<h2>Overall presentation</h2>
<p><img src='http://charlie-soft.com/Structure%20from%20motion/degrad.png' alt='Structure from motion' align='right' />
To allow use of simple but scalable API, the implementation will use abstract classes.<br />
This separation between concept and implementation should allow to modify the treatments for various processing blocks. Before detailing the proposed implementation, we must try to identify the different needs and constraints.<br /><br />
After some research on the topic, it appears that the API should at least allow users to:</p>
<ul>
<blockquote><li>Manage one or several cameras (I mean physical device) in a sequence (stereovision, single camera, multivision...).</li>
<li>Each camera can be of different type (Fisheye, with/without radial distortion, various intra-parameters...).</li>
<li>Initialize the different processing blocks according to the data availables:<ul>
<blockquote><li>Camera: Distortion, intra parameters, nothing, ...</li>
<li>Field of view: Extern position, points of interest, a known 3D pattern to match, 2D images, ...</li>
</blockquote><blockquote></ul>
</li>
</blockquote><li>Compute missing data (intra/extra parameters, 3D points,...)</li>
<li>Show the points cloud using an interactive visualization</li>
</ul>
<p>This GSoC will try to produce <strong>an high-quality dense 3D reconstruction for rigid models</strong>. Sub-pixels points detections, robust transformations estimations and sub-pixel dense displacement computation will be implemented into an <strong>efficient and scalable</strong> API.<br /><br />
<img src='http://charlie-soft.com/Structure%20from%20motion/exemple.png' alt='Example' />
<img src='http://charlie-soft.com/Structure%20from%20motion/exemple1.png' alt='Example1' align='right' /></p></blockquote>

Also you will use [the documentation of each classes](http://www.charlie-soft.com/Structure%20from%20motion/Doc/) generated thanks to [doxygen](http://www.stack.nl/~dimitri/doxygen/index.html).

I made a little video to show current reconstruction progress. This is not really a structure from motion as the cameras are fully parameterized, but it's a start:

<a href='http://www.youtube.com/watch?feature=player_embedded&v=9M4KWgRGNa0' target='_blank'><img src='http://img.youtube.com/vi/9M4KWgRGNa0/0.jpg' width='425' height=344 /></a>

The dependencies of this API are for now:
  * [Opencv](http://opencv.willowgarage.com)
  * [PCL (Point Cloud Library)](http://pointclouds.org)
  * [The libmv project](http://code.google.com/p/libmv/)
  * [The Eigen library (Needed by PCL and LibMV)](http://eigen.tuxfamily.org/)
  * [The Boost libraries](http://www.boost.org/)
  * [clapack](http://www.netlib.org/clapack/)
  * [lourakis' sba](http://www.ics.forth.gr/~lourakis/sba/)
    * [clapack for windows](http://icl.cs.utk.edu/lapack-for-windows/clapack/index.html)
    * [lapack fortran version](http://www.netlib.org/lapack/)