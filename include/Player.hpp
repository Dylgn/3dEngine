#pragma once

#include "Object.hpp"
#include "Camera.hpp"

struct Player: public Object {
    Player() = default;
    Player(const M4x4 &mat_proj, const V3d &look_dir, const V3d &pos, const float &yaw, const float &pitch, const float &roll);
    virtual ~Player() = default;

    virtual void SetCameraPos(const V3d &loc);
    virtual Camera *GetCamera();
};