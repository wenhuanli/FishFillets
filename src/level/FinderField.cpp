/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "FinderField.h"

#include "V2.h"

//-----------------------------------------------------------------
/**
 * Two dimensional array of booleans.
 * NOTE: [y][x] indexes
 */
FinderField::FinderField(int w, int h)
    : m_closed(h, std::vector<bool>(w, false))
{
    m_w = w;
    m_h = h;
}
//-----------------------------------------------------------------
/**
 * Erase all marks.
 */
void
FinderField::reset()
{
    for (int y = 0; y < m_h; ++y) {
        m_closed[y].assign(m_w, false);
    }
}
//-----------------------------------------------------------------
/**
 * Mark given place as closed.
 */
void
FinderField::markClosed(const V2 &loc)
{
    int x = loc.getX();
    int y = loc.getY();

    if ((0 <= x && x < m_w) && (0 <= y && y < m_h)) {
        m_closed[y][x] = true;
    }
}
//-----------------------------------------------------------------
/**
 * Returns true when place is closed.
 * NOTE: all places outside array are marked as closed
 */
bool
FinderField::isClosed(const V2 &loc) const
{
    int x = loc.getX();
    int y = loc.getY();

    bool result = true;
    if ((0 <= x && x < m_w) && (0 <= y && y < m_h)) {
        result = m_closed[y][x];
    }
    return result;
}


