#ifndef HEADER_FINDERFIELD_H
#define HEADER_FINDERFIELD_H

class V2;

#include "NoCopy.h"

#include <vector>

/**
 * Array of closed nodes used for finding.
 */
class FinderField : public NoCopy {
    private:
        std::vector<std::vector<bool>> m_closed;
        int m_w;
        int m_h;
    public:
        FinderField(int w, int h);
        void reset();

        void markClosed(const V2 &loc);
        bool isClosed(const V2 &loc) const;
};

#endif
