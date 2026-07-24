#ifndef HEADER_RESSOUNDPACK_H
#define HEADER_RESSOUNDPACK_H

class Path;

#include "ResourcePack.h"

#include "SDL3_mixer/SDL_mixer.h"

/**
 * Sound resources.
 */
class ResSoundPack : public ResourcePack<MIX_Audio*> {
    public:
        virtual const char *getName() const { return "sound_pack"; }

        static MIX_Audio *loadSound(const Path &file);
        void addSound(const std::string &name, const Path &file);
        virtual void unloadRes(MIX_Audio *res);
};

#endif
