#include "Entity.hpp"

V3d Entity::ToWorld(const V3d &v) const
{
    return this->rot.rotate(v) + this->pos;
}

V3d Entity::ToLocal(const V3d &v) const
{
    return this->rot.inverse().rotate(v - this->pos);
}

float Entity::GetInverseMass() const
{
    return 1.0f / mass;
}
