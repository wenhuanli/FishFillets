#ifndef HEADER_NOCOPY_H
#define HEADER_NOCOPY_H

/**
 * Class with deleted copy constructor and assign operator.
 */
class NoCopy {
    public:
        NoCopy() = default;
        virtual ~NoCopy() = default;

        NoCopy(const NoCopy &) = delete;
        NoCopy &operator=(const NoCopy &) = delete;
};

#endif
