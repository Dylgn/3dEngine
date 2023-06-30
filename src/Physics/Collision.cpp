#include "Collision.hpp"

bool Collision::Colliding(Interval a[3], Interval b[3]) {
    for (int i = 0; i < 2; ++i) {
        if (a[i].intersects(b[i])) return true;
    }
    return false;
}