#pragma once

#include "V3d.hpp"
#include "M4x4.hpp"
#include "Transform.hpp"

struct Camera: public Transform {
    M4x4 mat_proj;
    V3d look_dir;

    Camera() = default;
    Camera(const M4x4 &mat_proj, const V3d &look_dir, const Transform &transform): mat_proj{mat_proj}, look_dir{look_dir}, Transform{transform} {}
    Camera(const M4x4 &mat_proj, const V3d &look_dir, const V3d &pos, const float &yaw, const float &pitch, const float &roll): 
        mat_proj{mat_proj}, look_dir{look_dir}, Transform{pos, yaw, pitch, roll} {}
};