#ifndef HEADER_DUMMYSOUNDAGENT_H
#define HEADER_DUMMYSOUNDAGENT_H

#include "SoundAgent.h"

/**
 * NO sound and music.
 */
class DummySoundAgent : public SoundAgent {
    protected:
        virtual void setSoundVolume(int ) {}
        virtual void setMusicVolume(int ) {}
    public:
        virtual MIX_Audio *loadSound(const Path &) { return nullptr; }
        virtual MIX_Track *playSound(MIX_Audio *, int /*volume*/, int /*loops*/)
        { return nullptr; }

        virtual void playMusic(const Path &,
                BaseMsg *finished)
        {
            if (finished) {
                delete finished;
            }
        }
        virtual void stopMusic() {}
};

#endif
