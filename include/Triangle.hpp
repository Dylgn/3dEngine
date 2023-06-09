#pragma once

#include "V3d.hpp"
#include "V2d.hpp"

struct Triangle { 
    V3d p[3]; // World coords
    V2d t[3]; // Texture coords
    wchar_t sym;
	short col;
};