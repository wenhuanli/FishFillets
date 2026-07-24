/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "SDLSoundAgent.h"

#include "Log.h"
#include "Path.h"
#include "ExInfo.h"
#include "SDLException.h"
#include "MixException.h"
#include "Random.h"
#include "BaseMsg.h"
#include "OptionAgent.h"

BaseMsg *SDLSoundAgent::ms_finished = NULL;
//-----------------------------------------------------------------
/**
 * Init sound subsystem.
 * @throws SDLException when SDL_INIT_AUDIO cannot be initialized
 * @throws MixException when SDL_mixer cannot be initialized
 */
    void
SDLSoundAgent::own_init()
{
    SoundAgent::own_init();
}
//-----------------------------------------------------------------
    void
SDLSoundAgent::own_shutdown()
{
    stopMusic();

    for (size_t i = 0; i < m_soundTracks.size(); ++i) {
        MIX_DestroyTrack(m_soundTracks[i]);
    }
    m_soundTracks.clear();

    if (m_mixer) {
        MIX_DestroyMixer(m_mixer);
        m_mixer = NULL;
    }
    MIX_Quit();
    SDL_QuitSubSystem(SDL_INIT_AUDIO);
}
//-----------------------------------------------------------------
/**
 * Reinit the sound subsystem.
 */
    void
SDLSoundAgent::reinit()
{
    m_mixer = NULL;
    m_music = NULL;
    m_musicTrack = NULL;
    m_soundVolume = 1.0f;
    m_musicVolume = 1.0f;
    if (!SDL_InitSubSystem(SDL_INIT_AUDIO)) {
        throw SDLException(ExInfo("SDL_InitSubSystem"));
    }
    if (!MIX_Init()) {
        throw MixException(ExInfo("MIX_Init"));
    }

    int frequency =
       OptionAgent::agent()->getAsInt("sound_frequency", 44100);
    SDL_AudioSpec spec;
    spec.format = SDL_AUDIO_S16;
    spec.channels = 2;
    spec.freq = frequency;
    m_mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec);
    if (!m_mixer) {
        throw MixException(ExInfo("MIX_CreateMixerDevice"));
    }

    SoundAgent::reinit();
}
//-----------------------------------------------------------------
/**
 * Load unshared sound from file.
 * @return sound or NULL
 */
    MIX_Audio *
SDLSoundAgent::loadSound(const Path &file)
{
    MIX_Audio *sound = MIX_LoadAudio(m_mixer, file.getNative().c_str(), false);
    if (NULL == sound) {
        LOG_WARNING(ExInfo("cannot load sound")
                .addInfo("path", file.getNative())
                .addInfo("Mix", SDL_GetError()));
    }
    return sound;
}
//-----------------------------------------------------------------
/**
 * Find a track that is not currently playing, reusing our pool of tracks
 * before creating a new one.
 * NOTE: mirrors SDL 1.2's automatic free-channel selection
 * (Mix_PlayChannel(-1, ...)); the new SDL3_mixer API has no equivalent
 * built-in channel pool, tracks are caller-owned objects.
 */
    MIX_Track *
SDLSoundAgent::findFreeTrack()
{
    for (size_t i = 0; i < m_soundTracks.size(); ++i) {
        if (!MIX_TrackPlaying(m_soundTracks[i])) {
            return m_soundTracks[i];
        }
    }

    MIX_Track *track = MIX_CreateTrack(m_mixer);
    if (track) {
        m_soundTracks.push_back(track);
    }
    return track;
}
//-----------------------------------------------------------------
/**
 * Play this sound.
 * @param sound audio to play
 * @param volume percentage sound volume
 * @param loops numer of loops. 0=play once, 1=play twice, -1=play infinite
 *
 * @return track where the sound is played, or NULL on error or when
 * sound is NULL
 */
    MIX_Track *
SDLSoundAgent::playSound(MIX_Audio *sound, int volume, int loops)
{
    MIX_Track *track = NULL;
    if (sound) {
        track = findFreeTrack();
        if (NULL == track
                || !MIX_SetTrackAudio(track, sound)
                || !MIX_SetTrackLoops(track, loops))
        {
            //NOTE: maybe there are too few open channels
            LOG_WARNING(ExInfo("cannot play sound")
                    .addInfo("Mix", SDL_GetError()));
            track = NULL;
        }
        else {
            MIX_SetTrackGain(track, m_soundVolume * volume / 100.0f);
            MIX_PlayTrack(track, 0);
        }
    }

    return track;
}

