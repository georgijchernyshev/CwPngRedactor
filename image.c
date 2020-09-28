#include "image.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

Image* initImage()
{
    Image* image = (Image*)malloc(sizeof(Image));
    image -> path = NULL;
    image -> width = 0;
    image -> height = 0;
    image -> colorType = 0;
    image -> bitDepth = 0;
    image -> interlaceType = 0;
    image -> filterMethod = 0;
    image -> compressionType = 0;
    image -> pixels = NULL;

    return image;
}

void destroyImage(Image* image)
{
    free(image -> path);
    if(image -> pixels != NULL)
    {
        for(int i = 0; i < image -> height; ++i)
        {
            free(image -> pixels[i]);
        }
    }
    free(image -> pixels);
    free(image);
}


int readImage(Image* image, const char* path)
{
    FILE* fp = fopen(path, "rb");
    if(!fp)
    {
        fprintf(stderr, "File to read open error.\n");
        return 0;
    }

    unsigned char header[8];
    fread(header, sizeof(unsigned char), 8, fp);
    if(png_sig_cmp(header, 0, 8))
    {
        fprintf(stderr, "File doesn't have png format.\n");
        fclose(fp);
        return 0;
    }

    png_structp pngStructPtr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if(!pngStructPtr)
    {
        fprintf(stderr, "Something went wrong with png_create_read_struct().\n");
        fclose(fp);
        return 0;
    }

    png_infop  pngInfoStructPtr = png_create_info_struct(pngStructPtr);
    if(!pngInfoStructPtr)
    {
        fprintf(stderr, "Something went wrong with png_create_info_struct().\n");
        png_destroy_read_struct(&pngStructPtr, NULL, NULL);
        fclose(fp);
        return 0;
    }

    if(setjmp(png_jmpbuf(pngStructPtr)))
    {
        fprintf(stderr, "Init IO error.\n");
        png_destroy_read_struct(&pngStructPtr, &pngInfoStructPtr, NULL);
        fclose(fp);
        return 0;
    }

    png_init_io(pngStructPtr, fp);
    png_set_sig_bytes(pngStructPtr, 8);
    png_read_info(pngStructPtr, pngInfoStructPtr);

    image -> width = png_get_image_width(pngStructPtr, pngInfoStructPtr);
    image -> height = png_get_image_height(pngStructPtr, pngInfoStructPtr);
    image -> colorType = png_get_color_type(pngStructPtr, pngInfoStructPtr);
    image -> bitDepth = png_get_bit_depth(pngStructPtr, pngInfoStructPtr);
    image -> filterMethod = png_get_filter_type(pngStructPtr, pngInfoStructPtr);
    image -> compressionType = png_get_compression_type(pngStructPtr, pngInfoStructPtr);
    image -> interlaceType = png_get_interlace_type(pngStructPtr, pngInfoStructPtr);
    png_set_interlace_handling(pngStructPtr);

    if((image -> colorType != PNG_COLOR_TYPE_RGB && image->colorType != PNG_COLOR_TYPE_RGBA) ||
        image -> bitDepth != 8 || image -> filterMethod != 0 || image -> compressionType != 0 )
    {
        fprintf(stderr, "This png format doesn't support.\n");
        png_destroy_read_struct(&pngStructPtr, &pngInfoStructPtr, NULL);
        fclose(fp);
        return 0;
    }

    png_read_update_info(pngStructPtr, pngInfoStructPtr);

    if(setjmp(png_jmpbuf(pngStructPtr)))
    {
        fprintf(stderr, "Error with reading pixels map.\n");
        png_destroy_read_struct(&pngStructPtr, &pngInfoStructPtr, NULL);
        fclose(fp);
        return 0;
    }

    image -> pixels = (png_bytepp)calloc(image -> height, sizeof(png_bytep));
    for(int i = 0; i < image ->height; ++i)
    {
        image -> pixels[i] = (png_bytep)calloc(png_get_rowbytes(pngStructPtr, pngInfoStructPtr), sizeof(png_byte));
    }
    png_read_image(pngStructPtr, image -> pixels);

    image -> path = (char *)calloc(strlen(path)+1, sizeof(char));
    strcpy(image -> path, path);


    png_destroy_read_struct(&pngStructPtr, &pngInfoStructPtr, NULL);
    fclose(fp);

    return  1;
}

