/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "Font.h"

#include "Log.h"
#include "Path.h"
#include "TTFException.h"
#include "SDLException.h"
#include "Outline.h"

#ifdef HAVE_FRIBIDI
#include "fribidi.h"
#include <vector>
#endif

std::string
Font::biditize(const std::string &text)
{
#ifdef HAVE_FRIBIDI
    FriBidiCharType base = FRIBIDI_TYPE_ON;
    std::vector<FriBidiChar> logicalString(text.length() + 1);
    std::vector<FriBidiChar> visualString(text.length() + 1);

    int ucsLength = fribidi_charset_to_unicode(FRIBIDI_CHAR_SET_UTF8,
            const_cast<char*>(text.c_str()),
            text.length(), logicalString.data());
    fribidi_boolean ok = fribidi_log2vis(logicalString.data(), ucsLength, &base,
            visualString.data(), nullptr, nullptr, nullptr);
    if (!ok) {
        LOG_WARNING(ExInfo("cannot biditize text")
                .addInfo("text", text));
        return text;
    }

    std::vector<char> buffer(text.length() + 1);
    int length = fribidi_unicode_to_charset(FRIBIDI_CHAR_SET_UTF8,
            visualString.data(), ucsLength, buffer.data());
    return std::string(buffer.data(), length);
#else
    return text;
#endif
}

//-----------------------------------------------------------------
/**
 * Create new font from file.
 * Initialized TTF_Init when necessary.
 * @param file_ttf path to ttf file
 * @param height font height
 * @throws TTFException when cannot open font
 */
Font::Font(const Path &file_ttf, int height)
{
    m_ttfont = TTF_OpenFont(file_ttf.getNative().c_str(), height);
    if (!m_ttfont) {
        throw TTFException(ExInfo("OpenFont")
                .addInfo("file", file_ttf.getNative()));
    }

    //NOTE: bg color will be set to be transparent
    SDL_Color bg = {10, 10, 10, 0};
    m_bg = bg;
}
//-----------------------------------------------------------------
Font::~Font()
{
    TTF_CloseFont(m_ttfont);
}
//-----------------------------------------------------------------
/**
 * Prepare font rendering.
 * @throws TTFException when cannot init SDL_ttf.
 */
void
Font::init()
{
    if (!TTF_Init()) {
        throw TTFException(ExInfo("Init"));
    }
}
//-----------------------------------------------------------------
/**
 * Deinit font subsystem.
 */
void
Font::shutdown()
{
    TTF_Quit();
}

//-----------------------------------------------------------------
    int
Font::calcTextWidth(const std::string &text)
{
    int w;
    TTF_GetStringSize(m_ttfont, text.c_str(), 0, &w, nullptr);
    return w;
}
//-----------------------------------------------------------------
/**
 * Render text with this color.
 * @param text utf-8 encoded text
 * @param color text color
 * @return new rendered surface
 * @throws TTFException when render fails
 * @throws SDLException when converting fails
 */
SDL_Surface *
Font::renderText(const std::string &text, const SDL_Color &color) const
{
    std::string content = biditize(text);
    if (text.empty()) {
        content = " ";
        LOG_WARNING(ExInfo("empty text to render")
                .addInfo("r", color.r)
                .addInfo("g", color.g)
                .addInfo("b", color.b));
    }

    SDL_Surface *surface = TTF_RenderText_Shaded(m_ttfont, content.c_str(), 0,
            color, m_bg);
    if (!surface) {
        throw TTFException(ExInfo("RenderText")
                .addInfo("text", text));
    }

    //NOTE: at index 0 is bg color
    if (!SDL_SetSurfaceColorKey(surface, true, 0)) {
        throw SDLException(ExInfo("SetSurfaceColorKey"));
    }

    return surface;
}
//-----------------------------------------------------------------
/**
 * Render text with black outline around font.
 * @param text utf-8 encoded text
 * @param color text color
 * @param outlineWidth outline width
 * @return new rendered surface
 */
SDL_Surface *
Font::renderTextOutlined(const std::string &text,
                const SDL_Color &color, int outlineWidth) const
{
    static const SDL_Color BLACK = {0, 0, 0, 255};
    //NOTE: uses spaces to ensure space for outline
    SDL_Surface *surface = renderText(" " + text + " ", color);
    Outline outline(BLACK, outlineWidth);

    outline.drawOnColorKey(surface);
    return surface;
}

