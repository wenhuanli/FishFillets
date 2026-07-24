#ifndef HEADER_DRAWPRIMITIVES_H
#define HEADER_DRAWPRIMITIVES_H

#include "SDL3/SDL.h"

/**
 * Minimal SDL_Surface drawing primitives (filled circle, line).
 * Replaces the bundled, unmaintained SDL_gfx copy this project used to
 * carry for exactly two call sites (menu node markers, level rope decor).
 */
class DrawPrimitives {
    public:
        static SDL_Color colorFromRGBA(Uint32 rgba);
        static void filledCircle(SDL_Surface *surface, int cx, int cy,
                int radius, const SDL_Color &color);
        static void line(SDL_Surface *surface, int x1, int y1,
                int x2, int y2, const SDL_Color &color);
    private:
        static void blendPixel(SDL_Surface *surface, int x, int y,
                const SDL_Color &color);
};

#endif
