#include <cmath>
#include "CollideNarrow.hpp"

#define FLT_MAX __FLT_MAX__

namespace {
    float TransformToAxis(const Box &box, const V3d &axis) {
        return box.half_size.x * fabsf(axis.dot(box.getAxis(0))) +
               box.half_size.y * fabsf(axis.dot(box.getAxis(1))) +
               box.half_size.z * fabsf(axis.dot(box.getAxis(2)));
    }

    /** Checks if boxes overlap.
     * @param to_center vector between boxes' center points, to avoid recalculation
    */
    float penetrationOnAxis(const Box &one, const Box &two, const V3d &axis, const V3d &to_center) {
        // Project half-size onto axis
        float one_project = TransformToAxis(one, axis);
        float two_project = TransformToAxis(two, axis);

        // Project onto axis
        float dist = fabsf(to_center.dot(axis));

        // Return overlap (positive indicates overlap, negative indicates separation)
        return one_project + two_project - dist;
    }

    bool tryAxis(const Box& one, const Box& two, V3d axis, const V3d& to_centre, 
                 unsigned index, float& smallest_pen, unsigned& smallest_case) {
        if (axis.squareLength() < 0.0001f) return true;
        axis = axis.normalize();

        float pen = penetrationOnAxis(one, two, axis, to_centre);

        if (pen < 0) return false;
        else if (pen < smallest_pen) {
            smallest_pen = pen;
            smallest_case = index;
        }
        return true;
    }

    void fillPointFaceBoxBox(const Box &one, const Box &two, const V3d &to_centre, 
                             CollisionData *data, unsigned best, float pen) {
        Contact *contact = data->contacts;

        // Collision on axis 'best', need to find the two faces on this axis
        V3d normal = one.getAxis(best);
        if (one.getAxis(best).dot(to_centre) > 0) {
            normal *= 1.0f;
        }

        // Find vertex of 'two' we're colliding with
        V3d vertex = two.half_size;
        if (two.getAxis(0).dot(normal) < 0) vertex.x = -vertex.x;
        if (two.getAxis(1).dot(normal) < 0) vertex.y = -vertex.y;
        if (two.getAxis(2).dot(normal) < 0) vertex.z = -vertex.z;

        contact->normal = normal;
        contact->penetration = pen;
        contact->point = two.transform * vertex;
        contact->SetBodyData(one.body, two.body, data->friction, data->restitution);
    }

    V3d contactPoint(const V3d &p_one, const V3d &d_one, float one_size, 
                     const V3d &p_two, const V3d &d_two, float two_size, bool use_midpoint_one) {
        V3d to_st, c_one, c_two;
        float dp_st_a_one, dp_st_a_two, dp_one_two, sm_one, sm_two;
        float denom, mua, mub;

        sm_one = d_one.squareLength();
        sm_two = d_two.squareLength();
        dp_one_two = d_two.dot(d_one);

        to_st = p_one - p_two;
        dp_st_a_one = d_one.dot(to_st);
        dp_st_a_two = d_two.dot(to_st);

        denom = sm_one * sm_two - dp_one_two * dp_one_two;

        // Zero denominator indicates parallel lines
        if (fabs(denom) < 0.0001f) {
            return use_midpoint_one ? p_one : p_two;
        }

        mua = (dp_one_two * dp_st_a_two - sm_two * dp_st_a_one) / denom;
        mub = (sm_one * dp_st_a_two - dp_one_two * dp_st_a_one) / denom;

        if (mua > one_size || mua < -one_size || mub > two_size || mub < -two_size) {
            return use_midpoint_one ? p_one : p_two;
        } else {
            c_one = p_one + d_one * mua;
            c_two = p_two + d_two * mub;

            return c_one * 0.5f + c_two * 0.5f;
        }
    }
}

V3d Primitive::getAxis(unsigned index) const {
    return transform.GetColumnVector(index);
}

unsigned CollisionDetector::sphereSphere(const Sphere &one, const Sphere &two, CollisionData *data) {
    if (data->free_slots <= 0) return 0;

    V3d pos_one = one.getAxis(3);
    V3d pos_two = two.getAxis(3);

    V3d mid_line = pos_one - pos_two;
    float size = mid_line.length();

    if (size <= 0.0f || size >= one.radius + two.radius) return 0;

    Contact *contact = data->contacts;
    contact->normal = mid_line * (1.0f / size);
    contact->penetration = one.radius + two.radius - size;
    contact->SetBodyData(one.body, two.body, data->friction, data->restitution);
    return 1;
}

