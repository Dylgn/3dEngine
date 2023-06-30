#include <algorithm>
#include "Interval.hpp"

Interval::Interval(float s, float e): low{s}, high{e} {
    if (high <= low) std::swap(low, high);
}

float Interval::getLow() { return low; }
float Interval::getHigh() { return high; }

bool Interval::in(float f) {
    return (low <= f) && (f <= high);
}

bool Interval::intersects(Interval o) {
    return in(o.getLow()) || in(o.getHigh()) || o.in(low) || o.in(high);
}

void Interval::moveInterval(float k) {
    low += k;
    high += k;
}