int saveImage(Image* image, const char* path)
{
    FILE* fp = fopen(path, "wb");

    if(!fp)
    {
        fprintf(stderr, "File to write open error.\n");
        return  0;
    }

    png_structp pngStructPtr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if(!pngStructPtr)
    {
        fprintf(stderr, "Something went wrong with png_create_write_struct().\n");
        fclose(fp);
        return 0;
    }

    png_infop pngInfoStructPtr = png_create_info_struct(pngStructPtr);
    if(!pngInfoStructPtr)
    {
        fprintf(stderr, "Something went wrong with png_create_info_struct().\n");
        png_destroy_write_struct(&pngStructPtr, NULL);
        fclose(fp);
        return 0;
    }

    if(setjmp(png_jmpbuf(pngStructPtr)))
    {
        fprintf(stderr, "Init IO error.\n");//не забудь написать, че тут происходит
        png_destroy_write_struct(&pngStructPtr, &pngInfoStructPtr);
        fclose(fp);
        return 0;
    }
    png_init_io(pngStructPtr, fp);

    if(setjmp(png_jmpbuf(pngStructPtr)))
    {
        fprintf(stderr, "IHDR chunk error.\n");
        png_destroy_write_struct(&pngStructPtr, &pngInfoStructPtr);
        fclose(fp);
        return 0;
    }
    png_set_IHDR(pngStructPtr, pngInfoStructPtr, image -> width, image -> height, image -> bitDepth,
                 image -> colorType, image -> interlaceType, image -> compressionType, image -> filterMethod);
    png_write_info(pngStructPtr, pngInfoStructPtr);

    if(setjmp(png_jmpbuf(pngStructPtr)))
    {
        fprintf(stderr, "Writing image error.\n");
        png_destroy_write_struct(&pngStructPtr, &pngInfoStructPtr);
        fclose(fp);
        return 0;
    }
    png_write_image(pngStructPtr, image -> pixels);

    if(setjmp(png_jmpbuf(pngStructPtr)))
    {
        fprintf(stderr, "End writing error.\n");
        png_destroy_write_struct(&pngStructPtr, &pngInfoStructPtr);
        fclose(fp);
        return 0;
    }
    png_write_end(pngStructPtr, NULL);

    png_destroy_write_struct(&pngStructPtr, &pngInfoStructPtr);
    fclose(fp);

    return 1;
}

char* getImageColorType(Image* image)
{
    if(image -> colorType == PNG_COLOR_TYPE_RGB)
        return "RGB";
    return "RGBA";
}

int getImagePixelSize(Image* image)
{
    if (image->colorType == PNG_COLOR_TYPE_RGB)
        return 3;
    return 4;
}

void printImageInfo(Image* image)
{
    printf("\n");
    printf("Image info:\n");
    printf("Image path: %s\n", image -> path);
    printf("Image width: %d\n", image -> width);
    printf("Image height: %d\n", image -> height);
    printf("Image color type: %s\n", getImageColorType(image));
    printf("Image bit depth: %d\n", image -> bitDepth);
}

