#pragma once

#include "M4x4.hpp"
#include "V3d.hpp"
#include "V2d.hpp"

struct Triangle { 
    V3d p[3]; // World coords
    V2d t[3]; // Texture coords
    wchar_t sym;
	short col;

    Triangle &copyPoints(const Triangle &o);
    Triangle &copyTexture(const Triangle &o);

    Triangle &normalizePoints();

    Triangle addPoints(const V3d &o) const;
    Triangle multPoints(const M4x4 &m) const;
};