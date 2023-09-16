#pragma once

#include "V3d.hpp"
#include "Quaternion.hpp"
#include "M3x4.hpp"
#include "M3x3.hpp"

class Rigidbody {
    protected:
        float inv_mass; // Inverse mass of rigid body
        float linear_damping; // Damping applied to linear motion
        float angular_damping; // Damping applied to angular motion
        V3d pos;
        Quaternion orientation; // Angular orientation of rigid body
        V3d velocity; // Linear velocity of rigid body in world space
        V3d rotation; // Angular velocity (rotation) of rigid body in world space
        M3x4 trnasform_matrix; // Matrix for converting body space into world space (and vice versa)
        M3x3 inv_inertia_tensor; // Inverse of body's inertia tensor
        M3x3 inv_inertia_tensor_world; // Inverse of body's inertia tensor in world space
        V3d force; // Forces acting on the body
        V3d torque; // Torque acting on the body
        V3d acceleration; // Acceleration of the body
        V3d prev_accel; // Acceleration of the body during the last frame
        bool is_awake; // Sleeping avoids use by integrations functions and collision detection

    public:
        /** Calculates internal data from state data. Should be called after body's state is altered directly. */
        void CalculateDerivedData();

        void SetInertiaTensoor(const M3x3 &inertia_tensor);

        void ApplyForce(const V3d &force);

        /** Clears accumulators for force and torque */
        void ClearAccumulators();

        /** Apply force at given point in world space */
        void ApplyForceAtPoint(const V3d &force, const V3d &point);

        /** Apply force at given point in body space*/
        void ApplyForceAtBodyPoint(const V3d &force, const V3d &point);

        /** Converts point from world space to local space */
        V3d GetPointInLocalSpace(const V3d &v) const;

        /** Converts point from local space to world space */
        V3d GetPointInWorldSpace(const V3d &v) const;

        void Integrate(float elapsed_time);

        V3d GetVelocity() const;

        M3x4 GetTransform() const;
};