int copyPartOfImage(int x1Source, int y1Source, int x2Source, int y2Source, int xResult, int yResult, Image* image)
{
    int sizeOfPixel = getImagePixelSize(image);
    int dx = x2Source - x1Source;
    int dy = y2Source - y1Source;

    if(dx <= 0 || dy <= 0 || x1Source + dx >= image -> width || y1Source + dy >= image -> height)
    {
        fprintf(stderr, "In function copyPartOfImage(). Part to copy is set incorrectly.\n");
        return 1;
    }

    if(xResult + dx > image -> width || yResult + dy > image -> height)
    {
        fprintf(stderr, "In function copyPartOfImage(). Part to past is set incorrectly.\n");
        return 1;
    }

    png_bytepp newPixels = (png_bytepp)calloc(dy + 1, sizeof(png_bytep));
    for(int i = 0; i <= dy; ++i)
    {
        newPixels[i] = (png_bytep)calloc(sizeOfPixel * (dx + 1), sizeof(png_byte));
        for(int j = 0; j <= dx; ++j)
        {
            png_bytep newPixel = newPixels[i] + sizeOfPixel * j;
            png_bytep oldPixel = image -> pixels[y1Source + i] + sizeOfPixel * (j + x1Source);
            newPixel[0] = oldPixel[0];
            newPixel[1] = oldPixel[1];
            newPixel[2] = oldPixel[2];
            if(sizeOfPixel == 4)
                newPixel[3] = oldPixel[3];
        }
    }

    for(int y = 0; y <= dy; ++y)
    {
        for(int x = 0; x <= dx; ++x)
        {
            png_bytep pixel = image->pixels[y + yResult] + sizeOfPixel * (x + xResult);
            png_bytep bufPixel = newPixels[y] + sizeOfPixel * x;
            pixel[0] = bufPixel[0];
            pixel[1] = bufPixel[1];
            pixel[2] = bufPixel[2];
            if (sizeOfPixel == 4)
                pixel[3] = bufPixel[3];
        }
    }
    for(int i = 0; i <= dy; ++i)
    {
        free(newPixels[i]);
    }
    free(newPixels);
    return 0;
}

int reflectionOfPartOfImage(int xl, int yl, int xr, int yr, const char axis, Image* image)
{
    int sizeOfPixel = getImagePixelSize(image);
    int dx = xr - xl;
    int dy = yr - yl;

    if(dx < 0 || dy < 0 || xl + dx >= image -> width || yl + dy >= image -> height)
    {
        fprintf(stderr, "In function reflectionOfPartOfImage(). Part to reflection is set incorrectly.\n");
        return 1;
    }

    if(axis != 'x' && axis != 'y')
    {
        fprintf(stderr, "In function reflectionOfPartOfImage(). Axis of reflection is set incorrectly.\n");
        return 1;
    }

    png_bytepp newPixels = (png_bytepp)calloc(dy+1, sizeof(png_bytep));
    for(int y = 0; y < dy + 1; ++y)
    {
        newPixels[y] = (png_bytep)calloc(sizeOfPixel * (dx + 1), sizeof(png_byte));
    }

    if(axis == 'y')
    {
        for(int y = 0; y < dy + 1; ++y)
        {
            for(int x = 0; x < dx + 1; ++x)
            {
                png_bytep newPixel = newPixels[y] + sizeOfPixel * x;
                png_bytep oldPixel = image -> pixels[yl + y] + sizeOfPixel * (xr - x);
                newPixel[0] = oldPixel[0];
                newPixel[1] = oldPixel[1];
                newPixel[2] = oldPixel[2];
                if(sizeOfPixel == 4)
                    newPixel[3] = oldPixel[3];
            }
        }
    }
    else
    {
        for(int y = 0; y < dy + 1; ++y)
        {
            for(int x = 0; x < dx + 1; ++x)
            {
                png_bytep newPixel = newPixels[y] + sizeOfPixel * x;
                png_bytep oldPixel = image -> pixels[yr - y] + sizeOfPixel * (xl + x);
                newPixel[0] = oldPixel[0];
                newPixel[1] = oldPixel[1];
                newPixel[2] = oldPixel[2];
                if(sizeOfPixel == 4)
                    newPixel[3] = oldPixel[3];
            }
        }
    }
    for(int y = 0; y < dy + 1; ++y)
    {
        for(int x = 0; x < dx + 1; ++x)
        {
            png_bytep pixelToCopy = image -> pixels[yl + y] + sizeOfPixel * (xl + x);
            png_bytep pixelFromCopy = newPixels[y] + sizeOfPixel * x;
            pixelToCopy[0] = pixelFromCopy[0];
            pixelToCopy[1] = pixelFromCopy[1];
            pixelToCopy[2] = pixelFromCopy[2];
            if(sizeOfPixel == 4)
                pixelToCopy[3] = pixelFromCopy[3];
        }
    }

    for(int y = 0; y < dy + 1; ++y)
    {
        free(newPixels[y]);
    }
    free(newPixels);

    return 0;
}

