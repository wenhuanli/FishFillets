#ifndef HEADER_KEYSTROKE_H
#define HEADER_KEYSTROKE_H

#include "SDL3/SDL.h"
#include <string>

/**
 * KeyStroke.
 */
class KeyStroke {
    private:
        static const int STROKE_IGNORE = ~static_cast<int>(SDL_KMOD_CTRL|SDL_KMOD_ALT);
        SDL_Keycode m_sym;
        int m_mod;
    private:
        static int modStrip(int mod);
    public:
        KeyStroke(SDL_Keycode sym, int mod);

        SDL_Keycode getKey() const { return m_sym; }
        char toAscii() const;
        bool less(const KeyStroke &other) const;
        bool equals(const KeyStroke &other) const;
        std::string toString() const;
};

/**
 * KeyStroke comparation.
 */
struct stroke_less
{
    bool operator()(const KeyStroke &left, const KeyStroke &right) const
    {
        return left.less(right);
    }
};

#endif
