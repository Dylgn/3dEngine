#include "Player.hpp"

Player::Player(const M4x4 &mat_proj, const V3d &look_dir, const V3d &pos, const float &yaw, const float &pitch, const float &roll) {
    this->transform = new Camera{mat_proj, look_dir, pos, yaw, pitch, roll};
}

void Player::SetCameraPos(const V3d &loc) {
    transform->pos = loc;
}

Camera *Player::GetCamera() {
    return static_cast<Camera*>(transform);
}
