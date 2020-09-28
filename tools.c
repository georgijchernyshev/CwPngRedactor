#include "tools.h"
#include <stdio.h>
#include <stdlib.h>

void printHelp()
{
    printf("\nUsage: ./pngeditor [command] [options] <file>\n");
    printf("Use '-h', '--help', '?' to display options line of pngeditor.\n");
    printf("Use '-I' or '--info' to display information about image.\n\n");
    printf("Commands:\n");
    printf("  -C, --circle\t\tDrawing a circle.\n");
    printf("  Command options:\n");
    printf("    -c, --center\t\tSet center coordinates.\n");
    printf("    -r, --radius\t\tSer circle radius.\n");
    printf("    -u, --upper-corner\t\tSet upper left corner of circumscribed square.\n");
    printf("    -d, --down-corner\t\tSer bottom right corner of circumscribed square.\n");
    printf("    -w, --width\t\tcircle thickness.\n");
    printf("    -l, --line-color\t\tSet color of circle line.\n");
    printf("    -p, --paint-color\t\tfill color, if needed.\n");
    printf("  -R, --reflection\t\tReflection the part of the image.\n");
    printf("  Command option:\n");
    printf("    -a, --axis\t\tSet the axis of reflection ('x' or 'y').\n");
    printf("    -s, --start\t\tSet upper left corner of the area of reflection.\n");
    printf("    -f, --finish\t\tSet bottom right cornet of the area of reflection.\n");
    printf("  -M, --mirror\t\tCopy of the path of image.\n");
    printf("  Command option:\n");
    printf("    -b, --begin-corner\t\tSet upper left corner of the area of copying.\n");
    printf("    -e, --end-corner\t\tSet bottom right corner of the area of copying.\n");
    printf("    -m, --mirror-area\t\tSet upper left corner of the area of past.\n");
    printf("  -i, --input\t\t\tInput file path.\n");
    printf("  -o, --output\t\t\tOutput file path.\n\n");
    printf("The PNG image must be in 8-bit RGB or RGBA format.\n");
}

Options* initOptionsStruct()
{
    Options* options = malloc(sizeof(Options));
    options -> command = COMMAND_NONE;
    options -> info = 0;

    options -> circle_center_x = -1;
    options -> circle_center_y = -1;
    options -> radius = -1;

    options -> circle_xl = -1;
    options -> circle_yl = -1;
    options -> circle_xr = -1;
    options -> circle_yr = -1;

    options -> lineWidth = -1;
    options -> lineColor = makeColor(-1, -1, -1, 255);
    options -> fill = 0;
    options -> fillColor = makeColor(-1, -1, -1, 255);

    options -> axis = 0;
    options -> reflection_xl = -1;
    options -> reflection_yl = -1;
    options -> reflection_xr = -1;
    options -> reflection_yr = -1;

    options -> mirror_xl = -1;
    options -> mirror_yl = -1;
    options -> mirror_xr = -1;
    options -> mirror_yr = -1;
    options -> mirror_x_result = -1;
    options -> mirror_y_result = -1;

    options -> imageInputPath = NULL;
    options -> imageOutputPath = NULL;

    return options;
}

void destroyOptionStruct(Options* options)
{
    free(options -> imageOutputPath);
    free(options -> imageInputPath);
    free(options);
}
