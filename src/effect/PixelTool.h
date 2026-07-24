#ifndef HEADER_PIXELTOOL_H
#define HEADER_PIXELTOOL_H

#include "SDL3/SDL.h"

/**
 * Pixel rutines.
 */
class PixelTool {
    public:
        static bool colorEquals(const SDL_Color &color1,
                const SDL_Color &color2);
        static Uint32 convertColor(const SDL_PixelFormatDetails *format,
                const SDL_Color &color);

        static const SDL_PixelFormatDetails *formatDetails(SDL_Surface *surface);

        static SDL_Color getColor(SDL_Surface *surface, int x, int y);
        static void putColor(SDL_Surface *surface, int x, int y,
                const SDL_Color &color);

        static Uint32 getPixel(SDL_Surface *surface, int x, int y);
        static void putPixel(SDL_Surface *surface, int x, int y, Uint32 pixel);

        static Uint32 unpackPixel(Uint8 bpp, Uint8 *p);
        static void packPixel(Uint8 bpp, Uint8 *p, Uint32 pixel);
};

#endif
