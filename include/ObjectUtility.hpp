#pragma once

#include "Rigidbody.hpp"
#include "PolyCollider.hpp"

namespace ObjectUtil {
    PolyCollider *GetBoxCollider(const float &width, const float &height, const float &depth);

    Body *GetBox(const float &width, const float &height, const float &depth);

    Rigidbody *GetRigidBox(const float &width, const float &height, const float &depth);
}