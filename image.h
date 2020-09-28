#ifndef COURSEWORK_IMAGE_H
#define COURSEWORK_IMAGE_H

#include "png.h"


typedef struct {
    int r;
    int g;
    int b;
    int a;
} Color;

typedef struct{
    char* path;
    int width;
    int height;
    png_byte colorType;
    png_byte bitDepth;
    png_byte interlaceType;
    png_byte filterMethod;
    png_byte compressionType;
    png_bytepp pixels;
} Image;

void setPixel(Image* image, png_bytep pixel, Color color);
Color makeColor(int r, int g, int b, int a);
int checkColor(Color color);
int checkPoint(Image* image, int x, int y);
Image* initImage();
void destroyImage(Image* image);
int readImage(Image* image, const char* path);
int saveImage(Image* image, const char* path);
char* getImageColorType(Image* image);
int getImagePixelSize(Image* image);
void printImageInfo(Image* image);
int copyPartOfImage(int x1Source, int y1Source, int x2Source, int y2Source, int xResult, int yResult, Image* image);
int reflectionOfPartOfImage(int xl, int yl, int xr, int yr, char axis, Image* image);
void circleDrawingWithRadiusAndCenter(int x0, int y0, int R, int lineWidth, Color lineColor, int fill, Color fillColor, Image* image);
void circleDrawingWithCircumscribedSquare(int xl, int yl, int xr, int yr, int lineWidth, Color lineColor, int fill, Color fillColor, Image* image);

#endif
