This wiki page is not the API documentation. Indeed, Doxygen will do it for us. It's only for sharing ideas...

# Role #

The aim of this class is to find good features to track. Theses features can be computed using various methods or loaded from files (XML, PCD...).

The points can be expressed using normalized or image coordinates.

# Attributes #
```
vector<cv::KeyPoint> keypoints;
cv::Mat features;
FieldOfView foV;
```