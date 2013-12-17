#include <iostream>
#include "CImg.h"
#include <stdlib.h>
#include <math.h>
#include <climits>

using namespace std;

int** initPatchmatch(cimg_library::CImg<unsigned char> image, int w, int t);

int patchDistance(cimg_library::CImg<unsigned char> currentPatch,cimg_library::CImg<unsigned char> patchPlusOffset);

int** propagationPatchmatch(int** offsetField,cimg_library::CImg<unsigned char> image, int w);

int** rdnsearchPatchmatch(int** offsetField, cimg_library::CImg<unsigned char> image,int w,int t,float alpha);

int** patchmatch(cimg_library::CImg<unsigned char> image,int w,int t,float alpha);
