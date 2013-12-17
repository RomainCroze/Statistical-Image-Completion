#include "Include/GUI.h"

int displayImage(cimg_library::CImg<unsigned char> image)
{
    cout<<"Display image"<<endl;
    cimg_library::CImgDisplay main_disp(image,"Output");
    while (!main_disp.is_closed())
    {
        main_disp.wait();
    }

    return 0;
};

int* getXYImage(cimg_library::CImg<unsigned char> image)
{
    cout<<"Selection of the zone to complete"<<endl;
    int* tab = new int [2];

    cimg_library::CImgDisplay main_disp(image,"Select Limits");
    while (!main_disp.is_closed())
    {
        main_disp.wait();
        if (main_disp.button())
        {

            tab[0]=main_disp.mouse_x();
            tab[1]=main_disp.mouse_y();
            return tab;
        }
    }

	tab[0]=-1;
    return tab;

};
