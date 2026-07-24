/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "SysVideo.h"

/**
 * Set window title.
 * @param title UTF-8 string
 *
 * NOTE: SDL3's SDL_SetWindowTitle is UTF-8 native on every platform, so the
 * X11/WIN32 workarounds SDL 1.2 needed here are gone.
 */
    void
SysVideo::setCaption(SDL_Window *window, const std::string &title)
{
    SDL_SetWindowTitle(window, title.c_str());
}
