# What I find difficult when starting with Libmv #

  * Documentation is hard to find. Simple examples would be very helpful!
  * Basic objects like cameras are implemented but not used in SfM functions.
  * Compilation time is really (_really_) long! This is why I take a subset of functions for now.
  * I don't know if it's Visual studio related, but after creating project files using CMake, I have a lot of projects (approx. 70!).
  * Kernel version vs standard functions vs functions interface for kernel... Which one are the best?
  * Finding related functions is not easy (no commune namespace/class for epipolar functions, triangulation, etc...)
  * Functions don't take cameras as input but low levels information (advantages/disadvantages)

```
void RelativeCameraMotion(const Mat3 &R1, const Vec3 &t1,
                          const Mat3 &R2, const Vec3 &t2,
                          Mat3 *R, Vec3 *t);
```
instead of this better version:
```
void RelativeCameraMotion(const Camera &c1,
                          const Camera &c2,
                          Camera *outPosition);
```
  * Could not find easy ways to use Levenberg-Marquardt method for fundamental estimation ([for example this method](http://sist.sysu.edu.cn/~chenpei/papers/IETCV-10.pdf)). Maybe add some explanation about needed parameters (or bigger examples than in tests cases)
  * Bundle wrapper make a strong assumption: all the points are assumed to be observed in all images. Is this a limitation of SSBA? Maybe use Lourakis' sba?
  * SixPointReconstruction needs an other robust processor (two\_view\_kernel can't work ;). Only two view robust estimation seems to be implemented (or can't find robust version of SixPointReconstruction).
  * vector class with aligned allocator [can be done like this](http://eigen.tuxfamily.org/dox/TopicStlContainers.html#vector_spec)
  * **Can't find a robust N-Views triangulation function.**
  * **Can't find a robust 5 points relative motion estimator.**

# Question related to my gsoc #
This summer I want to create a strong API both for beginners and experienced users of SfM field. I don't want to do something useful only for me but I don't know how doing this properly.

  * My first issue concerns the use of many external API (OpenCV, Libmv, Eigen, Boost,...). Indeed, they help us to build something strong but the user is often tired to build each project separately before being able to work with our API. Maybe we can add the source files of the various teams into this API in order to help the user to work with our API... For example, the boost filesystem can probably be added into the trunk because the version is now relatively stable... But is this correct with the Boost licence?
  * Which type of matrix we should use? The Eigen library seems to be the best choice but each opencv's function takes a cv::Mat as input. As the need of OpenCV at first step of SfM is mandatory (strong, efficient, fast...), we can ask us if adding an other library for matrix handling is better than adding functions to cv::Mat.
  * Long term question: if we build a good API and we want to add it to OpenCV, a lot of things should be changed in order to not being redundant (all the SfM part). I need help to understand what I can do and what I can't. We should take care of what is well done in OpenCV and what isn't.