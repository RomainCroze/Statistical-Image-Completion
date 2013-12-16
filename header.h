#ifndef HEADER_H_INCLUDED
#define HEADER_H_INCLUDED

#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h> /* div, div_t */
#include <climits>
#include <math.h>
#include <string>
#include <time.h>
#include "GCoptimization.h"
#define cimg_use_jpeg
#include "CImg/CImg.h"

using namespace std;


int displayImage(cimg_library::CImg<unsigned char> image)
{
    cout<<"Affichage de l'image"<<endl;
    cimg_library::CImgDisplay main_disp(image,"Output");
    while (!main_disp.is_closed())
    {
        main_disp.wait();
    }

    return 0;
};

int* getXYImage(cimg_library::CImg<unsigned char> image)
{
    cout<<"Récupération de la zone à supprimer"<<endl;
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
    return tab;

};

int** initPatchmatch(cimg_library::CImg<unsigned char> image, int w, int t)
{
    cout<<"Initialization"<<endl;
    int width = image.width();
    int height = image.height();
    int sizeOffsetField = (width-w+1)*(height-w+1);
    int** offsetField = new int* [sizeOffsetField];
    int xPatch;
    int yPatch;
    int x;
    int y;
    div_t divresult;

    for(int i =0; i<sizeOffsetField; i++)
    {

        divresult = div (i,width-w+1);

        xPatch = divresult.rem;

        yPatch = divresult.quot;

        do
        {
            x = (rand() % (width-w+1)) - xPatch;
            y = (rand() % (height-w+1)) - yPatch;
        }
        while ((x*x+y*y)<(t*t));
        offsetField[i]=new int[2];
        (offsetField[i])[0]=x;
        (offsetField[i])[1]=y;
    }

    return offsetField;
}

int patchDistance(cimg_library::CImg<unsigned char> currentPatch,cimg_library::CImg<unsigned char> patchPlusOffset)
{
    int width = currentPatch.width();
    int height = currentPatch.height();
    int distance =0;
    for(int i = 0; i< width; i++)
    {
        for(int j = 0; j< height; j++)
        {
            distance = distance + ((int)(*(currentPatch.data(i,j)))-(int)(*(patchPlusOffset.data(i,j))))*((int)(*currentPatch.data(i,j))-(int)(*(patchPlusOffset.data(i,j))));
        }
    }

    return distance;

};


int** propagationPatchmatch(int** offsetField,cimg_library::CImg<unsigned char> image, int w)
{
    cout<<"Propagation"<<endl;
    int width = image.width();
    int height = image.height();
    int sizeOffsetField = (width-w+1)*(height-w+1);
    int xPatch;
    int yPatch;
    int x;
    int y;
    int u1;
    int v1;
    int u2;
    int v2;
    int u3;
    int v3;
    div_t divresult;
    cimg_library::CImg<unsigned char> currentPatch;
    cimg_library::CImg<unsigned char> patchPlusOffset;
    int d1;
    int d2;
    int d3;
    int delta = (w-1)/2;

    for(int i = 0; i<sizeOffsetField; i++)
    {

        divresult = div (i,width-w+1);

        xPatch = divresult.rem;
        yPatch = divresult.quot;

        if( yPatch!=0 && xPatch!=0)
        {

            x=xPatch+(w-1)/2;
            y=yPatch+(w-1)/2;

            currentPatch=image.get_crop(x-delta,y-delta,x+delta,y+delta);

            u1=(offsetField[i])[0];
            v1=(offsetField[i])[1];

            patchPlusOffset=image.get_crop(x+u1-delta,y+v1-delta,x+u1+delta,y+v1+delta);
            d1=patchDistance(currentPatch,patchPlusOffset);

            u2=(offsetField[i-1])[0];
            v2=(offsetField[i-1])[1];
            if ((xPatch+u2)>=0 && (xPatch+u2)<(width-w+1) && (yPatch+v2)>=0 && (yPatch+v2)<(height-w+1))
            {
                patchPlusOffset=image.get_crop(x+u2-delta,y+v2-delta,x+u2+delta,y+v2+delta);
                d2=patchDistance(currentPatch,patchPlusOffset);
            }
            else
            {
                d2=INT_MAX;

            }

            u3=(offsetField[i-(width-w+1)])[0];
            v3=(offsetField[i-(width-w+1)])[1];

            if ((xPatch+u3)>=0 && (xPatch+u3)<(width-w+1) && (yPatch+v3)>=0 && (yPatch+v3)<(height-w+1))
            {

                patchPlusOffset=image.get_crop(x+u3-delta,y+v3-delta,x+u3+delta,y+v3+delta);
                d3=patchDistance(currentPatch,patchPlusOffset);
            }
            else
            {
                d3=INT_MAX;
            }

            if(d2<=d1)
            {
                if(d2<d3)
                {
                    (offsetField[i])[0]=u2;
                    (offsetField[i])[1]=v2;
                }

                else if(d3<=d1)
                {
                    (offsetField[i])[0]=u3;
                    (offsetField[i])[1]=v3;
                }


            }


        }
    }

    for(int i = sizeOffsetField-1; i>=0; i--)
    {

        divresult = div (i,width-w+1);

        xPatch = divresult.rem;
        yPatch = divresult.quot;

        if( yPatch!=(height-w) && xPatch!=(width-w))
        {

            x=xPatch+(w-1)/2;
            y=yPatch+(w-1)/2;

            currentPatch=image.get_crop(x-delta,y-delta,x+delta,y+delta);

            u1=(offsetField[i])[0];
            v1=(offsetField[i])[1];

            patchPlusOffset=image.get_crop(x+u1-delta,y+v1-delta,x+u1+delta,y+v1+delta);
            d1=patchDistance(currentPatch,patchPlusOffset);

            u2=(offsetField[i+1])[0];
            v2=(offsetField[i+1])[1];
            if ((xPatch+u2)>=0 && (xPatch+u2)<(width-w+1) && (yPatch+v2)>=0 && (yPatch+v2)<(height-w+1))
            {
                patchPlusOffset=image.get_crop(x+u2-delta,y+v2-delta,x+u2+delta,y+v2+delta);
                d2=patchDistance(currentPatch,patchPlusOffset);
            }
            else
            {
                d2=INT_MAX;

            }

            u3=(offsetField[i+(width-w+1)])[0];
            v3=(offsetField[i+(width-w+1)])[1];

            if ((xPatch+u3)>=0 && (xPatch+u3)<(width-w+1) && (yPatch+v3)>=0 && (yPatch+v3)<(height-w+1))
            {

                patchPlusOffset=image.get_crop(x+u3-delta,y+v3-delta,x+u3+delta,y+v3+delta);
                d3=patchDistance(currentPatch,patchPlusOffset);
            }
            else
            {
                d3=INT_MAX;
            }

            if(d2<=d1)
            {
                if(d2<d3)
                {
                    (offsetField[i])[0]=u2;
                    (offsetField[i])[1]=v2;
                }

                else if(d3<=d1)
                {
                    (offsetField[i])[0]=u3;
                    (offsetField[i])[1]=v3;

                }


            }


        }
    }

    return offsetField;

}

