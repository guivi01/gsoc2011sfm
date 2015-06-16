# Background #

  * Difficulties to work with libmv team
  * Only 1 month left for my GSoC
  * Want to have a wahoo object to show ;)
  * Thanks to my thesis I have already done some work on multiple image fusion

# What can be useful in my previous work #

The idea below my superresolution algorithm is that if we choose the
projective model as motion transformation, this is sufficient to model every camera motion as the object is a flat surface. That's the model we use and every 2D point we see are supposed to have the 3rd coordinates equal to 0.

Then the cameras positions are easy to get (just a camera resectioning...) and we recompute the 3D flat surface by accumulating the value extracted form each pixel (we call this value Expectation, but in our case this is the RGB value).

# What can we already do #

We can find 2D points, match them in the entire sequence in a robust way (but can be improved), load cameras parameters and finally triangulate the points.

# What I propose to do #

## Short term (1-2 week) ##
First simply create a graph of close 3D points (using the classic radius search) for each 3D points.

Remove links which are just lines (i.e. one of extremity as only one link)

Remove links when it is not correctly showed on 2D images (we can for example create a 3D point between these 2 3D points, and if this new 3D point is not correctly projected into the 2D images, we remove this link)

Remove links which are just lines (i.e. one of extremity has only one link)


And voila, we should have a mesh with of course some gap and without textures.

## Long term (1month +) ##
### Adding textures ###
Project image on mesh and create a super-texture for each polygon (memory extensive approach).
Find the best image for each polygon and store is reference.

### Improve mesh ###
Using PCL methods, improve the mesh (smoother, fill holes), then using images create a displacement texture for each polygon based on reprojection differences.