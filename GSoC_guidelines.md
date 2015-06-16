# Week 1 #

Wiki creation in order to exchange ideas with community
First ideas of Camera structures

# Week 2 #
Camera and projection structures are improved.
First contact with libmv team by Vincent. They have build an highly optimized library for SfM. See how we can work together.

# Week 3 #
Start the collaboration with libmv. Code for libmv\_core will be hosted on [GitHub](https://github.com/Petititi/libmv). First idea is to have only a small number of functions.

Template vs inheritances study for robust estimation shows that inheritance is 1.4 slower than template... A bottleneck was identified as being responsible of 80% of computation time! See if this is the same on Linux and different error metric...
# Week 4 #
Keir don't have time to guide me on libmv\_core for now, he suggest me to write an entire SfM pipeline to exchange about what we need in the core part of libmv.
I started the implementation of Noah Snavely: [Modeling the World from Internet Photo Collections](http://citeseerx.ist.psu.edu/viewdoc/summary?doi=10.1.1.163.3666).
# Week 5 #
The implementation of tracks explained in Snavely's method is done, seems working correctly.

The triangulation is working too, but we need a robust implementation which is missing in libmv. The isotropic transformation is not used in my experiments, this can explain such bad triangulation.

The YAML export / import of most of classes is now working.