#include "CImg.h"
#include <iostream>

using namespace std;

cimg_library::CImg<unsigned char> switchImage(cimg_library::CImg<unsigned char> image,int** newVect,int* result, int xmin, int xmax, int ymin, int ymax);
