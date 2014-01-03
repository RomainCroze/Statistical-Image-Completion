#include "Include/header.h"


// Conditions

int main(int argc,char *argv[])
{


    float beta;
    int* tab;
    int xmin;
    int xmax;
    int ymax;
    int ymin;

    // Loading image
    char* imagePath = argv[1];

    cimg_library::CImg<unsigned char> image(imagePath);
    // Patch size
    int w;
    cout << "Please enter size of Patch (must be odd and positive): ";
    cin >> w;
    bool odd = (w-(w/2)*2!=0);
    assert(odd);
    bool positive = (w>=0);
    assert(positive);
    // Minimum offset size
    int tau;
    cout << "Please enter minimum offset value (must be positive): ";
    cin >>tau;
    positive = (tau>=0);
    assert(positive);
    // Random search parameter
    float alpha;
    cout << "Please enter alpha (random search parameter) value (must be between 0 and 1): ";
    cin >> alpha;
    positive = (alpha>=0);
    assert(positive);
    bool inferior_one = (alpha<1);
    assert(inferior_one);
    // Number of vectors to keep
    int nb;
    cout << "Please enter the number of offsets to keep: ";
    cin >> nb;
    positive = (nb>=0);
    assert(positive);

    // Loading offset field

    int** offsetField = patchmatch(image,w,tau,alpha);
    int** tabVect = occurrenceOffsets(offsetField,image,w,nb);


    while(true){
    // Zone to complete
    tab = getXYImage(image);
    xmin = tab[0];
    ymin = tab[1];

if (tab[0]==-1){return 0;}

    tab = getXYImage(image);
    xmax = tab[0];
    ymax = tab[1];

if (tab[0]==-1){return 0;}

    cout << "Please enter Beta value: ";
    cin >> beta;

    int width = image.width();
    int height = image.height();
    cimg_library::CImg<unsigned char> newImage(width,height,1,3);
    newImage = graphcut(image,tabVect,xmin,xmax,ymin,ymax,w,beta,nb);
    displayImage(newImage);
    }

return 0;
}

