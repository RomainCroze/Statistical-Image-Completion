#include "Include/patchmatch.h"

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

            distance = distance + ((int)(currentPatch.atXY(i,j,0))-(int)(patchPlusOffset.atXY(i,j,0)))*((int)(currentPatch.atXY(i,j,0))-(int)(patchPlusOffset.atXY(i,j,0))) + ((int)(currentPatch.atXY(i,j,1))-(int)(patchPlusOffset.atXY(i,j,1)))*((int)(currentPatch.atXY(i,j,1))-(int)(patchPlusOffset.atXY(i,j,1))) + ((int)(currentPatch.atXY(i,j,2))-(int)(patchPlusOffset.atXY(i,j,2)))*((int)(currentPatch.atXY(i,j,2))-(int)(patchPlusOffset.atXY(i,j,2)));
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
