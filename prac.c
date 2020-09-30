/*void separateImage(int xParts, int yParts, const char* partsPath, Image* image)
{
    int partHeight = image -> height / yParts;
    int partWidth = image -> width / xParts;

    if(partHeight == 0 || partWidth == 0)
    {
        printf( "Image is too small.\n");
        return;
    }
    char* partPath = calloc(strlen(partsPath) + 41, sizeof(char));
    strcpy(partPath,partsPath);
    if(partPath[strlen(partPath) - 1] != '/')
    {
        strcat(partPath, "/");
    }

    int pixelSize = getImagePixelSize(image);
    int pathFilenameIndex = strlen(partPath);
    int imageIndex = 1;

    int partTempHeight = partHeight;
    int partTempWidth = partWidth;

    for(int yPart = 0; yPart < image -> height; yPart+=partTempHeight)
    {
        for(int xPart = 0; xPart < image -> width; xPart+=partTempWidth)
        {
            Image* part = initImage();
            part -> colorType = image -> colorType;
            part -> bitDepth = image -> bitDepth;
            part -> interlaceType = image -> interlaceType;
            part -> filterMethod = image -> filterMethod;
            part -> compressionType = image -> compressionType;

            if(xPart + 2 * partWidth > image -> width)
                part -> width = image -> width - xPart;
            else
                part -> width = partWidth;

            if(yPart + 2 * partHeight > image -> height)
                part -> height = image -> height - yPart;
            else
                part -> height = partHeight;

            part -> pixels = calloc(part -> height, sizeof(png_bytep));
            for(int y = 0; y < part -> height; ++y)
            {
                part -> pixels[y] = calloc(part -> width * pixelSize, sizeof(png_byte));
            }

            for(int y = 0; y < part -> height; ++y)
            {
                for(int x = 0; x < part -> width; ++x)
                {
                    png_bytep partPixel = part -> pixels[y] + pixelSize * x;
                    png_bytep pixel = image -> pixels[yPart + y] + pixelSize * (xPart + x);
                    partPixel[0] = pixel[0];
                    partPixel[1] = pixel[1];
                    partPixel[2] = pixel[2];
                    if(pixelSize == 4)
                        partPixel[3] = pixel[3];
                }
            }

            partTempWidth = part -> width;
            partTempHeight = part -> height;

            char s_number[35];
            snprintf(s_number, 35, "%d", imageIndex);
            strcat(partPath, s_number);
            strcat(partPath, ".png");


            int error = saveImage(part, partPath);
            destroyImage(part);

            if (error) {
                printf("Can't access directory: %s\n", partPath);
                free(partPath);
                return;
            }

            partPath[pathFilenameIndex] = '\0';
            imageIndex++;

        }
    }

    free(partPath);
}
*/
