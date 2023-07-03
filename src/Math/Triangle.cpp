#include "Triangle.hpp"

Triangle &Triangle::copyPoints(const Triangle &o) {
    for (int i = 0; i < 3; ++i) {
        this->p[i] = o.p[i];
    }
    return *this;
}

Triangle &Triangle::copyTexture(const Triangle &o) {
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

Triangle Triangle::addPoints(const V3d &v) const {
    return {
        this->p[0] + v,
        this->p[1] + v,
        this->p[2] + v
    };
}

Triangle Triangle::multPoints(const M4x4 &m) const {
    return {
        m * this->p[0],
        m * this->p[1],
        m * this->p[2]
    };
}

