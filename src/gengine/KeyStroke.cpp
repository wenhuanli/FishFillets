/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 * Copyright (C) 2026 Wenhuan Li (hikerhauk@gmail.com)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "KeyStroke.h"

#include "StringTool.h"

//-----------------------------------------------------------------
/**
 * Create new keystroke.
 * NOTE: SDL_KMOD_ALT mean (SDL_KMOD_LALT and SDL_KMOD_RALT),
 * i.e. either ALTs pressed!
 *
 * @param sym SDL_Keycode
 * @param mod SDL_Keymod ored
 */
KeyStroke::KeyStroke(SDL_Keycode sym, int mod)
{
    m_sym = sym;
    m_mod = modStrip(mod);
}
//-----------------------------------------------------------------
/**
 * Strip ignored modes.
 * SDL_KMOD_SHIFT|SDL_KMOD_NUM|SDL_KMOD_CAPS|SDL_KMOD_MODE are ignored.
 */
    int
KeyStroke::modStrip(int mod)
{
    return mod & ~STROKE_IGNORE;
}
//-----------------------------------------------------------------
/**
 * KeyStroke comparation.
 *
 * @param other other keystroke
 * @return this < other
 */
bool
KeyStroke::less(const KeyStroke &other) const
{
    bool result = m_sym < other.m_sym;
    if (m_sym == other.m_sym) {
        result = m_mod < other.m_mod;
    }
    return result;
}
//-----------------------------------------------------------------
/**
 * Test keyStroke equality.
 * SDL_KMOD_NUM|SDL_KMOD_CAPS|SDL_KMOD_MODE are ignored.
 *
 * @param other other keystroke
 * @return this == other
 */
bool
KeyStroke::equals(const KeyStroke &other) const
{
    return m_sym == other.m_sym &&
        m_mod == other.m_mod;
}
//-----------------------------------------------------------------
/**
 * Approximate the plain ASCII character typed by this stroke, for the
 * debug console. SDL3 dropped SDL 1.2's per-key unicode translation in
 * favor of a separate text-input event stream; console text entry never
 * supported more than ASCII anyway (see the "TODO: support UTF-8" note
 * in ConsoleInput), so this derives the character straight from the
 * keycode + shift state instead of wiring a whole new input pathway.
 * @return ascii char, or 0 when this stroke doesn't map to plain text
 */
char
KeyStroke::toAscii() const
{
    static const char *DIGITS_SHIFTED = ")!@#$%^&*(";

    bool shift = (m_mod & SDL_KMOD_SHIFT) != 0;
    if (m_sym >= SDLK_A && m_sym <= SDLK_Z) {
        return static_cast<char>(shift ? m_sym - 32 : m_sym);
    }
    if (m_sym >= SDLK_0 && m_sym <= SDLK_9) {
        return shift ? DIGITS_SHIFTED[m_sym - SDLK_0] : static_cast<char>(m_sym);
    }
    if (m_sym > 0 && m_sym < 128 && !shift) {
        return static_cast<char>(m_sym);
    }
    return 0;
}
//-----------------------------------------------------------------
/**
 * Return text fashion.
 */
std::string
KeyStroke::toString() const
{
    std::string result = SDL_GetKeyName(m_sym);
    result.append("+" + StringTool::toString(m_mod));
    return result;
}