unsigned CollisionDetector::sphereHalfSpace(const Sphere &sphere, const Plane &plane, CollisionData *data) {
    if (data->free_slots <= 0) return 0;

    V3d pos = sphere.getAxis(3);

    float ball_dist = plane.normal.dot(pos) - sphere.radius - plane.offset;

    if (ball_dist >= 0) return 0;

    // Normal in same direction as plane's normal
    Contact *contact = data->contacts;
    contact->normal = plane.normal;
    contact->penetration = -ball_dist;
    contact->point = pos - plane.normal * (ball_dist + sphere.radius);
    contact->SetBodyData(sphere.body, nullptr, data->friction, data->restitution);

    data->AddContacts(1);
    return 1;
}

unsigned CollisionDetector::spherePlane(const Sphere &sphere, const Plane &plane, CollisionData *data) {
    if (data->free_slots <= 0) return 0;

    V3d pos = sphere.getAxis(3);

    // Get dist from plane
    float center_dist = plane.normal.dot(pos) - plane.offset;

    if (center_dist * center_dist > sphere.radius * sphere.radius) return 0;

    // Check which side of plane we're ono
    V3d normal = plane.normal;
    float penetration = -center_dist;
    if (center_dist < 0) {
        normal *= -1;
        penetration = -penetration;
    }
    penetration += sphere.radius;

    // Normal in same direction as plane's normal
    Contact *contact = data->contacts;
    contact->normal = normal;
    contact->penetration = penetration;
    contact->point = pos - plane.normal * center_dist;
    contact->SetBodyData(sphere.body, nullptr, data->friction, data->restitution);

    data->AddContacts(1);
    return 1;
}

unsigned CollisionDetector::boxHalfSpace(const Box &box, const Plane &plane, CollisionData *data) {
    if (data->free_slots <= 0) return 0;

    // First, check for collision
    float projected_radius = TransformToAxis(box, plane.normal);
    // How far box is from origin
    float box_dist = plane.normal.dot(box.getAxis(3)) - projected_radius;
    if (box_dist > plane.offset) return 0;

    // Otherwise, we have collision. Find intersection points by checking vertices

    // Each combination of +/- for each half-size
    static float mults[8][3] = {{1,1,1}, {-1,1,1},{1,-1,1},{-1,-1,1},
                                {1,1,-1},{-1,1,-1},{1,-1,-1},{-1,-1,-1}};

    Contact *contact = data->contacts;
    unsigned contacts_used = 0;
    for (unsigned i = 0; i < 8; ++i) {
        // Calculate pos of each vertex
        V3d pos{mults[i][0], mults[i][1], mults[i][2]};
        pos.x *= box.half_size.x;
        pos.y *= box.half_size.y;
        pos.z *= box.half_size.z;
        pos = box.transform.Transform(pos);

        // Distance from plane
        float dist = pos.dot(plane.normal);

        // Create contact data
        if (dist <= plane.offset) {
            // Contact pooint halfway between vertex and plane
            contact->point = plane.normal;
            contact->point *= (dist - plane.offset);
            contact->point += pos;
            contact->normal = plane.normal;
            contact->penetration = plane.offset - dist;

            contact->SetBodyData(box.body, nullptr, data->friction, data->restitution);

            ++contact;
            ++contacts_used;
            if (contacts_used == data->free_slots) return contacts_used;
        }
    }

    data->AddContacts(contacts_used);
    return contacts_used;
}

unsigned CollisionDetector::boxSphere(const Box &box, const Sphere &sphere, CollisionData *data) {
    // Transform center oof sphere into box coords
    V3d center = sphere.getAxis(3);
    V3d rel_center = box.transform.TransformInverse(center);

    if (fabsf(rel_center.x) - sphere.radius > box.half_size.x ||
        fabsf(rel_center.y) - sphere.radius > box.half_size.y ||
        fabsf(rel_center.z) - sphere.radius > box.half_size.z) 
    return 0;

    V3d closest{0,0,0};
    float dist;

    // Clamp each coord to box
    dist = rel_center.x;
    if (dist > box.half_size.x) dist = box.half_size.x;
    if (dist < -box.half_size.x) dist = -box.half_size.x;
    closest.x = dist;

    dist = rel_center.y;
    if (dist > box.half_size.y) dist = box.half_size.y;
    if (dist < -box.half_size.y) dist = -box.half_size.y;
    closest.y = dist;

    dist = rel_center.z;
    if (dist > box.half_size.z) dist = box.half_size.z;
    if (dist < - box.half_size.z) dist = -box.half_size.z;
    closest.z = dist;

    // Check if in contact
    dist = (closest - rel_center).squareLength();
    if (dist > sphere.radius * sphere.radius) return 0;

    // Compile contact
    V3d closest_world = box.transform.Transform(closest);

    Contact *contact = data->contacts;
    contact->normal = closest_world - center;
    contact->normal = contact->normal.normalize();
    contact->point = closest_world;
    contact->penetration = sphere.radius - std::sqrt(dist);
    contact->SetBodyData(box.body, sphere.body, data->friction, data->restitution);

    data->AddContacts(1);
    return 1;
}

