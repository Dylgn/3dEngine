#pragma once

#include "PolyCollider.hpp"
#include "Body.hpp"

namespace ObjectUtil {
    /** Get axis-alligned bounding box collider with given dimensions */
    PolyCollider *GetBoxCollider(const float &width, const float &height, const float &depth);

    /** Get Axis-alligned bounding box with given dimensions */
    Body *GetBox(const float &width, const float &height, const float &depth);

    /** Get Axis-alligned bounding box with given dimensions*/
    Rbody *GetRigidBox(const float &width, const float &height, const float &depth);
}