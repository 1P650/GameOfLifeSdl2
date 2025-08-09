#ifndef BUTTON_H
#define BUTTON_H
#include <SDL.h>

struct {
    int x; int y; int w; int h;
    int COLOR_PRESSED ;
    int COLOR_REST;
    char IS_PRESSED;
} GOL_Button;

#endif // BUTTON_H
