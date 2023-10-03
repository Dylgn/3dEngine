#include "Player.hpp"

Player::Player(): Object{} {}

Player::Player(const M4x4 &mat_proj, const V3d &look_dir, const V3d &pos, const float &yaw, const float &pitch, const float &roll) : Object{} {
    this->transform = new Camera{mat_proj, look_dir, pos, yaw, pitch, roll};
}

Player::~Player() {}

Player &Player::operator=(Player &&o) {
    Object::operator=(std::move(o));

    return *this;
}

void Player::SetCameraPos(const V3d &loc)
{
    transform->pos = loc;
}

Camera *Player::GetCamera() {
    return static_cast<Camera*>(transform);
}