unsigned CollisionDetector::boxBox(const Box &one, const Box &two, CollisionData *data) {
    // Vector between both centres
    V3d to_centre = two.getAxis(3) - one.getAxis(3);

    float pen = FLT_MAX;
    unsigned best = 0xffffff;

    auto checkOverlap = [&one, &two, &to_centre, &pen, &best] (V3d axis, unsigned index) {
        if (!tryAxis(one, two, axis, to_centre, index, pen, best)) return 0;
    };

    // Check each axes, returning if it gives us a separating axis
    // Keeps track of axis with smallest penetration otherwise
    checkOverlap(one.getAxis(0), 0);
    checkOverlap(one.getAxis(1), 1);
    checkOverlap(one.getAxis(2), 2);

    checkOverlap(two.getAxis(0), 3);
    checkOverlap(two.getAxis(1), 4);
    checkOverlap(two.getAxis(2), 5);

    unsigned best_single_axis = best;

    checkOverlap(one.getAxis(0).cross(two.getAxis(0)), 6);
    checkOverlap(one.getAxis(0).cross(two.getAxis(1)), 7);
    checkOverlap(one.getAxis(0).cross(two.getAxis(2)), 8);
    checkOverlap(one.getAxis(1).cross(two.getAxis(0)), 9);
    checkOverlap(one.getAxis(1).cross(two.getAxis(1)), 10);
    checkOverlap(one.getAxis(1).cross(two.getAxis(2)), 11);
    checkOverlap(one.getAxis(2).cross(two.getAxis(0)), 12);
    checkOverlap(one.getAxis(2).cross(two.getAxis(1)), 13);
    checkOverlap(one.getAxis(2).cross(two.getAxis(2)), 14);

    if (best == 0xffffff) return 0;

    if (best < 3) {
        fillPointFaceBoxBox(one, two, to_centre, data, best, pen);
        data->AddContacts(1);
        return 1;
    } else if (best < 6) {
        fillPointFaceBoxBox(two, one, to_centre * -1.0f, data, best - 3, pen);
        data->AddContacts(1);
        return 1;
    } else {
        // Edge-edge, find axis
        best -= 6;
        unsigned one_axis_index = best / 3;
        unsigned two_axis_index = best % 3;
        V3d one_axis = one.getAxis(one_axis_index);
        V3d two_axis = two.getAxis(two_axis_index);
        V3d axis = one_axis.cross(two_axis).normalize();

        // Make axis point from one to two
        if (axis.dot(to_centre) > 0) axis = axis * -1.0f;

        V3d one_edge_pt = one.half_size;
        V3d two_edge_pt = two.half_size;
        for (unsigned i = 0; i < 3; ++i) {
            if (i == one_axis_index) one_edge_pt[i] = 0;
            else if (one.getAxis(i).dot(axis) > 0) one_edge_pt[i] = -one_edge_pt[i];

            if (i == two_axis_index) two_edge_pt[i] = 0;
            else if (two.getAxis(i).dot(axis) < 0) two_edge_pt[i] = -two_edge_pt[i];
        }

        // Convert to world coords
        one_edge_pt = one.transform * one_edge_pt;
        two_edge_pt = two.transform * two_edge_pt;

        // Need to find point of closest approach of the two line-segments
        V3d vertex = contactPoint(one_edge_pt, one_axis, one.half_size[one_axis_index],
                                  two_edge_pt, two_axis, two.half_size[two_axis_index], best_single_axis > 2);

        // Fill contact
        Contact *contact = data->contacts;

        contact->penetration = pen;
        contact->normal = axis;
        contact->point = vertex;
        contact->SetBodyData(one.body, two.body, data->friction, data->restitution);
        data->AddContacts(1);
        return 1;
    }
    return 0;
}

void CollisionData::AddContacts(unsigned count) {
    free_slots -= count;
    // Move array forward to next free slot
    contacts += count;
}
