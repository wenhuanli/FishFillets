#ifndef HEADER_SDLSOUNDAGENT_H
#define HEADER_SDLSOUNDAGENT_H

#include "SoundAgent.h"

#include "SDL3/SDL.h"
#include "SDL3_mixer/SDL_mixer.h"
#include <string>
#include <vector>

/**
 * Sound and music.
 */
class SDLSoundAgent : public SoundAgent {
    private:
    static BaseMsg *ms_finished;
    MIX_Mixer *m_mixer;
    MIX_Audio *m_music;
    MIX_Track *m_musicTrack;
    std::vector<MIX_Track*> m_soundTracks;
    std::string m_playingPath;
    float m_soundVolume;
    float m_musicVolume;

    private:
        MIX_Track *findFreeTrack();

        static void musicFinished(void *userdata, MIX_Track *track);
    protected:
        virtual void own_init();
        virtual void own_shutdown();
        virtual void reinit();

        virtual void setSoundVolume(int volume);
        virtual void setMusicVolume(int volume);
    public:
        virtual MIX_Audio *loadSound(const Path &file);
        virtual MIX_Track *playSound(MIX_Audio *sound, int volume, int loops=0);

        virtual void playMusic(const Path &file,
                BaseMsg *finished);
        virtual void stopMusic();
};

#endif
