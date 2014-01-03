### Statistical Image Completion
-----

Image processing project realized for the Image Processing class SI241 at Telecom ParisTech. Based on patches statistics of an image, the program aims at completing a missing part of it. It can be used to delete small objects in an image.

### How to use:
-----

Compile the program with a compiler of your choice (tested with GCC). The executable only takes one argument: the image path. Then you have to enter several parameters (size of each patch, minimum offset value, random search parameter, number of offsets, graphcut parameter).

### Sources:
-----

[1] Statistical Image Completion, by Kaiming He and Jian Sun, in MSR-TR-2013-51

[2] Connelly Barnes, Eli Shechtman, Adam Finkelstein, and Dan B Goldman. PatchMatch: A Randomized Correspondence Algorithm for Structural Image Editing. ACM Transactions on Graphics (Proc. SIGGRAPH) 28(3), August 2009.

Software:

GCoptimization - software for energy minimization with graph cuts

    [1] Efficient Approximate Energy Minimization via Graph Cuts.
        Y. Boykov, O. Veksler, R.Zabih. IEEE TPAMI, 20(12):1222-1239, Nov 2001.

    [2] What Energy Functions can be Minimized via Graph Cuts?
        V. Kolmogorov, R.Zabih. IEEE TPAMI, 26(2):147-159, Feb 2004. 

    [3] An Experimental Comparison of Min-Cut/Max-Flow Algorithms for 
        Energy Minimization in Vision. Y. Boykov, V. Kolmogorov. 
        IEEE TPAMI, 26(9):1124-1137, Sep 2004.