int** rdnsearchPatchmatch(int** offsetField, cimg_library::CImg<unsigned char> image,int w,int t,float alpha)
{
    cout<<"Random search"<<endl;
    int width = image.width();
    int height = image.height();
    int sizeOffsetField = (width-w+1)*(height-w+1);
    int radius = max(width,height);
    int u;
    int v;
    int k;
    float Ru;
    float Rv;
    int a;
    int b;
    int xPatch;
    int yPatch;
    div_t divresult;
    int d1;
    int d2;
    int delta=(w-1)/2;
    cimg_library::CImg<unsigned char> currentPatch;
    cimg_library::CImg<unsigned char> patchPlusOffset;
    int x;
    int y;

    for(int i=0; i<sizeOffsetField; i++)
    {

        divresult = div (i,width-w+1);

        xPatch = divresult.rem;
        yPatch = divresult.quot;
        x=xPatch+(w-1)/2;
        y=yPatch+(w-1)/2;

        k = 0;

        currentPatch=image.get_crop(x-delta,y-delta,x+delta,y+delta);


        while((radius*pow(alpha,k))>1)
        {
            u=(offsetField[i])[0];
            v=(offsetField[i])[1];
            patchPlusOffset=image.get_crop(x+u-delta,y+v-delta,x+u+delta,y+v+delta);
            d1=patchDistance(currentPatch,patchPlusOffset);

            do
            {
                do
                {
                    Ru=((float)(rand()%1000)/1000)*2-1;
                    a=u+(int)(radius*pow(alpha,k)*Ru);
                }
                while ((xPatch+a)<0 || (xPatch+a)>=(width-w+1));

                do
                {
                    Rv=((float)(rand()%1000)/1000)*2-1;
                    b=v+(int)(radius*pow(alpha,k)*Rv);
                }
                while((yPatch+b)<0 || (yPatch+b)>=(height-w+1));
            }
            while((a*a+b*b)<(t*t));

            patchPlusOffset=image.get_crop(x+a-delta,y+b-delta,x+a+delta,y+b+delta);
            d2=patchDistance(currentPatch,patchPlusOffset);
            if(d2<d1)
            {
                (offsetField[i])[0]=a;
                (offsetField[i])[1]=b;
            }
            k=k+1;

        }
    }

    return offsetField;

}

int** patchmatch(cimg_library::CImg<unsigned char> image,int w,int t,float alpha)
{
    cout<<"Patchmatch:"<<endl;
    // Initializing offsetField with random values
    int** offsetField = initPatchmatch(image, w, t);

    // Propagation of the best offsets
    offsetField = propagationPatchmatch(offsetField,image,w);

    // Look for better offsets around
    offsetField= rdnsearchPatchmatch(offsetField,image,w,t,alpha);
    return offsetField;
}

