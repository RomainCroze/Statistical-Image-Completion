#include <iostream>
#include <algorithm>
#include "CImg.h"

using namespace std;

int** occurrenceOffsets(int** offsetField,cimg_library::CImg<unsigned char> image,int w,int nb);
