#include "Triangle.hpp"

Triangle &Triangle::copyPoints(Triangle o) {
    for (int i = 0; i < 3; ++i) {
        this->p[i] = o.p[i];
    }
    return *this;
}

Triangle &Triangle::copyTexture(Triangle o) {
    for (int i = 0; i < 3; ++i) {
        this->t[i] = o.t[i];
    }
    return *this;
}

Triangle &Triangle::normalizePoints() {
    for (int i = 0; i < 3; ++i) {
        this->p[i] = this->p[i] / this->p[i].w;
    }
    return *this;
}

Triangle Triangle::addPoints(V3d v) {
    return {
        this->p[0] + v,
        this->p[1] + v,
        this->p[2] + v
    };
}

Triangle Triangle::multPoints(M4x4 m) {
    return {
        m * this->p[0],
        m * this->p[1],
        m * this->p[2]
    };
}

