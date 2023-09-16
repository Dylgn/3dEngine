#include <cmath>
#include "Rigidbody.hpp"
#include "MathUtility.hpp"

/** Creates a transform matrix from a vector (pos) and quaternion (orientation) */
static inline void CalculateTransformMatrix(M3x4 &transform, const V3d &pos, const Quaternion &orientation) {
    transform.data[0] = 1 - 2 * orientation.j * orientation.j - 2 * orientation.k * orientation.k;
    transform.data[1] = 2 * orientation.i * orientation.j - 2 * orientation.r * orientation.k;
    transform.data[2] = 2 * orientation.i * orientation.k + 2 * orientation.r * orientation.j;
    transform.data[3] = pos.x;
    transform.data[4] = 2 * orientation.i * orientation.j + 2 * orientation.r * orientation.k;
    transform.data[5] = 1 - 2 * orientation.i * orientation.i - 2 * orientation.k * orientation.k;
    transform.data[6] = 2 * orientation.j * orientation.k - 2 * orientation.r * orientation.i;
    transform.data[7] = pos.y;
    transform.data[8] = 2 * orientation.i * orientation.k - 2 * orientation.r * orientation.j;
    transform.data[9] = 2 * orientation.j * orientation.k + 2 * orientation.r * orientation.i;
    transform.data[10] = 1 - 2 * orientation.i * orientation.i - 2 * orientation.j * orientation.j;
    transform.data[11] = pos.z;
}

/** Inertia tensor transform by quaternion */
static inline void TransformInertiaTensor(M3x3 &iit_world, const Quaternion &q, const M3x3 &iit_body, const M3x4 &rot_mat) {
    float t4 = rot_mat.data[0] * iit_body.data[0] + rot_mat.data[1] * iit_body.data[3] + rot_mat.data[2] * iit_body.data[6];
    float t9 = rot_mat.data[0] * iit_body.data[1] + rot_mat.data[1] * iit_body.data[4] + rot_mat.data[2] * iit_body.data[7];
    float t14 = rot_mat.data[0] * iit_body.data[2] + rot_mat.data[1] * iit_body.data[5] + rot_mat.data[2] * iit_body.data[8];
    float t28 = rot_mat.data[4] * iit_body.data[0] + rot_mat.data[5] * iit_body.data[3] + rot_mat.data[6] * iit_body.data[6];
    float t33 = rot_mat.data[4] * iit_body.data[1] + rot_mat.data[5] * iit_body.data[4] + rot_mat.data[6] * iit_body.data[7];
    float t38 = rot_mat.data[4] * iit_body.data[2] + rot_mat.data[5] * iit_body.data[5] + rot_mat.data[6] * iit_body.data[8];
    float t52 = rot_mat.data[8] * iit_body.data[0] + rot_mat.data[9] * iit_body.data[3] + rot_mat.data[10] * iit_body.data[6];
    float t57 = rot_mat.data[8] * iit_body.data[1] + rot_mat.data[9] * iit_body.data[4] + rot_mat.data[10] * iit_body.data[7];
    float t62 = rot_mat.data[8] * iit_body.data[2] + rot_mat.data[9] * iit_body.data[5] + rot_mat.data[10] * iit_body.data[8];

    iit_world.data[0] = t4 * rot_mat.data[0] + t9 * rot_mat.data[1] + t14 * rot_mat.data[2];
    iit_world.data[1] = t4 * rot_mat.data[4] + t9 * rot_mat.data[5] + t14 * rot_mat.data[6];
    iit_world.data[2] = t4 * rot_mat.data[8] + t9 * rot_mat.data[9] + t14 * rot_mat.data[10];
    iit_world.data[3] = t28 * rot_mat.data[0] + t33 * rot_mat.data[1]+ t38 * rot_mat.data[2];
    iit_world.data[4] = t28 * rot_mat.data[4] + t33 * rot_mat.data[5] + t38 * rot_mat.data[6];
    iit_world.data[5] = t28 * rot_mat.data[8] + t33 * rot_mat.data[9] + t38 * rot_mat.data[10];
    iit_world.data[6] = t52 * rot_mat.data[0] + t57 * rot_mat.data[1] + t62 * rot_mat.data[2];
    iit_world.data[7] = t52 * rot_mat.data[4] + t57 * rot_mat.data[5] + t62 * rot_mat.data[6];
    iit_world.data[8] = t52 * rot_mat.data[8] + t57 * rot_mat.data[9] + t62 * rot_mat.data[10];
}

void Rigidbody::CalculateDerivedData() {
    orientation.Normalize();

    CalculateTransformMatrix(trnasform_matrix, pos, orientation);

    TransformInertiaTensor(inv_inertia_tensor_world, orientation, inv_inertia_tensor, trnasform_matrix);
}

void Rigidbody::SetInertiaTensoor(const M3x3 &inertia_tensor) {
    inv_inertia_tensor.SetAsInverseOf(inertia_tensor);
}

void Rigidbody::ApplyForce(const V3d &force) {
    this->force += force;
    is_awake = true;
}

void Rigidbody::ClearAccumulators() {
    force = V3d::origin;
    torque = V3d::origin;
}

void Rigidbody::ApplyForceAtPoint(const V3d &force, const V3d &point) {
    V3d relative = point - pos;

    this->force += force;
    this->torque += relative % force;

    is_awake = true;
}

void Rigidbody::ApplyForceAtBodyPoint(const V3d &force, const V3d &point) {
    ApplyForceAtPoint(force, GetPointInWorldSpace(point));
}

V3d Rigidbody::GetPointInLocalSpace(const V3d &v) const {
    return trnasform_matrix.TransformInverse(v);
}

V3d Rigidbody::GetPointInWorldSpace(const V3d &v) const {
    return trnasform_matrix.Transform(v);
}

void Rigidbody::Integrate(float elapsed_time) {
    // Linear acceleration from applied force
    prev_accel = acceleration;
    prev_accel += MathUtil::AdjustToLength(force, inv_mass);
    // Angular acceleration from applied torque
    V3d angular_accel = inv_inertia_tensor_world * torque;

    // Update velocities from accel/impulse
    velocity += MathUtil::AdjustToLength(prev_accel, elapsed_time);
    rotation += MathUtil::AdjustToLength(angular_accel, elapsed_time);

    // Impulse drag
    velocity = velocity * powf(linear_damping, elapsed_time);
    rotation = rotation * powf(angular_damping, elapsed_time);

    // Adjust positions
    pos += MathUtil::AdjustToLength(velocity, elapsed_time);
    orientation.AddScaledVector(rotation, elapsed_time);

    CalculateDerivedData();
    ClearAccumulators();
}

V3d Rigidbody::GetVelocity() const {
    return velocity;
}

M3x4 Rigidbody::GetTransform() const {
    return trnasform_matrix;
}
