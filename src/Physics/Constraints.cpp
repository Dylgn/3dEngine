#include "Constraints.hpp"
#include "MathUtility.hpp"

#define GRAVITY -9.8

namespace {

V3d limitAngle(const V3d &n, const V3d &n1, const V3d &n2, float alpha, float beta) {
    float phi = std::asin(n1.cross(n2).dot(n));
    if (n1.dot(n2) < 0.0) phi = PI - phi;
    if (phi > PI) phi = phi - 2.0 * PI;
    if (phi < -PI) phi = phi + 2.0 * PI;
    if (phi < alpha || phi > beta) {
        phi = MathUtil::clamp(phi, alpha, beta);
        return Quaternion(n, phi).rotate(n1).cross(n2);
    }
    return V3d::origin;
}

}

namespace Constraints {

// Apply constraint to bodies
void Position::apply(const V3d &normal, float magnitude, float delta_lambda)
{
    V3d pos_impulse = normal * delta_lambda;

    entities[0]->pos += pos_impulse / entities[0]->mass;
    entities[1]->pos += pos_impulse / entities[1]->mass;

    // Constraint points relative to their centre of masses
    V3d r1 = *points[0] - entities[0]->pos;
    V3d r2 = *points[1] - entities[1]->pos;

    V3d irp1 = entities[0]->inv.inertia_tensor * r1.cross(pos_impulse);
    V3d irp2 = entities[1]->inv.inertia_tensor * r2.cross(pos_impulse);
    entities[0]->rot = entities[0]->rot + (Quaternion{irp1.x, irp1.y, irp1.z, 0} * entities[0]->rot) * 0.5f;
    entities[1]->rot = entities[1]->rot - (Quaternion{irp2.x, irp2.y, irp2.z, 0} * entities[1]->rot) * 0.5f;
}

float Position::getDeltaLambda(float h, const V3d &normal, float magnitude, float compliance, float lambda) const
{
    // Constraint points relative to their centre of masses
    V3d r1 = *points[0] - entities[0]->pos;
    V3d r2 = *points[1] - entities[1]->pos;

    // Calculate generalized inverse masses
    float w1 = getGeneralizedInverseMass(0, r1, normal, magnitude);
    float w2 = getGeneralizedInverseMass(1, r2, normal, magnitude);

    float alpha = compliance / (h * h);
    return (-magnitude - alpha * lambda) / (w1 + w2 + alpha);
}

float Position::getGeneralizedInverseMass(unsigned i, const V3d &r, const V3d &n, float c) const
{
    if (i != 0 || i != 1) return 0.0f;
    Entity *e = entities[i];
    
    // TODO : e->rot.inverse().rotate(r.cross(n)) ?
    V3d rn = r.cross(n);
    
    return (1.0 / e->mass) + rn.dot((e->inv.inertia_tensor * rn));
}

void Distance::solve(float h)
{
    // Correction direction (normal)
    V3d n = entities[0]->pos - entities[1]->pos;
    // Correction magnitude (delta between attachment distance and constraint distance)
    float c = n.length() - distance;
    
    n = n.normalize();

    // moved to Position::getDeltaLambda()
    float delta_lambda = getDeltaLambda(h, n, c, compliance, *lambda);
    *lambda += delta_lambda;

    // // Constraint points relative to their centre of masses
    // V3d r1 = *points[0] - entities[0]->pos;
    // V3d r2 = *points[1] - entities[1]->pos;

    // // Calculate generalized inverse masses
    // float w1 = getGeneralizedInverseMass(0, r1, n, c);
    // float w2 = getGeneralizedInverseMass(1, r2, n, c);

    // float alpha = compliance / (h * h);
    // float delta_lambda = (-c - alpha * *lambda) / (w1 + w2 + alpha);
    // *lambda += delta_lambda;

    // moved to Position::apply()
    apply(n, c, delta_lambda);   

    // V3d pos_impulse = n * delta_lambda;

    // // Apply constraint to bodies
    // entities[0]->pos += pos_impulse / entities[0]->mass;
    // entities[1]->pos += pos_impulse / entities[1]->mass;

    // V3d irp1 = entities[0]->inv.inertia_tensor * r1.cross(pos_impulse);
    // V3d irp2 = entities[1]->inv.inertia_tensor * r2.cross(pos_impulse);
    // entities[0]->rot = entities[0]->rot + (Quaternion{irp1.x, irp1.y, irp1.z, 0} * entities[0]->rot) * 0.5f;
    // entities[1]->rot = entities[1]->rot - (Quaternion{irp2.x, irp2.y, irp2.z, 0} * entities[1]->rot) * 0.5f;
}

void Collision::solve(float h)
{
    V3d r1 = *points[0] - entities[0]->pos;
    V3d r2 = *points[1] - entities[1]->pos;
    V3d p1 = entities[0]->pos + entities[0]->rot.rotate(r1);
    V3d p2 = entities[1]->pos + entities[1]->rot.rotate(r2);
    float cur_pen = (p1 - p2).dot(normal);

    if (cur_pen > 0.0) {
        V3d delta_x = normal * cur_pen;
        float delta_lambda = getDeltaLambda(h, delta_x.normalize(), delta_x.length(), 0, *lambda_n);

        apply(delta_x.normalize(), delta_x.length(), delta_lambda);
        *lambda_n += delta_lambda;

        V3d r1 = *points[0] - entities[0]->pos;
        V3d r2 = *points[1] - entities[1]->pos;
        V3d p1 = entities[0]->pos + entities[0]->rot.rotate(r1);
        V3d p2 = entities[1]->pos + entities[1]->rot.rotate(r2);

        delta_lambda = getDeltaLambda(h, delta_x.normalize(), delta_x.length(), 0, *lambda_t);
        *lambda_t += delta_lambda;

        // static friction coefficient
        float mu = (entities[0]->static_friction + entities[1]->static_friction) / 2.0;

        if (*lambda_t < mu * *lambda_n) {
            V3d p1_prev = entities[0]->prev.pos + entities[0]->prev.rot.rotate(r1);
            V3d p2_prev = entities[1]->prev.pos + entities[1]->prev.rot.rotate(r2);
            V3d delta_p = (p1 - p1_prev) - (p2 - p2_prev);
            V3d delta_p_t = delta_p - normal * (delta_p.dot(normal));

            apply(delta_p_t.normalize(), delta_p_t.length(), delta_lambda);
            *lambda_t += delta_lambda;
        }
    }
}

void Collision::velocitySolve(float h)
{
    V3d r1 = *points[0] - entities[0]->pos;
    V3d r2 = *points[1] - entities[1]->pos;
    V3d v = entities[0]->velocity + entities[0]->angular_velocity.cross(r1)
        - entities[1]->velocity + entities[1]->angular_velocity.cross(r2);
    float vn = normal.dot(v);
    V3d vt = v - normal * vn;

    // dynamic friction coefficient
    float mu = (entities[0]->dynamic_friction + entities[1]->dynamic_friction) / 2.0;
    // normal force
    float fn = *lambda_n / (h * h);

    V3d delta_v = -(vt / vt.length()) * std::min(h * mu * std::abs(fn), vt.length());

    float w1 = getGeneralizedInverseMass(0, r1, normal.normalize(), normal.length());
    float w2 = getGeneralizedInverseMass(1, r2, normal.normalize(), normal.length());

    V3d p = delta_v / (w1 + w2);

    // Restitution
    V3d v_prev = entities[0]->prev.velocity + entities[0]->prev.angular_velocity.cross(r1)
        - entities[1]->prev.velocity + entities[1]->prev.angular_velocity.cross(r2);
    float vn_prev = normal.dot(v_prev);

    float e = std::abs(vn) <= 2.0 * std::abs(GRAVITY) * h
        ? 0 : e = entities[0]->restitution * entities[1]->restitution;

    delta_v += normal * (-vn + std::min(-e * vn_prev, 0.0f));

    // Apply
    entities[0]->velocity += p / entities[0]->mass;
    entities[1]->velocity += p / entities[1]->mass;
    V3d irp1 = entities[0]->inv.inertia_tensor * r1.cross(p);
    V3d irp2 = entities[1]->inv.inertia_tensor * r2.cross(p);
    entities[0]->angular_velocity += irp1;
    entities[1]->angular_velocity += irp2;
}

void Angular::apply(float h, const V3d &delta_q, float &lambda)
{
    V3d n = delta_q.normalize();
    float theta = delta_q.length();

    float w1 = n.dot((entities[0]->inv.inertia_tensor * n));
    float w2 = n.dot((entities[1]->inv.inertia_tensor * n));

    float alpha = compliance / (h * h);
    float delta_lambda = (-theta - alpha * lambda) / (w1 + w2 + alpha);
    lambda += delta_lambda;

    V3d pos_impulse = n * -delta_lambda; // TODO : negative?

    V3d ip1 = entities[0]->inv.inertia_tensor * pos_impulse;
    V3d ip2 = entities[1]->inv.inertia_tensor * pos_impulse;
    entities[0]->rot = entities[0]->rot + (Quaternion{ip1.x, ip1.y, ip1.z, 0} * entities[0]->rot) * 0.5f;
    entities[1]->rot = entities[1]->rot - (Quaternion{ip2.x, ip2.y, ip2.z, 0} * entities[1]->rot) * 0.5f;
}

void Mutual::solve(float h)
{
    Quaternion q = entities[0]->rot * entities[1]->rot.inverse();
    V3d delta_q = V3d{q.x, q.y, q.z} * 2.0f;

    apply(h, delta_q, lambda_pos);
}

void Hinge::solve(float h)
{
    // Transform into world space and allign with each other
    V3d delta_q = entities[0]->rot.rotate(*align_axes[0]).cross(
        entities[1]->rot.rotate(*align_axes[1])
    );

    apply(h, delta_q, *lambda_align);

    // Positional Degrees of Freedom
    Constraints::Distance dist_constraint{};
    dist_constraint.distance = 0; // Typical case for joints is no separation
    dist_constraint.compliance = this->compliance;
    dist_constraint.lambda = &this->lambda_pos;
    dist_constraint.entity = this->entity;
    dist_constraint.point = this->point;
    dist_constraint.solve(h);

    // Joint angle limit (Rotational Degrees of Freedomo)
    V3d n = entities[0]->rot.rotate(*align_axes[0]).normalize(); // TODO normalize necessary?
    V3d n1 = entities[0]->rot.rotate(*limit_axes[0]).normalize();
    V3d n2 = entities[1]->rot.rotate(*limit_axes[1]).normalize();
    delta_q = limitAngle(n, n1, n2, lower_limit, upper_limit);
    if (delta_q) apply(h, delta_q, *lambda_limit);
}

void Spherical::solve(float h)
{
    // Positional Degrees of Freedom
    Constraints::Distance dist_constraint{};
    dist_constraint.distance = 0; // Typical case for joints is no separation
    dist_constraint.compliance = this->compliance;
    dist_constraint.lambda = &this->lambda_pos;
    dist_constraint.entity = this->entity;
    dist_constraint.point = this->point;
    dist_constraint.solve(h);

    // Swing limit
    V3d n1 = entities[0]->rot.rotate(*swing_axes[0]).normalize();
    V3d n2 = entities[1]->rot.rotate(*swing_axes[1]).normalize();
    V3d n = n1.cross(n2).normalize();
    V3d delta_q = limitAngle(n, n1, n2, swing_limtis.lower_limit, swing_limtis.upper_limit);
    if (delta_q) apply(h, delta_q, *lambda_swing);

    // Twist limit
    V3d a1 = entities[0]->rot.rotate(*swing_axes[0]).normalize();
    V3d b1 = entities[0]->rot.rotate(*twist_axes[0]).normalize();
    V3d a2 = entities[1]->rot.rotate(*swing_axes[1]).normalize();
    V3d b2 = entities[1]->rot.rotate(*twist_axes[1]).normalize();
    V3d n = (a1 + a2).normalize();
    V3d n1 = (b1 - n * b1.dot(n)).normalize();
    V3d n2 = (b2 - n * b2.dot(n)).normalize();
    V3d delta_q = limitAngle(n, n1, n2, twist_limits.lower_limit, twist_limits.upper_limit);
    if (delta_q) apply(h, delta_q, *lambda_twist);
}

};