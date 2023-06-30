#include "Object.hpp"

bool Object::Colliding(Object &o) {
    return box.Colliding(o.box);
}