#ifndef COURSEWORK_TOOLS_H
#define COURSEWORK_TOOLS_H
#include "image.h"

void printHelp();
typedef enum
{
    COMMAND_NONE = 0,
    COMMAND_DRAW_CIRCLE = 1,
    COMMAND_REFLECTION_IMAGE = 2,
    COMMAND_COPY_IMAGE = 3,
} Command;

typedef struct
{
  Command command;
  int info;
  int circle_center_x;
  int circle_center_y;
  int radius;
  int circle_xl, circle_yl;
  int circle_xr, circle_yr;
  int lineWidth;
  Color lineColor;
  int fill;
  Color fillColor;

  char axis;
  int reflection_xl, reflection_yl;
  int reflection_xr, reflection_yr;

  int mirror_xl, mirror_yl;
  int mirror_xr, mirror_yr;
  int mirror_x_result, mirror_y_result;

  char* imageInputPath;
  char* imageOutputPath;
} Options;

Options* initOptionsStruct();
void destroyOptionStruct(Options* options);
#endif
