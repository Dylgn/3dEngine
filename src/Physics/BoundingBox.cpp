#include "BoundingBox.hpp"

BoundingBox::BoundingBox(): x{}, y{}, z{} {}

BoundingBox::BoundingBox(Interval x, Interval y, Interval z): x{x}, y{y}, z{z} {}

BoundingBox::BoundingBox(Interval bounds[3]): x{bounds[0]}, y{bounds[1]}, z{bounds[2]} {}

bool BoundingBox::Colliding(BoundingBox &b) {
    return x.intersects(b.x) && z.intersects(b.z) && y.intersects(b.y);
}

float BoundingBox::getCentreX() {
    return (x.getLow() + x.getHigh()) / 2;
}

float BoundingBox::getCentreY() {
    return (y.getLow() + y.getHigh()) / 2;
}

float BoundingBox::getCentreZ() {
    return (z.getLow() + z.getHigh()) / 2;
}

void BoundingBox::move(V3d dir) {
    x.moveInterval(dir.x);
    y.moveInterval(dir.y);
    z.moveInterval(dir.z);
}
