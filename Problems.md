# Bundle adjustement #

When using a few matches (approx 80) between two cameras, the bundle adjustement find a strange solution: the two cameras are really close... I made a video to show this:

<a href='http://www.youtube.com/watch?feature=player_embedded&v=YWinHaZUvw8' target='_blank'><img src='http://img.youtube.com/vi/YWinHaZUvw8/0.jpg' width='425' height=344 /></a>

As you see, before bundle adjustment, the configuration is close to the correct one, but after the bundle nothing is good :(

I think it's due to the object's shape whose close to a plane... If someone has already had this issue, please tell me, this will help me!

# KRT\_from\_P problems #

Thanks to [Julien Michot](http://michot.julien.free.fr/drupal/index.php), this problem is now solved. Thanks again for this!

The problem was sometimes the KRT\_from\_P function from libmv to extract K, R and t don't find correct rotation matrix. This function use the RQ decomposition proposed by Hartley,Zisserman, but the the determinant of the rotation matrix R was not 1, it is -1. This type of matrix are called [improper rotation](http://en.wikipedia.org/wiki/Improper_rotation).

It's now an old story...

# Automatic tracking #

Even using fundamental constraints, the matches are sometimes not good... How can we robustly reject outliers even if they are on epilines?

This problem is not really problematic for now because I use fully parameterized camera so I can reject bad matches using reprojection error, but when dealing with real data, we will have to face such problem!