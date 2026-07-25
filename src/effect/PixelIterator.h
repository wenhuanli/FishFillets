#ifndef HEADER_PIXELITERATOR_H
#define HEADER_PIXELITERATOR_H

class SurfaceLock;

#include "V2.h"
#include "NoCopy.h"

#include "SDL3/SDL.h"

#include <memory>

/**
 * Iterator over surface pixels.
 */
class PixelIterator : public NoCopy {
    private:
        SDL_Surface *m_surface;
        const SDL_PixelFormatDetails *m_details;
        std::unique_ptr<SurfaceLock> m_lock;
        Uint8 *m_p;
        Uint8 *m_end;
        Uint8 m_bpp;
    public:
        PixelIterator(SDL_Surface *surface);
        virtual ~PixelIterator();
        void setPos(const V2 &pos);
        inline bool isValid() const { return m_p < m_end; }
        inline void inc() { m_p += m_bpp; }

        bool isTransparent() const;
        SDL_Color getColor() const;
        Uint32 getPixel() const;

        void putColor(const SDL_Color &color);
        void putPixel(Uint32 pixel);
};

#endif
