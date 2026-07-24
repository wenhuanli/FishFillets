#ifndef HEADER_SYSVIDEO_H
#define HEADER_SYSVIDEO_H

#include "SDL3/SDL.h"

#include <string>

/**
 * System dependend video functions.
 */
class SysVideo {
    public:
        static void setCaption(SDL_Window *window, const std::string &title);
};

#endif
