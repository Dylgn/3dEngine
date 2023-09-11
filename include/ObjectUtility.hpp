#pragma once

#include "PolyCollider.hpp"

namespace ObjectUtil {
    /** Get axis-alligned bounding box collider with given dimensions */
    PolyCollider *GetBoxCollider(const float &width, const float &height, const float &depth);
}