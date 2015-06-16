# Introduction #


Post from [Keir Mierle](http://groups.google.com/group/libmv-devel/msg/79128a47e4508ff1) proposed to view SfM as a different level of abstraction. This is a useful way to help people to exchange points of view of various implementation.

I propose to use these pages to exchange ideas about the best way to implement algorithms.


# Details #

The proposed segmentation was the following:
  * [Level 0](Lev0Points.md) routines are available, but not in a single, unified library that makes it trivial to experiment with different solvers.
  * [Level 1](Lev1Core.md) routines are mostly trivial to write. However, again, there is the issue of different libraries having different L1 routines available with different interfaces.
  * [Level 2](Lev2Optimiz.md) routines are easy to write specifically, but harder to write in general (so that you can plug any level 1 routine into a level 2 routine to get a robust solver for any level 1 routine). In practice, this means that the existing SfM pipelines have level 2 routines only for specific cases of level 1.
  * [Level 3](Lev3Algo.md) routines are where all the secret sauce is, that up until e.g. the release of Bundler, were hard to find. I argue that libmvcore should **not** have any level 3 routines, but that libmv **should**.


Please remove / add / update missing important informations!