int** occurenceOffsets(int** offsetField,cimg_library::CImg<unsigned char> image,int w,int nb)
{
    cout<<"Keep only good vectors"<<endl;
    int* tabNbOccur = new int [nb];
    int** tabVect = new int* [nb];
    int width = image.width();
    int height = image.height();
    int sizeField = (width-w+1)*(height-w+1);
    int** occurence=new int* [2*width];
    int* minimum;
    int position;
    int currentVal;

    for (int i =0; i<nb; i++)
    {
        tabVect[i]=new int[2];
        tabNbOccur[i]=0;
    }



    for (int i =0; i<2*width; i++)
    {
        occurence[i]=new int[2*height];
        for(int j=0; j<2*height; j++)
        {
            (occurence[i])[j]=0;
        }
    }


    for (int k=0; k< sizeField; k++)
    {
        (occurence[((offsetField[k])[0])+width])[((offsetField[k])[1])+height]+=1;
    }

    for(int i =0; i<2*width; i++)
    {
        for(int j =0; j<2*height; j++)
        {
            minimum=min_element(tabNbOccur,tabNbOccur+nb);
            position=minimum-tabNbOccur;
            currentVal=(occurence[i])[j];
            if (currentVal>*minimum)
            {
                tabNbOccur[position]=currentVal;
                tabVect[position][0]=i-width;
                tabVect[position][1]=j-height;
            }
        }
    }


    delete [] tabNbOccur;
    delete [] occurence;

    return tabVect;
}

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

    cimg_library::CImg<unsigned char> output(width,height);

    for (int i=0; i<width; i++)
    {
        for(int j=0; j<height; j++)
        {
            if(i<xmin || i>xmax || j<ymin || j>ymax)
            {
                *(output.data(i,j))= *(image.data(i,j));
            }
            else
            {

                counter = (i-xmin)+(j-ymin)*widthZone;
                u=newVect[result[counter]][0];
                v=newVect[result[counter]][1];
                *(output.data(i,j))=*(image.data(i+u,j+v));

            }

        }
    }

    return output;
}

cimg_library::CImg<unsigned char> graphcut(cimg_library::CImg<unsigned char> image,int** tabVect,int xmin,int xmax,int ymin,int ymax,int w,float beta,int nb)
{
    cout<<"Graphcut:"<<endl;
    int width = image.width();
    int height = image.height();
    int nb2=nb;
    int widthZone = xmax-xmin+1;
    int heightZone = ymax-ymin+1;
    int num_pixels = widthZone*heightZone;
    int* result = new int[num_pixels];
    int xPix;
    int yPix;
    div_t divresult;
    int x;
    int y;


    for(int i=0; i<nb; i++)
    {
        if ((xmax+tabVect[i][0])>=width || (xmin+tabVect[i][0])<0 || (ymax+tabVect[i][1])>=height || (ymin+tabVect[i][1])<0)
        {
            nb2=nb-1;
        }
    }

    int** newTab= new int* [nb2];
    nb2=0;

    for(int i=0; i<nb; i++)
    {
        if ((xmax+tabVect[i][0])>=width || (xmin+tabVect[i][0])<0 || (ymax+tabVect[i][1])>=height || (ymin+tabVect[i][1])<0)
        {
            nb2+=1;
        }
        else
        {
            newTab[i-nb2]=new int [2];
            newTab[i-nb2][0]=tabVect[i][0];
            newTab[i-nb2][1]=tabVect[i][1];
        }
    }

    nb2=nb-nb2;



    try
    {
        GCoptimizationGridGraph* gc = new GCoptimizationGridGraph(widthZone,heightZone,nb2);
        for(int i=0; i<num_pixels; i++)
        {
            for(int j=0; j<nb2; j++)
            {
                divresult = div (i,widthZone);
                xPix = divresult.rem;
                yPix = divresult.quot;
                x=xPix+xmin;
                y=yPix+ymin;


                gc->setDataCost(i,j,(abs(*(image.data(x,y))-*(image.data(x+newTab[j][0],y+newTab[j][1])))*min(heightZone,widthZone))/(1+(min(min(min(xPix,yPix),heightZone-1-yPix),widthZone-1-xPix))));

            }

        }


        for (int l1=0; l1<nb2; l1++)
        {
            for(int l2=0; l2<nb2; l2++)
            {

                if(l1==l2)
                {
                    gc->setSmoothCost(l1,l2,-beta*255);
                }
                else gc->setSmoothCost(l1,l2,0);
            }

        }

        gc->swap(2);
        for(int i = 0; i<num_pixels; i++)
        {
            result[i] = gc->whatLabel(i);

        }
        delete gc;

        return switchImage(image,newTab,result,xmin,xmax,ymin,ymax);
    }
    catch(GCException e)
    {
        e.Report();
    }

}


#endif // HEADER_H_INCLUDED












