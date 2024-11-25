#include "Dynamics.hpp"
#include "Constraints.hpp"

void SimulatePhysics(float dt, std::vector<Entity*> &entities, std::vector<Constraint*> constraints, int pos_iters, int substeps)
{
    if (dt <= 0.0f) return;

    float h = dt / static_cast<float>(substeps);

    // TODO : GET COLLISION PAIRS

    for (int i = 0; i < substeps; ++i) {
        for (int j = 0; j < entities.size(); ++j) {
            Entity *e = entities[j];

            V3d ext_force; // TODO : CALCULATE EXTERNAL FORCE

            e->prev.pos = e->pos;
            e->velocity += ext_force * h / e->mass;
            e->pos += e->velocity * h;

            V3d ext_torque; // TODO : CALCULATE EXTERNAL TORQUE

            e->prev.rot = e->rot;
            e->angular_velocity += e->inv.inertia_tensor * 
                (ext_torque - (e->angular_velocity.cross(e->inertia_tensor * e->angular_velocity))) * h;
            e->rot = Quaternion{
                e->rot.x + h * 0.5f * e->angular_velocity.x * e->rot.x,
                e->rot.y + h * 0.5f * e->angular_velocity.y * e->rot.y,
                e->rot.z + h * 0.5f * e->angular_velocity.z * e->rot.z,
                e->rot.w
            };

            e->rot = e->rot.normalize();
        }

        for (unsigned j = 0; j < pos_iters; ++j) {
            for (unsigned k = 0; k < constraints.size(); ++k) {
                constraints[k]->solve(h);
            }
        }

        for (int j = 0; j < entities.size(); ++j) {
            Entity *e = entities[j];

            e->velocity = (e->pos - e->prev.pos) / h;
            Quaternion d_rot = e->rot * -e->prev.rot;
            e->angular_velocity = V3d{d_rot.x, d_rot.y, d_rot.z} * 2 / h;
            if (d_rot.w < 0) e->angular_velocity = -e->angular_velocity;
        }
    }
}
