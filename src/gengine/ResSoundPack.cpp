/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 * Copyright (C) 2026 Wenhuan Li (hikerhauk@gmail.com)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "ResSoundPack.h"

#include "Path.h"
#include "OptionAgent.h"
#include "SoundAgent.h"

//-----------------------------------------------------------------
    void
ResSoundPack::unloadRes(MIX_Audio *res)
{
    MIX_DestroyAudio(res);
}
//-----------------------------------------------------------------
/**
 * Load unshared sound from file.
 * @return sound or nullptr
 */
    MIX_Audio *
ResSoundPack::loadSound(const Path &file)
{
    MIX_Audio *sound = nullptr;
    if (OptionAgent::agent()->getAsBool("sound", true)) {
        sound = SoundAgent::agent()->loadSound(file);
    }
    return sound;
}
//-----------------------------------------------------------------
/**
 * Store sound under this name.
 * Nothing is stored when sound cannot be loaded.
 */
    void
ResSoundPack::addSound(const std::string &name, const Path &file)
{
    MIX_Audio *sound = loadSound(file);
    if (sound) {
        addRes(name, sound);
    }
}
