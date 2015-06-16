# Introduction #

The SfM visualization will be done using the [PCL library](http://pointclouds.org/).

To be able to easily switch between one and other, I made some conversion functions and structures.

# Convert with zero data copy #

This conversion is not available for every conversions. Indeed cv::Vec3f for example has 3 float while pcl::PointXYZ need 4 float! So you will not being able to convert a Vec3f into a PointXYZ directly... But you can do the opposite conversion! That's this conversion we will present here:
```
  pcl::PointXYZ my_PCL_Point;//this point comes from anywhere
  Point3D_mapping convertor( my_PCL_Point );
  cv::Vec3f& cv_vector = convertor;
  //We can now play with cv_vector and every changes will affect the PCL point (and vice versa)!
```

# Convert with data copy #
Sometimes you don't care of memory copies (for example when conversion is done once and old data are freed right after conversion).

You can do something like that (if you want to convert from Vec3f to PointXYZ):
```
  Point3D_mapping convertor;
  cv::Vec3f& getCV_Vect = convertor;
  
  //Get some openCV datas:
  getCV_Vect = vec3f_from_somewhere;
  //Put them into a PCL point:
  pcl::PointXYZ my_PCL_Point = convertor;//here my_PCL_Point has it's own memory so changes won't affect getCV_Vect!
```
Remeber that `sizeof(Vec3f) < sizeof(PointXYZ)` so direct conversion was not possible! But if we want to convert a Vec4f, we could also do:
```
  cv::Vec4f getCV_Vect;//get data from somewhere;
  Point3D_mapping convertor(getCV_Vect);
  
  //Put them into a PCL point:
  pcl::PointXYZ my_PCL_Point = convertor;//here my_PCL_Point has it's own memory so changes won't affect getCV_Vect!
```
# Conversion of arrays #
For now (as it's just the beginning) we can just convert to and from a mapped vector to an other vector using `convertToMappedVector` and `convertFromMappedVector`, but soon we will be able to use InputArray (and OutputArray), I'm working on such conversions...
```
  vector<cv::Vec4f> my_vector;//get data from somewhere;
  vector<Point3D_mapping> my_generic_points;
  convertToMappedVector( my_vector, my_generic_points );

  vector<pcl::PointXYZ> my_vector_converted;
  convertFromMappedVector( my_generic_points, my_vector_converted );
```