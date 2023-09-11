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