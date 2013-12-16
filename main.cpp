#include "header.h"


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
    // Minimum offset size
    int tau;
    cout << "Please enter minimum offset value (must be positive): ";
    cin >>tau;
    // Random search parameter
    float alpha;
    cout << "Please enter alpha (random search parameter) value (must be between 0 and 1): ";
    cin >> alpha;
    // Number of vectors to keep
    int nb;
    cout << "Please enter the number of offsets to keep: ";
    cin >> nb;




    // Loading offset field
    //OffsetField * offsetField = patchmatch(image,w,tau,alpha);
    //displayImage((occurenceOffsets(offsetField,image)));


    int** offsetField = patchmatch(image,w,tau,alpha);
    int** tabVect = occurenceOffsets(offsetField,image,w,nb);


    while(true){
    // Zone to complete
    tab = getXYImage(image);
    xmin = tab[0];
    ymin = tab[1];

    tab = getXYImage(image);
    xmax = tab[0];
    ymax = tab[1];

    cout << "Please enter Beta value: ";
    cin >> beta;
    cimg_library::CImg<unsigned char> newImage = graphcut(image,tabVect,xmin,xmax,ymin,ymax,w,beta,nb);
    displayImage(newImage);
    }

return 0;
}

