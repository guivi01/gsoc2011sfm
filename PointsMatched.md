This wiki page is not the API documentation. Indeed, Doxygen will do it for us. It's only for sharing ideas...

# Role #

The aim of this class is to find good matches between points of FieldOfView.
We have to take into account:
  * Possibility to match 2/more different views
  * Possibility to use features of point when available
  * Use the fundamental matrix to improve the matching estimation

# Attributes #
```
vector<FieldOfView> foVs;
float* matches;//Array of 2D points (idea from Bundler)
```

# Ideas of implementation #

_Ideas after Bundler and SBA code analysis_

The points matches should be stored using a structure like this :

|P<sub>1</sub>Cam<sub>1</sub>_X_|P<sub>1</sub>Cam<sub>1</sub>_Y_|P<sub>1</sub>Cam<sub>2</sub>_X_|P<sub>1</sub>Cam<sub>2</sub>_Y_|...|P<sub>1</sub>Cam<sub>N</sub>_X_|P<sub>1</sub>Cam<sub>N</sub>_Y_|
|:------------------------------|:------------------------------|:------------------------------|:------------------------------|:--|:------------------------------|:------------------------------|
|P<sub>2</sub>Cam<sub>1</sub>_X_|P<sub>2</sub>Cam<sub>1</sub>_Y_|P<sub>2</sub>Cam<sub>2</sub>_X_|P<sub>2</sub>Cam<sub>2</sub>_Y_|...|P<sub>2</sub>Cam<sub>N</sub>_X_|P<sub>2</sub>Cam<sub>N</sub>_Y_|
|...                            |...                            |...                            |...                            |...|...                            |...                            |
|P<sub>M</sub>Cam<sub>1</sub>_X_|P<sub>M</sub>Cam<sub>1</sub>_Y_|P<sub>M</sub>Cam<sub>2</sub>_X_|P<sub>M</sub>Cam<sub>2</sub>_Y_|...|P<sub>M</sub>Cam<sub>N</sub>_X_|P<sub>M</sub>Cam<sub>N</sub>_Y_|

Of course, this implementation can be done using a 1D array.

This will of course take a lot of memory, but will be OK for classical computer hardware (if we have M points=50000 and N cameras=100, this will use `2*4*50000*100=38` Mo.

As the `50 000` points are not in all pictures, we can expect to gain memory using some compression because lot of points in the vector are undefined. If this is a needed feature, we can use the following idea:

Undefined points are set below to 0 and equal to the cardinal of the sequence of undefined points. For example if cam1 see only P1, P2 in position (20,25) and (30,35), cam2 see P1(30,20), P2(20,40), P3(70,20) and P4 in (120,130) and Cam3 see only P2 in (50,100) and P3 in (20,20), the first version of vector should have done this:
|20|25|30|35|-1|-1|-1|-1|30|20|20|40|70|20|120|130|-1|-1|50|100|20|20|-1|-1| |<b>24</b>|
|:-|:-|:-|:-|:-|:-|:-|:-|:-|:-|:-|:-|:-|:-|:--|:--|:-|:-|:-|:--|:-|:-|:-|:-|:|:--------|
|P1|  |P2|  |P3|  |P4|  |P1|  |P2|  |P3|  |P4 |   |P1|  |P2|   |P3|  |P4|  | |<b>Values</b>|

But with compacted values, the vector is:
|20|25|30|35|-4|30|20|20|40|70|20|120|130|-2|50|100|20|20|-2| |<b>19</b>|
|:-|:-|:-|:-|:-|:-|:-|:-|:-|:-|:-|:--|:--|:-|:-|:--|:-|:-|:-|:|:--------|
|P1|  |P2|  |X |P1|  |P2|  |P3|  |P4 |   |X |P2|   |P3|  |X | |<b>Values</b>|

Of course, if we want to use centered coordinates, we have to change a little bit this representation... But the idea is still here: the sequence of undefined points should be compressed.