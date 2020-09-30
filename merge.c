void mergeImages(Image* image1, Image* image2)
{
    int height = image1 -> height > image2 -> height ? image2 -> height : image1 -> height;
    int width = image1 -> width > image2 -> width ? image2 -> width : image1 -> width;
    int ind1 = 0;
    int ind2 = 0;

    png_bytepp newPixels = (png_bytepp)calloc(2 * height, sizeof(png_bytep));
    for(int y = 0; y < 2 * height; ++y)
    {
        newPixels[y] = (png_bytep)calloc(width * getImagePixelSize(image1), sizeof(png_byte));
        if(y % 2 == 0)
        {
            for(int x = 0; x < width; ++x)
            {
                png_bytep  pixel1 = image1 -> pixels[ind1] + x * getImagePixelSize(image1);
                png_bytep newPixel = newPixels[y] + getImagePixelSize(image1) * x;
                newPixel[0] = pixel1[0];
                newPixel[1] = pixel1[1];
                newPixel[2] = pixel1[2];
                if(getImagePixelSize(image1) == 4)
                    newPixel[3] = pixel1[3];
            }
            ind1++;
        }
        else{
            for(int x = 0; x < width; ++x)
            {
                png_bytep pixel2 = image2->pixels[ind2] + getImagePixelSize(image2) * x;
                png_bytep newPixel = newPixels[y] + getImagePixelSize(image1) * x;
                newPixel[0] = pixel2[0];
                newPixel[1] = pixel2[1];
                newPixel[2] = pixel2[2];
                if(getImagePixelSize(image2) == 4 && getImagePixelSize(image1) == 4)
                    newPixel[3] = pixel2[3];
                else if(getImagePixelSize(image1) == 4)
                {
                    newPixel[3] = 255;
                }
            }
            ind2++;
        }

    }
    image1 -> width = width;
    image1 -> height = height * 2;
    image1 -> pixels = newPixels;

}
