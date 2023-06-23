#include "V2d.hpp"

V2d V2d::operator+(V2d o) {
    return {
        this->u + o.u,
        this->v + o.v,
        this->w + o.w
    };
}

V2d V2d::operator-(V2d o) {
    return {
        this->u - o.u,
        this->v - o.v,
        this->w - o.w
    };
}

V2d V2d::operator*(float k) {
    return {
        this->u * k,
        this->v * k,
        this->w * k
    };
}

V2d V2d::operator/(float k) {
    return {
        this->u / k,
        this->v / k,
        this->w / k
    };
}

