#include "ObjectUtility.hpp"

PolyCollider *ObjectUtil::GetBoxCollider(const float &width, const float &height, const float &depth) {
    return new PolyCollider{
        {0,0,0},
        {width,0,0},
        {0,height,0},
        {width,height,0},
        {0,0,depth},
        {width,0,depth},
        {0,height,depth},
        {width,height,depth}
    };
}

Body *ObjectUtil::GetBox(const float &width, const float &height, const float &depth) {
    return new Body{GetBoxCollider(width, height, depth)};
}

Rbody *ObjectUtil::GetRigidBox(const float &width, const float &height, const float &depth) {
    return new Rbody{GetBoxCollider(width, height, depth)};
}