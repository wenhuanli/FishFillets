/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 * Copyright (C) 2026 Wenhuan Li (hikerhauk@gmail.com)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "InputAgent.h"

#include "KeyBinder.h"
#include "InputHandler.h"

#include "MessagerAgent.h"
#include "SimpleMsg.h"
#include "UnknownMsgException.h"
#include "Name.h"
#include "MouseStroke.h"

#include "SDL3/SDL.h"

//-----------------------------------------------------------------
InputAgent::InputAgent() = default;
//-----------------------------------------------------------------
InputAgent::~InputAgent() = default;
//-----------------------------------------------------------------
/**
 * Set console handler to ScriptAgent.
 *
 * NOTE: KeyConsole() use Path which asks OptionAgent
 */
    void
InputAgent::own_init()
{
    m_keyBinder = std::make_unique<KeyBinder>();
    m_handler = nullptr;
    m_keys = SDL_GetKeyboardState(nullptr);
}
//-----------------------------------------------------------------
    void
InputAgent::own_update()
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_EVENT_QUIT:
                {
                    BaseMsg *msg = new SimpleMsg(Name::APP_NAME, "quit");
                    MessagerAgent::agent()->forwardNewMsg(msg);
                    break;
                }
            case SDL_EVENT_KEY_DOWN:
                m_keyBinder->keyDown(event.key.key, event.key.mod);
                if (m_handler) {
                    m_handler->keyEvent(KeyStroke(event.key.key, event.key.mod));
                }
                break;
            case SDL_EVENT_KEY_UP:
                if (m_handler) {
                    m_handler->keyUp(KeyStroke(event.key.key, event.key.mod));
                }
                break;
            case SDL_EVENT_MOUSE_BUTTON_DOWN:
                if (m_handler) {
                    m_handler->mouseEvent(MouseStroke(event.button));
                }
                break;
            default:
                break;
        }
    }

    if (m_handler) {
        Uint32 buttons;
        V2 mouseLoc = getMouseState(&buttons);
        m_handler->mouseState(mouseLoc, buttons);
    }
}
//-----------------------------------------------------------------
/**
 * Delete console.
 */
    void
InputAgent::own_shutdown()
{
    m_keyBinder.reset();
}
//-----------------------------------------------------------------
void
InputAgent::installHandler(InputHandler *handler)
{
    if (m_handler) {
        m_handler->takePressed(nullptr);
        m_handler->mouseState(V2(-1, -1), 0);
    }
    m_handler = handler;
    if (m_handler) {
        m_handler->takePressed(m_keys);
        Uint32 buttons;
        V2 mouseLoc = getMouseState(&buttons);
        m_handler->mouseState(mouseLoc, buttons);
    }
}
//-----------------------------------------------------------------
/**
 * Return mouse location.
 * @param out_buttons place where to store state of buttons
 * @return (mouseX, mouseY)
 */
    V2
InputAgent::getMouseState(Uint32 *out_buttons)
{
    float x;
    float y;
    Uint32 pressed = SDL_GetMouseState(&x, &y);
    if (out_buttons) {
        *out_buttons = pressed;
    }
    return V2(static_cast<int>(x), static_cast<int>(y));
}


