#include "image.h"

int main(int argc, char** argv)
{
    Image* image = initImage();
    readImage(image, argv[1]);
    //copyPartOfImage(0 , 0, 500, 400, 100, 100, image);
    reflectionOfPartOfImage(0, 0, 799, 599, 'x', image);
    //printImageInfo(image);
    saveImage(image, argv[2]);
    destroyImage(image);
    return 0;
}