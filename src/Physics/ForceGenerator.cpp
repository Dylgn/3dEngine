#include "ForceGenerator.hpp"

void ForceRegistry::Add(Rigidbody *b, ForceGenerator *fg) {
    ForceRegistration reg;
    reg.b = b;
    reg.fg = fg;
    registrations.push_back(reg);
}

void ForceRegistry::Remove(Rigidbody *b, ForceGenerator *fg) {
    for (Registry::iterator it = registrations.begin(); it != registrations.end(); ++it) {
        if (it->b == b && it->fg == fg) {
            registrations.erase(it);
            return;
        }
    }
}

void ForceRegistry::Clear() {
    registrations.clear();
}

void ForceRegistry::Update(float elapsed_time) {
    for (ForceRegistration &fr : registrations) {
        fr.fg->UpdateForce(fr.b, elapsed_time);
    }
}

Aerodynamics::Aerodynamics(const M3x3 &tensor, const V3d &pos, const V3d *wind_speed): tensor{tensor}, pos{pos}, wind_speed{wind_speed} {}

void Aerodynamics::UpdateForce(Rigidbody *b, float elapsed_time) {
    UpdateForceFromTensor(b, elapsed_time, tensor);
}

void Aerodynamics::UpdateForceFromTensor(Rigidbody *b, float elapsed_time, const M3x3 &tensor) {
    // Calculate total velocity
    V3d velo = b->GetVelocity();
    velo += *wind_speed;

    // Calculate velocity in body coords
    V3d b_vel = b->GetTransform().TrasnformInverseDirection(velo);

    // Calculate force in body coords
    V3d b_force = tensor * b_vel;
    V3d force = b->GetTransform().TransformDirection(b_force);

    // Apply force
    b->ApplyForceAtBodyPoint(force, pos);
}

M3x3 AeroControl::GetTensor() {
    if (control <= -1.0f) return min_tensor;
    else if (control >= 1.0f) return max_tensor;
    else if (control < 0) {
        M3x3::Lerp(min_tensor, tensor, control + 1.0f);
    } else if (control > 0) {
        M3x3::Lerp(tensor, max_tensor, control);
    } else return tensor;
    return tensor;
}

AeroControl::AeroControl(const M3x3 &base, const M3x3 &min, const M3x3 &max, const V3d &pos, const V3d *wind_speed):
    Aerodynamics(base, pos, wind_speed), min_tensor{min}, max_tensor{max}, control{0.0f}  {}

void AeroControl::SetControl(float value) {
    control = value;
}

void AeroControl::UpdateForce(Rigidbody *b, float elapsed_time) {
    M3x3 tensor = GetTensor();
    UpdateForceFromTensor(b, elapsed_time, tensor);
}

Buoyancy::Buoyancy(const V3d &centre_buoyancy, float max_depth, float volume, float water_height, float liquid_density):
    centre_buoyancy{centre_buoyancy}, liquid_density{liquid_density}, max_depth{max_depth}, volume{volume}, water_height{water_height} {}

void Buoyancy::UpdateForce(Rigidbody *b, float elapsed_time) {
    // Calculate submersion depth
    V3d point_in_world = b->GetPointInWorldSpace(centre_buoyancy);
    float depth = point_in_world.y;

    // Check if out of water
    if (depth >= water_height + max_depth) return;
    V3d force = V3d::origin;

    // Check if max depth
    if (depth <= water_height - max_depth) {
        force.y = liquid_density * volume;
        b->ApplyForceAtBodyPoint(force, centre_buoyancy);
        return;
    }

    // Partily submerged
    force.y = liquid_density * volume * (depth - max_depth - water_height) / 2 * max_depth;
    b->ApplyForceAtBodyPoint(force, centre_buoyancy);
}
