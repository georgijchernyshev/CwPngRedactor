#include "image.h"
#include <getopt.h>
#include "tools.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
    if(argc == 1)
    {
        printHelp();
        return 0;
    }

    int longIndex = 0;
    const char* optionsString = "h?CRMIc:r:u:d:w:l:p:a:s:f:b:e:m:i:o:";//копирование R(replication), отображение M(mirror)
    struct option longOptions[] =
    {
            {"info", no_argument,NULL,'I'},
            {"help", no_argument, NULL, 'h'},
            {"circle", no_argument, NULL, 'C'},
            {"reflection", no_argument, NULL, 'R'},
            {"mirror", no_argument, NULL, 'M'},
            {"center", required_argument, NULL, 'c'},
            {"radius", required_argument, NULL, 'r'},
            {"upper-corner", required_argument, NULL, 'u'},
            {"down-corner", required_argument, NULL, 'd'},
            {"width", required_argument, NULL, 'w'},
            {"line-color", required_argument, NULL, 'l'},
            {"paint-color", required_argument, NULL, 'p'},
            {"axis", required_argument, NULL, 'a'},
            {"start", required_argument, NULL, 's'},
            {"finish", required_argument, NULL, 'f'},
            {"beginning-corner", required_argument, NULL, 'b'},
            {"end-corner", required_argument,NULL, 'e'},
            {"mirror-area", required_argument, NULL, 'm'},
            {"input", required_argument, NULL, 'i'},
            {"output", required_argument, NULL, 'o'},
            {NULL, 0, NULL, 0}
    };


    Options* options = initOptionsStruct();
    int  option = getopt_long(argc, argv, optionsString, longOptions, &longIndex);
    while (option != -1)
    {
        switch (option)
        {
            case 'I':
                options -> info = 1;
                break;
            case 'C':
                if(options -> command == COMMAND_NONE)
                    options -> command = COMMAND_DRAW_CIRCLE;
                else
                    printf("command has already been selected, Skipping.\n");
                break;
            case 'R':
                if(options -> command == COMMAND_NONE)
                    options -> command = COMMAND_REFLECTION_IMAGE;
                else
                    printf("command has already been selected, Skipping.\n");
                break;
            case 'M':
                if(options -> command == COMMAND_NONE)
                    options -> command = COMMAND_COPY_IMAGE;
                else
                    printf("command has already been selected, Skipping.\n");
                break;
            case 'c':
                if(sscanf(optarg, "%d:%d", &(options -> circle_center_x), &(options -> circle_center_y)) != 2)
                    printf("Failed to read center coordinate %s, try 'x:y'.\n", optarg);
                break;
            case 'r':
                if(sscanf(optarg,"%d", &(options -> radius)) != 1)
                    printf("Failed to read radius %s, try -r of --radius <int>.\n", optarg);
                break;
            case 'u':
                if(sscanf(optarg, "%d:%d", &(options -> circle_xl), &(options -> circle_yl)) != 2)
                    printf("Failed to read coordinate %s. try 'x:y'.\n", optarg);
                break;
            case 'd':
                if(sscanf(optarg, "%d:%d", &(options -> circle_xr), &(options -> circle_yr)) != 2)
                    printf("Failed to read coordinate %s. try 'x:y'.\n", optarg);
                break;
            case 'w':
                if(sscanf(optarg, "%d", &(options -> lineWidth)) != 1)
                    printf("Failed to read width of line %s. tre '-w or --width <int>.\n", optarg);
                break;
            case 'l':
                if(sscanf(optarg,"%d:%d:%d", &(options -> lineColor.r), &(options -> lineColor.g), &(options -> lineColor.b)) != 3)
                    printf("Recognize color %s fail, try 'r:g:b'.\n", optarg);
                break;
            case 'p':
                if(sscanf(optarg,"%d:%d:%d", &(options -> fillColor.r), &(options -> fillColor.g), &(options -> fillColor.b)) != 3)
                    printf("Recognize color %s fail, try 'r:g:b'.\n", optarg);
                options -> fill = 1;
                break;
            case 'a':
                if(strcmp(optarg, "x") == 0)
                    options -> axis = 'x';
                else if (strcmp(optarg, "y") == 0)
                    options -> axis = 'y';
                else
                    printf("Axis set incorrectly try 'x or y'.\n");
                break;
            case 's':
                if(sscanf(optarg, "%d:%d", &(options -> reflection_xl), &(options -> reflection_yl)) != 2)
                    printf("Failed to read coordinate %s. try 'x:y'.\n", optarg);
                break;
            case 'f':
                if(sscanf(optarg, "%d:%d", &(options -> reflection_xr), &(options -> reflection_yr)) != 2)
                    printf("Failed to read coordinate %s. try 'x:y'.\n", optarg);
                break;
            case 'b':
                if(sscanf(optarg, "%d:%d", &(options -> mirror_xl), &(options -> mirror_yl)) != 2)
                    printf("Failed to read coordinate %s. try 'x:y'.\n", optarg);
                break;
            case 'e':
                if(sscanf(optarg, "%d:%d", &(options -> mirror_xr), &(options -> mirror_yr)) != 2)
                    printf("Failed to read coordinate %s. try 'x:y'.\n", optarg);
                break;
            case 'm':
                if(sscanf(optarg, "%d:%d", &(options -> mirror_x_result), &(options -> mirror_y_result)) != 2)
                    printf("Failed to read coordinate %s. try 'x:y'.\n", optarg);
                break;
            case 'i':
                options -> imageInputPath = (char*)calloc(strlen(optarg)+1, sizeof(char));
                strcpy(options -> imageInputPath, optarg);
                break;
            case 'o':
                options -> imageOutputPath = (char*)calloc(strlen(optarg)+1, sizeof(char));
                strcpy(options -> imageOutputPath, optarg);
                break;
            case 'h':
            case '?':
                printHelp();
                destroyOptionStruct(options);
                return 0;
            default:
                printf("Parameter '%s' not recognized.\n", argv[optind]);
                printHelp();
                destroyOptionStruct(options);
                return 0;

        }

        option = getopt_long(argc, argv, optionsString, longOptions, &longIndex);
    }

    argc -= optind;
    argv += optind;

    if(options -> imageInputPath == NULL && argc != 1)
    {
        if(argc < 1)
            printf("You did not enter path to the image.\n");
        else {
            printf("You have entered too many arguments.\n");
            for (int i = 0; i <= argc; ++i) {
                printf("Arg: %s", argv[i]);
            }
        }
        printHelp();
        destroyOptionStruct(options);
        return 0;
    }

    Image * image = initImage();
    char* path = (options -> imageInputPath == NULL)?argv[0]:options->imageInputPath;
    if(!readImage(image, path))
    {
        destroyOptionStruct(options);
        destroyImage(image);
        return 0;
    }

    if(options -> info)
        printImageInfo(image);
    int err = 0;
    switch (options -> command) {
        case COMMAND_DRAW_CIRCLE:
            if (options->lineWidth >= 0 && checkColor(options->lineColor) && (!options->fill || checkColor(options->fillColor)))
            {
                if(checkPoint(image, options -> circle_center_x, options -> circle_center_y) && options -> radius >=0)
                {
                    circleDrawingWithRadiusAndCenter(options -> circle_center_x, options -> circle_center_y,
                                                     options -> radius, options -> lineWidth, options-> lineColor, options -> fill,
                                                     options -> fillColor, image);
                }else if((checkPoint(image, options -> circle_xl, options -> circle_yl) &&
                          checkPoint(image, options -> circle_xr, options -> circle_yr)))
                {
                    if(options->circle_xr <= options->circle_xl || options->circle_yr <= options->circle_yl ||
                       (options->circle_xr - options->circle_xl) != (options->circle_yr - options->circle_yl))
                    {
                        printf("Invalid square corner.\n");
                        err = 1;
                    } else{
                        circleDrawingWithCircumscribedSquare(options->circle_xl, options->circle_yl, options->circle_xr, options->circle_yr,
                                                             options->lineWidth, options->lineColor, options->fill, options->fillColor, image);
                    }
                } else{
                    printf("Some options are invalid for command '-C'.\n");
                    err = 1;
                }
            }else{
                printf("Some options are invalid for command '-C'.\n");
                err = 1;
            }
            break;
        case COMMAND_REFLECTION_IMAGE:
            if(options -> axis != 0 && options -> reflection_xl > 0 && options -> reflection_yl > 0 &&
               options -> reflection_xr > 0 && options -> reflection_yr > 0)
            {
                if(reflectionOfPartOfImage(options->reflection_xl, options -> reflection_yl,
                                           options -> reflection_xr, options -> reflection_yr, options -> axis, image))
                    err = 1;
            }
            else
            {
                printf("Some arguments are invalid for the 'R' option.\n");
                err = 1;
            }
            break;
        case COMMAND_COPY_IMAGE:
            if(options -> mirror_xl >= 0 && options -> mirror_yl >= 0 && options -> mirror_yr > 0 &&
               options -> mirror_xr > 0 && options -> mirror_x_result > 0 && options -> mirror_y_result > 0)
            {
                if(copyPartOfImage(options -> mirror_xl, options -> mirror_yl, options -> mirror_xr, options -> mirror_yr,
                                options -> mirror_x_result, options -> mirror_y_result, image))
                    err = 1;
            }
            else
            {
                printf("Some arguments are invalid for the 'M' option.\n");
                err = 1;
            }
            break;
        case COMMAND_NONE:
            printf("You did not chose command.\n");
            err = 1;
            break;
    }

    if(!err)
    {
        if(options -> imageOutputPath != NULL)
        {
            saveImage(image, options -> imageOutputPath);
        }
        else{
            saveImage(image, image -> path);
        }
    }

    destroyImage(image);
    destroyOptionStruct(options);
    return 0;
}