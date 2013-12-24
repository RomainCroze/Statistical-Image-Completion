#include "Include/switch.h"

cimg_library::CImg<unsigned char> switchImage(cimg_library::CImg<unsigned char> image,int** newVect,int* result, int xmin, int xmax, int ymin, int ymax)
{
    cout<<"Creating new image"<<endl;
    int width = image.width();
    int height = image.height();
    int widthZone = xmax-xmin+1;
    int heightZone = ymax-ymin+1;
    int num_pixels = widthZone*heightZone;
    int counter;
    int u;
    int v;

    cimg_library::CImg<unsigned char> output(width,height,1,3);

    for (int i=0; i<width; i++)
    {
        for(int j=0; j<height; j++)
        {
            if(i<xmin || i>xmax || j<ymin || j>ymax)
            {
                output.atXY(i,j,0)=image.atXY(i,j,0);
                output.atXY(i,j,1)=image.atXY(i,j,1);
                output.atXY(i,j,2)=image.atXY(i,j,2);
            }
            else
            {

                counter = (i-xmin)+(j-ymin)*widthZone;
                u=newVect[result[counter]][0];
                v=newVect[result[counter]][1];
                output.atXY(i,j,0)=image.atXY(i+u,j+v,0);
                output.atXY(i,j,1)=image.atXY(i+u,j+v,1);
                output.atXY(i,j,2)=image.atXY(i+u,j+v,2);

            }

        }
    }

    return output;
}
