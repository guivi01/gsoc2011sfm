This wiki page is not the API documentation. Indeed, Doxygen will do it for us. It's only for sharing ideas...

# Role #

Represent a field of view : a 3D position and direction of a [Camera](Camera.md). So only extern parameters should be present in attribute and a reference to the Camera which take the picture.

There is two major scenarios:
  * a single camera take all FoV.
  * a different camera for each FoV.


# Attributes #
```
cv::Mat Rotation;
cv::Mat Translation;
cv::Mat ProjMatrix;//redundancy but speed improvement
cv::Ptr<Camera> device;//intra parameters
```
# Methods #