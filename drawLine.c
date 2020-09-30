void drawLine(int x1, int y1, int x2, int y2, png_bytep color, Png* image) {
    const int deltaX = abs(x2 - x1);
    const int deltaY = abs(y2 - y1);
    const int signX = x1 < x2 ? 1 : -1;
    const int signY = y1 < y2 ? 1 : -1;
    int error = deltaX - deltaY;

    setPixel(&(image->row_pointers[y2][x2*4]), color);
    while (x1 != x2 || y1 != y2) {
        setPixel(&(image->row_pointers[y1][x1*4]), color);
        const int error2 = error * 2;
        if (error2 > -deltaY) {
            error -= deltaY;
            x1 += signX;
        }
        if (error2 < deltaX) {
            error += deltaX;
            y1 += signY;
        }
    }
}


void drawLineThick(int x1, int y1, int x2, int y2, int thick, png_bytep color, Png* image) {
    if (x1 < 0 || x2 < 0 || y1 < 0 || y2 < 0 || x1 > image->width || x2 > image->width || y2 > image->height ||
        y1 > image->height || x1-thick/2 < 0 || x2-thick/2 < 0 || y1-thick/2 < 0 || y2-thick/2 < 0 ||
        x1+thick/2 > image->width || x2+thick/2 > image->width || y1+thick/2 > image->height ||
        y2+thick/2 > image->height)
        return;
    if (abs(y2-y1) > abs(x2-x1))
        for (int i = -thick/2; i < thick/2; i++)
            drawLine(x1+i, y1, x2+i, y2, color, image);
    else
        for (int i = -thick/2; i < thick/2; i++)
            drawLine(x1, y1+i, x2, y2+i, color, image);
}
