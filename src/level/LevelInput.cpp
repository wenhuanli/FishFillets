/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 * Copyright (C) 2026 Wenhuan Li (hikerhauk@gmail.com)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "LevelInput.h"

#include "Level.h"
#include "Keymap.h"

#include "KeyStroke.h"
#include "OptionAgent.h"
#include "MenuOptions.h"
#include "SubTitleAgent.h"

//-----------------------------------------------------------------
LevelInput::LevelInput(Level *level)
    : GameInput(level)
{
    m_keymap->registerKey(KeyStroke(SDLK_SPACE, SDL_KMOD_NONE),
            KeyDesc(KEY_SWITCH, "switch"));
    m_keymap->registerKey(KeyStroke(SDLK_F2, SDL_KMOD_NONE),
            KeyDesc(KEY_SAVE, "save"));
    m_keymap->registerKey(KeyStroke(SDLK_F3, SDL_KMOD_NONE),
            KeyDesc(KEY_LOAD, "load"));
    m_keymap->registerKey(KeyStroke(SDLK_BACKSPACE, SDL_KMOD_NONE),
            KeyDesc(KEY_RESTART, "restart"));
    m_keymap->registerKey(KeyStroke(SDLK_F5, SDL_KMOD_NONE),
            KeyDesc(KEY_SHOW_STEPS, "show number of steps"));

    KeyDesc undo = KeyDesc(KEY_UNDO, "undo");
    m_keymap->registerKey(KeyStroke(SDLK_MINUS, SDL_KMOD_NONE), undo);
    m_keymap->registerKey(KeyStroke(SDLK_KP_MINUS, SDL_KMOD_NONE), undo);

    KeyDesc redo = KeyDesc(KEY_REDO, "redo");
    m_keymap->registerKey(KeyStroke(SDLK_PLUS, SDL_KMOD_NONE), redo);
    m_keymap->registerKey(KeyStroke(SDLK_EQUALS, SDL_KMOD_NONE), redo);
    m_keymap->registerKey(KeyStroke(SDLK_KP_PLUS, SDL_KMOD_NONE), redo);
}
//-----------------------------------------------------------------
Level *
LevelInput::getLevel()
{
    return dynamic_cast<Level*>(m_state);
}
//-----------------------------------------------------------------
void
LevelInput::specKey(int keyIndex)
{
    switch (keyIndex) {
        case KEY_SWITCH:
            if (!getLevel()->isActing()) {
                getLevel()->switchFish();
            }
            break;
        case KEY_SAVE:
            if (!getLevel()->isActing()) {
                getLevel()->action_save();
            }
            break;
        case KEY_LOAD:
            if (!getLevel()->isShowing()) {
                getLevel()->action_load();
            }
            break;
        case KEY_RESTART:
            getLevel()->interruptShow();
            getLevel()->action_restart(1);
            break;
        case KEY_UNDO:
            if (!getLevel()->isShowing()) {
                getLevel()->action_undo(1);
            }
            break;
        case KEY_REDO:
            if (!getLevel()->isShowing()) {
                getLevel()->action_undo(-1);
            }
            break;
        case KEY_SHOW_STEPS:
            toggleParam("show_steps");
            break;
        default:
            GameInput::specKey(keyIndex);
    }
}
//-----------------------------------------------------------------
void
LevelInput::specStroke(const KeyStroke &stroke)
{
    if (!getLevel()->isActing()) {
        getLevel()->controlEvent(stroke);
    }
}
//-----------------------------------------------------------------
void
LevelInput::mouseEvent(const MouseStroke &buttons)
{
    getLevel()->controlMouse(buttons);
}
//-----------------------------------------------------------------
void
LevelInput::keyUp(const KeyStroke &stroke) {
    int index = m_keymap->indexPressed(stroke);
    switch (index) {
        case KEY_UNDO:
        case KEY_REDO:
            getLevel()->action_undo_finish();
            break;
        default:
            break;
    }
}