void setPixel(Image* image, png_bytep pixel, Color color)
{
    pixel[0] = color.r;
    pixel[1] = color.g;
    pixel[2] = color.b;
    if(getImagePixelSize(image) == 4)
        pixel[3] = color.a;
}

Color makeColor(int r, int g, int b, int a)
{
    Color color = {r, g, b, a};
    return color;
}

int checkColor(Color color)
{
    return color.r >= 0 && color.r <= 255 &&
           color.g >= 0 && color.g <= 255 &&
           color.b >= 0 && color.b <= 255 &&
           color.a >= 0 && color.a <= 255;
}

int checkPoint(Image* image, int x, int y)
{
    return x >= 0 && x < image->width && y >= 0 && y < image->height;
}

void circleDrawingWithRadiusAndCenter(int x0, int y0, int R, int lineWidth, Color lineColor, int fill, Color fillColor, Image* image)
{

    int xl ,yl ,xr , yr;
    xl = x0 - R;
    yl = y0 - R;
    xr = x0 + R;
    yr = y0 + R;
    if(xl < 0)
    {
        xl = 0;
    }
    else if (xl >= image -> width)
    {
        xl = image -> width - 1;
    }

    if(yl < 0)
    {
        yl = 0;
    } else if(yl >= image -> height)
    {
        yl = image -> height - 1;
    }

    if(xr < 0)
    {
        xr = 0;
    }
    else if (xr >= image -> width)
    {
        xr = image -> width - 1;
    }

    if(yr < 0)
    {
        yr = 0;
    } else if(yr >= image -> height)
    {
        yr = image -> height - 1;
    }

    for(int y = yl; y < yr; ++y )
    {
        for(int x = xl; x < xr; ++x)
        {
            png_bytep pixel = image -> pixels[y] + getImagePixelSize(image) * x;
            double dist = sqrt(pow((x - x0), 2) + pow((y - y0), 2));
            if(dist < R)
            {
                if(dist >= (R - lineWidth))
                {
                    setPixel(image, pixel, lineColor);
                } else
                {
                    if(fill)
                    {
                        setPixel(image, pixel, fillColor);
                    }
                }
            }
        }
    }
}
//если указан не квадрат, то произойдет выравнивание (обрежется, либо увеличится высота прямоугольника до квадрата)
void circleDrawingWithCircumscribedSquare(int xl, int yl, int xr, int yr, int lineWidth, Color lineColor, int fill, Color fillColor, Image* image)
{
    int R = (xr - xl + 1) / 2;
    int x0, y0;
    x0 = xl + R;
    y0 = yl + R;
    if(xl < 0)
    {
        xl = 0;
    }
    else if (xl >= image -> width)
    {
        xl = image -> width - 1;
    }

    if(yl < 0)
    {
        yl = 0;
    } else if(yl >= image -> height)
    {
        yl = image -> height - 1;
    }

    if(xr < 0)
    {
        xr = 0;
    }
    else if (xr >= image -> width)
    {
        xr = image -> width - 1;
    }

    if(yr < 0)
    {
        yr = 0;
    } else if(yr >= image -> height)
    {
        yr = image -> height - 1;
    }

    for(int y = yl; y < yl + 2*R + 1; ++y)
    {
        for(int x = xl; x < xr; ++x)
        {
            png_bytep pixel = image -> pixels[y] + getImagePixelSize(image) * x;
            double dist = sqrt(pow((x - x0), 2) + pow((y - y0), 2));
            if(dist < R)
            {
                if(dist >= (R - lineWidth))
                {
                    setPixel(image, pixel, lineColor);
                } else
                {
                    if(fill)
                    {
                        setPixel(image, pixel, fillColor);
                    }
                }
            }
        }
    }
}



