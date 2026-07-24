/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "InputHandler.h"

#include "ExInfo.h"

//-----------------------------------------------------------------
InputHandler::InputHandler()
: m_mouseLoc(-1, -1)
{
    m_pressed = NULL;
    m_buttons = 0;
}
//-----------------------------------------------------------------
void
InputHandler::mouseState(const V2 &loc, Uint32 buttons)
{
    m_mouseLoc = loc;
    m_buttons = buttons;
}
//-----------------------------------------------------------------
bool
InputHandler::isPressed(SDL_Keycode key) const
{
    //NOTE: SDL3's keyboard-state array is indexed by scancode (physical
    //key), not keycode (mapped key), unlike SDL 1.2's array
    if (!m_pressed) {
        return false;
    }
    SDL_Scancode scancode = SDL_GetScancodeFromKey(key, NULL);
    return m_pressed[scancode];
}
//-----------------------------------------------------------------
bool
InputHandler::isLeftPressed() const
{
    return m_buttons & SDL_BUTTON_LMASK;
}
//-----------------------------------------------------------------
bool
InputHandler::isMiddlePressed() const
{
    return m_buttons & SDL_BUTTON_MMASK;
}
//-----------------------------------------------------------------
bool
InputHandler::isRightPressed() const
{
    return m_buttons & SDL_BUTTON_RMASK;
}
//-----------------------------------------------------------------
std::string
InputHandler::toString() const
{
    return ExInfo("input")
        .addInfo("mouse", m_mouseLoc.toString())
        .addInfo("buttons", m_buttons).info();
}

