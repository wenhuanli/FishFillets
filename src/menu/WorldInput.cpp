/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 * Copyright (C) 2026 Wenhuan Li (hikerhauk@gmail.com)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "WorldInput.h"

#include "WorldMap.h"
#include "Keymap.h"

#include "KeyStroke.h"
#include "MouseStroke.h"

//-----------------------------------------------------------------
WorldInput::WorldInput(WorldMap *world)
    : GameInput(world)
{
    //TODO: 'O' ... options, 'I' ... intro, 'E' ... exit, 'C' ... credits
    KeyDesc key_tab(KEY_TAB, "select next level");
    KeyDesc key_enter(KEY_ENTER, "run selected");

    m_keymap->registerKey(KeyStroke(SDLK_TAB, SDL_KMOD_NONE), key_tab);
    m_keymap->registerKey(KeyStroke(SDLK_RETURN, SDL_KMOD_NONE), key_enter);
}
//-----------------------------------------------------------------
WorldMap *
WorldInput::getWorld()
{
    return dynamic_cast<WorldMap*>(m_state);
}
//-----------------------------------------------------------------
void
WorldInput::specKey(int keyIndex)
{
    switch (keyIndex) {
        case KEY_TAB:
            getWorld()->selectNextLevel();
            break;
        case KEY_ENTER:
            getWorld()->runSelected();
            break;
        default:
            GameInput::specKey(keyIndex);
    }
}
//-----------------------------------------------------------------
void
WorldInput::mouseEvent(const MouseStroke &buttons)
{
    if (buttons.isLeft()) {
        getWorld()->runSelected();
    }
}

