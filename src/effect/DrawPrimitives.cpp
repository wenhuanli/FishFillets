/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "DrawPrimitives.h"

#include "PixelTool.h"
#include "SurfaceLock.h"

#include <cmath>
#include <cstdlib>

//-----------------------------------------------------------------
/**
 * Unpack a 0xRRGGBBAA color, the convention this codebase's callers
 * already used for the old SDL_gfx *Color() functions.
 */
SDL_Color
DrawPrimitives::colorFromRGBA(Uint32 rgba)
{
    SDL_Color color;
    color.r = static_cast<Uint8>((rgba >> 24) & 0xff);
    color.g = static_cast<Uint8>((rgba >> 16) & 0xff);
    color.b = static_cast<Uint8>((rgba >> 8) & 0xff);
    color.a = static_cast<Uint8>(rgba & 0xff);
    return color;
}
//-----------------------------------------------------------------
/**
 * Blend color onto the pixel at x, y. Surface must be locked.
 */
    void
DrawPrimitives::blendPixel(SDL_Surface *surface, int x, int y,
        const SDL_Color &color)
{
    if (x < 0 || y < 0 || x >= surface->w || y >= surface->h) {
        return;
    }
    if (255 == color.a) {
        PixelTool::putColor(surface, x, y, color);
        return;
    }

    SDL_Color under = PixelTool::getColor(surface, x, y);
    Uint8 a = color.a;
    SDL_Color blended;
    blended.r = static_cast<Uint8>((color.r * a + under.r * (255 - a)) / 255);
    blended.g = static_cast<Uint8>((color.g * a + under.g * (255 - a)) / 255);
    blended.b = static_cast<Uint8>((color.b * a + under.b * (255 - a)) / 255);
    blended.a = 255;
    PixelTool::putColor(surface, x, y, blended);
}
//-----------------------------------------------------------------
    void
DrawPrimitives::filledCircle(SDL_Surface *surface, int cx, int cy,
        int radius, const SDL_Color &color)
{
    SurfaceLock lock(surface);

    int r2 = radius * radius;
    for (int dy = -radius; dy <= radius; ++dy) {
        int dx = static_cast<int>(std::sqrt(static_cast<double>(r2 - dy * dy)));
        for (int x = cx - dx; x <= cx + dx; ++x) {
            blendPixel(surface, x, cy + dy, color);
        }
    }
}
//-----------------------------------------------------------------
/**
 * Draw a line using Bresenham's algorithm.
 */
    void
DrawPrimitives::line(SDL_Surface *surface, int x1, int y1, int x2, int y2,
        const SDL_Color &color)
{
    SurfaceLock lock(surface);

    int dx = std::abs(x2 - x1);
    int dy = -std::abs(y2 - y1);
    int sx = x1 < x2 ? 1 : -1;
    int sy = y1 < y2 ? 1 : -1;
    int err = dx + dy;

    while (true) {
        blendPixel(surface, x1, y1, color);
        if (x1 == x2 && y1 == y2) {
            break;
        }
        int e2 = 2 * err;
        if (e2 >= dy) {
            err += dy;
            x1 += sx;
        }
        if (e2 <= dx) {
            err += dx;
            y1 += sy;
        }
    }
}
