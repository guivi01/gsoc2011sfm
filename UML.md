<h1>OpenCV & Structure from motion</h1>
<h2>Ideas for implementation</h2>

To be consistent with what has been stated above, I identified five processing blocks that can be separated:<br />
> <ul>
<blockquote><li> PointsToTrack: given an image, this block must be able to find points and/or features to track. OpenCV has already a lot of traitments, just have to create wrappers...</li>
<li> PointsMatched: given points and/or features, this block must be able to find correspondances. If only images are known, use first <i>PointsToTrack</i> to find points... OpenCV has a lot of algo too, so not a lot of work.</li>
<li> ExternPosEstim: given points and/or intra parameters of cameras and/or matches and/or known 3D points, this block have to estimate the extern parameters of cameras (position, orientation) and intra parameters if needed. Same as before, a lot of methods exist in OpenCV or using toolbox of Mr Rabaud!</li>
<li> Points3DEstim: given 2D points and/or correspondances and/or intra parameters and/or extern parameters and/or known 3D points, this block have to estimate the 3D positions of each 2D points sets. Various methods exists (Middle point, iterative least square...), choice of methods has to be done.</li>
<li> Object: given 3D points from differents field of view and/without correspondances, find commun objects between views. A lot of algorithms exist in PCL, so just need wrappers...<br>
</blockquote><blockquote></li>
</blockquote><blockquote></ul></blockquote>


<h2>Examples</h2><br />
As the library is under development, for now we don't have working example.

I hope we will have something nice to show here ;)