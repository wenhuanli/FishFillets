/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "VideoAgent.h"

#include "Log.h"
#include "Path.h"
#include "ImgException.h"
#include "SDLException.h"
#include "LogicException.h"
#include "AgentPack.h"
#include "SimpleMsg.h"
#include "StringMsg.h"
#include "UnknownMsgException.h"
#include "OptionAgent.h"
#include "SysVideo.h"

#include "SDL3_image/SDL_image.h"
#include <stdlib.h> // atexit()

//-----------------------------------------------------------------
/**
 * Init SDL and grafic window.
 * Register watcher for "fullscren" and "screen_*" options.
 * @throws SDLException if there is no usuable video mode
 */
    void
VideoAgent::own_init()
{
    m_window = NULL;
    m_screen = NULL;
    m_fullscreen = false;
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        throw SDLException(ExInfo("Init"));
    }
    atexit(SDL_Quit);

    registerWatcher("fullscreen");
    initVideoMode();

    setIcon(Path::dataReadPath("images/icon.png"));
}
//-----------------------------------------------------------------
/**
 * Draw all drawer from list.
 * First will be drawed first.
 */
    void
VideoAgent::own_update()
{
    drawOn(m_screen);
    SDL_UpdateWindowSurface(m_window);
}
//-----------------------------------------------------------------
/**
 * Shutdown SDL.
 */
    void
VideoAgent::own_shutdown()
{
    SDL_Quit();
}

//-----------------------------------------------------------------
/**
 * Load and set icon.
 * @throws ImgException
 */
    void
VideoAgent::setIcon(const Path &file)
{
    SDL_Surface *icon = IMG_Load(file.getNative().c_str());
    if (NULL == icon) {
        throw ImgException(ExInfo("Load")
                .addInfo("file", file.getNative()));
    }

    if (m_window) {
        SDL_SetWindowIcon(m_window, icon);
    }
    SDL_DestroySurface(icon);
}

//-----------------------------------------------------------------
/**
 * Init video mode along options.
 * Change window only when necessary.
 *
 * @throws SDLException when video mode cannot be made,
 * the old video mode remain usable
 */
    void
VideoAgent::initVideoMode()
{
    OptionAgent *options = OptionAgent::agent();
    int screen_width = options->getAsInt("screen_width", 640);
    int screen_height = options->getAsInt("screen_height", 480);
    std::string caption = options->getParam("caption", "A game");

    if (NULL == m_window
            || m_screen->w != screen_width
            || m_screen->h != screen_height)
    {
        changeVideoMode(screen_width, screen_height, caption);
    }
    else {
        SysVideo::setCaption(m_window, caption);
    }
}
//-----------------------------------------------------------------
/**
 * Init new video mode.
 * NOTE: m_window and m_screen pointers will change
 */
    void
VideoAgent::changeVideoMode(int screen_width, int screen_height,
        const std::string &caption)
{
    OptionAgent *options = OptionAgent::agent();
    SDL_WindowFlags videoFlags = getVideoFlags();
    m_fullscreen = options->getAsBool("fullscreen", false);
    if (m_fullscreen) {
        videoFlags |= SDL_WINDOW_FULLSCREEN;
    }

    if (m_window) {
        SDL_DestroyWindow(m_window);
        m_window = NULL;
        m_screen = NULL;
    }

    SDL_Window *newWindow =
        SDL_CreateWindow(caption.c_str(), screen_width, screen_height, videoFlags);
    if (NULL == newWindow && (videoFlags & SDL_WINDOW_FULLSCREEN)) {
        LOG_WARNING(ExInfo("unable to use fullscreen resolution, trying windowed")
                .addInfo("width", screen_width)
                .addInfo("height", screen_height));

        videoFlags = videoFlags & ~SDL_WINDOW_FULLSCREEN;
        newWindow = SDL_CreateWindow(caption.c_str(), screen_width,
                screen_height, videoFlags);
    }

    if (newWindow) {
        m_window = newWindow;
        m_screen = SDL_GetWindowSurface(m_window);
        if (NULL == m_screen) {
            throw SDLException(ExInfo("GetWindowSurface"));
        }
        //NOTE: must be two times to change MouseState
        SDL_WarpMouseInWindow(m_window, screen_width / 2.0f, screen_height / 2.0f);
        SDL_WarpMouseInWindow(m_window, screen_width / 2.0f, screen_height / 2.0f);
    }
    else {
        throw SDLException(ExInfo("CreateWindow")
                .addInfo("width", screen_width)
                .addInfo("height", screen_height));
    }
}
//-----------------------------------------------------------------
/**
 * Obtain video information about best video mode.
 * @return best video flags
 */
    SDL_WindowFlags
VideoAgent::getVideoFlags()
{
    return 0;
}
//-----------------------------------------------------------------
/**
 *  Toggle fullscreen.
 */
    void
VideoAgent::toggleFullScreen()
{
    bool success = SDL_SetWindowFullscreen(m_window, !m_fullscreen);
    if (success) {
        m_fullscreen = !m_fullscreen;
        m_screen = SDL_GetWindowSurface(m_window);
    }
    else {
        //NOTE: some platforms need reinit video
        changeVideoMode(m_screen->w, m_screen->h,
                OptionAgent::agent()->getParam("caption", "A game"));
    }
}
//-----------------------------------------------------------------
/**
 * Handle incoming message.
 * Messages:
 * - fullscreen ... toggle fullscreen
 *
 * @throws UnknownMsgException
 */
    void
VideoAgent::receiveSimple(const SimpleMsg *msg)
{
    if (msg->equalsName("fullscreen")) {
        OptionAgent *options = OptionAgent::agent();
        bool toggle = !(options->getAsBool("fullscreen"));
        options->setPersistent("fullscreen", toggle);
    }
    else {
        throw UnknownMsgException(msg);
    }
}
//-----------------------------------------------------------------
/**
 * Handle incoming message.
 * Messages:
 * - param_changed(fullscreen) ... handle fullscreen
 *
 * @throws UnknownMsgException
 */
    void
VideoAgent::receiveString(const StringMsg *msg)
{
    if (msg->equalsName("param_changed")) {
        std::string param = msg->getValue();
        if ("fullscreen" == param) {
            bool fs = OptionAgent::agent()->getAsBool("fullscreen");
            if (fs != m_fullscreen) {
                toggleFullScreen();
            }
        }
        else {
            throw UnknownMsgException(msg);
        }
    }
    else {
        throw UnknownMsgException(msg);
    }
}

