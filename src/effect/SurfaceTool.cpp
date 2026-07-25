/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 * Copyright (C) 2026 Wenhuan Li (hikerhauk@gmail.com)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "SurfaceTool.h"

#include "PixelTool.h"
#include "SDLException.h"

//-----------------------------------------------------------------
/**
 * Return new empty surface with the same format.
 * @param surface surface to ask for pixel format
 * @param width width or 0 for the same width
 * @param height height or 0 for the same height
 * @throws SDLException when function fails
 */
    SDL_Surface *
SurfaceTool::createEmpty(SDL_Surface *surface, int width, int height)
{
    if (!width) {
        width = surface->w;
    }
    if (!height) {
        height = surface->h;
    }

    SDL_Surface *result = SDL_CreateSurface(width, height, surface->format);
    if (nullptr == result) {
        throw SDLException(ExInfo("CreateSurface"));
    }
    return result;
}
//-----------------------------------------------------------------
/**
 * Return new surface with transparent background.
 */
    SDL_Surface *
SurfaceTool::createTransparent(int w, int h, const SDL_Color &transparent)
{
    SDL_Surface *surface = SDL_CreateSurface(w, h, SDL_PIXELFORMAT_RGBA32);
    if (nullptr == surface) {
        throw SDLException(ExInfo("CreateSurface"));
    }

    Uint32 transparentKey = PixelTool::convertColor(
            PixelTool::formatDetails(surface), transparent);
    SDL_SetSurfaceColorKey(surface, true, transparentKey);
    SDL_SetSurfaceRLE(surface, true);

    SurfaceTool::alphaFill(surface, nullptr, transparent);
    return surface;
}
//-----------------------------------------------------------------
/**
 * Return new cloned surface.
 * @return new surface, free it after use
 * @throws SDLException when function fails
 */
    SDL_Surface *
SurfaceTool::createClone(SDL_Surface *surface)
{
    SDL_Surface *clone = SDL_ConvertSurface(surface, surface->format);
    if (nullptr == clone) {
        throw SDLException(ExInfo("ConvertSurface"));
    }
    return clone;
}
//-----------------------------------------------------------------
/**
 * Fill surface with given color. Alpha value in color is supported.
 * The final blit rectangle is saved in dstrect.
 *
 * @param surface surface to fill
 * @param dstrect dstrect or nullptr (the whole surface will be filled with color).
 * @param color {red, green, blue, alpha}
 */
    void
SurfaceTool::alphaFill(SDL_Surface *surface, SDL_Rect *dstrect,
        const SDL_Color &color)
{
    int w = surface->w;
    int h = surface->h;
    if (dstrect) {
        w = dstrect->w;
        h = dstrect->h;
    }
    SDL_Surface *canvas = createEmpty(surface, w, h);
    Uint32 pixel = PixelTool::convertColor(PixelTool::formatDetails(canvas), color);
    SDL_FillSurfaceRect(canvas, nullptr, pixel);
    SDL_SetSurfaceAlphaMod(canvas, color.a);
    SDL_SetSurfaceRLE(canvas, true);

    SDL_BlitSurface(canvas, nullptr, surface, dstrect);
    SDL_DestroySurface(canvas);
}

