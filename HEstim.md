# Introduction #

Thank's to wikipedia, a homography is a concept in the mathematical science of geometry. A homography is an invertible transformation from the real projective plane to the projective plane that maps straight lines to straight lines. Synonyms are collineation, projective transformation, and projectivity, though "collineation" is also used more generally.
## Implementation idea: ##
Like the [5 points method](5ptRelative.md), this method should inherit from a class like **"TwoViewsEstimator"**. This could help Level2 algorithm to handle such algorithm.
# How it's done #

## Opencv ##

We can use FindHomography to compute homographies. Same for fundamental function, we can choose to use RANSAC or LMedS minimisation!

## Libmv ##

As for OpenCV, they have a function to estimate the homography using points correspondances, but they have also an other function which can estimate the homography transformation from a list of 3D correspondences (in file homography.cc: `Homography3DFromCorrespondencesLinear`)