//-----------------------------------------------------------------
/**
 * Set sound volume.
 * NOTE: all already running sound will get equals volume
 * @param volume percentage volume, e.g. 30%=30
 */
    void
SDLSoundAgent::setSoundVolume(int volume)
{
    m_soundVolume = volume / 100.0f;
    if (m_soundVolume > 1.0f) {
        m_soundVolume = 1.0f;
    }
    else if (m_soundVolume < 0.0f) {
        m_soundVolume = 0.0f;
    }

    for (size_t i = 0; i < m_soundTracks.size(); ++i) {
        MIX_SetTrackGain(m_soundTracks[i], m_soundVolume);
    }
}

//---------------------------------------------------------------------------
// Music part
//---------------------------------------------------------------------------

/**
 * Play music.
 * @param file path to music file (i.e. *.ogg)
 * @param finished send this message when music is finished.
 * If finished is NULL, play music forever.
 */
void
SDLSoundAgent::playMusic(const Path &file,
        BaseMsg *finished)
{
    // The same music is not restarted when it is not needed.
    if (m_playingPath == file.getPosixName()
            && ms_finished == NULL && finished == NULL) {
        return;
    }

    stopMusic();
    m_playingPath = file.getPosixName();

    m_music = MIX_LoadAudio(m_mixer, file.getNative().c_str(), false);
    if (NULL == m_music) {
        LOG_WARNING(ExInfo("cannot play music")
                .addInfo("music", file.getNative())
                .addInfo("Mix", SDL_GetError()));
        return;
    }

    m_musicTrack = MIX_CreateTrack(m_mixer);
    MIX_SetTrackAudio(m_musicTrack, m_music);
    MIX_SetTrackGain(m_musicTrack, m_musicVolume);

    if (finished) {
        ms_finished = finished;
        //NOTE: play once, ms_finished fires when it naturally stops
        MIX_SetTrackLoops(m_musicTrack, 0);
        MIX_SetTrackStoppedCallback(m_musicTrack, musicFinished, this);
    }
    else {
        //NOTE: SDL 1.2's SDLMusicLooper spliced raw PCM around custom
        //loop points (from *.ogg.meta) for a seamless ambient loop.
        //SDL3_mixer's track API has no equivalent raw-buffer hook, so
        //ambient music now uses a plain whole-track infinite loop.
        MIX_SetTrackLoops(m_musicTrack, -1);
    }
    MIX_PlayTrack(m_musicTrack, 0);
}
//-----------------------------------------------------------------
/**
 * @param volume percentage volume, e.g. 30%=30
 */
    void
SDLSoundAgent::setMusicVolume(int volume)
{
    m_musicVolume = volume / 100.0f;
    if (m_musicVolume > 1.0f) {
        m_musicVolume = 1.0f;
    }
    else if (m_musicVolume < 0.0f) {
        m_musicVolume = 0.0f;
    }

    if (m_musicTrack) {
        MIX_SetTrackGain(m_musicTrack, m_musicVolume);
    }
}
//-----------------------------------------------------------------
    void
SDLSoundAgent::stopMusic()
{
    if (m_musicTrack) {
        MIX_SetTrackStoppedCallback(m_musicTrack, NULL, NULL);
        MIX_StopTrack(m_musicTrack, 0);
        MIX_DestroyTrack(m_musicTrack);
        m_musicTrack = NULL;
    }
    if (m_music) {
        MIX_DestroyAudio(m_music);
        m_music = NULL;
    }
    if (ms_finished) {
        delete ms_finished;
        ms_finished = NULL;
    }
    m_playingPath = "";
}
//-----------------------------------------------------------------
/**
 * Callback called when music is finished.
 * NOTE: no one exception can be passed to "C" SDL_mixer code
 */
    void
SDLSoundAgent::musicFinished(void * /*userdata*/, MIX_Track * /*track*/)
{
    try {
        if (ms_finished) {
            ms_finished->sendClone();
        }
        else {
            LOG_WARNING(ExInfo("NULL == ms_finished"));
        }
    }
    catch (std::exception &e) {
        LOG_WARNING(ExInfo("musicFinished error")
                .addInfo("what", e.what()));
    }
    catch (...) {
        LOG_ERROR(ExInfo("musicFinished error - unknown exception"));
    }
}
