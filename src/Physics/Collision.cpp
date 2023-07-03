#include "Collision.hpp"
#include "Simplex.hpp"
#include "MathUtility.hpp"

namespace {
    /** Gets diff of furthest points in opposite directions to get a vertex on hull of A - B*/
    V3d GreatestDiff(const Collider *a, const Collider *b, V3d dir) {
        return a->FurthestPointIn(dir) - b->FurthestPointIn(dir.opposite());
    }

    bool NSLine(Simplex &simplex, V3d &dir) {
        V3d a = simplex[0];
        V3d b = simplex[1];

        V3d ab = b - a;
        V3d ao = a.opposite();

        if (MathUtil::SameDirection(ab, ao)) dir = ab.crossProd(ao).crossProd(ab);
        else {
            simplex = {a};
            dir = ao;
        }
        return false;
    }

    bool NSTriangle(Simplex &simplex, V3d &dir) {
        V3d a = simplex[0];
        V3d b = simplex[1];
        V3d c = simplex[2];

        V3d ab = b - a;
        V3d ac = c - a;
        V3d ao = a.opposite();

        V3d abc = ab.crossProd(ac);

        if (MathUtil::SameDirection(abc.crossProd(ac), ao)) {
            if (MathUtil::SameDirection(ac, ao)) {
                simplex = {a, c};
                dir = ac.crossProd(ao).crossProd(ac);
            } else return NSLine(simplex = {a, b}, dir);
        } else if (MathUtil::SameDirection(ab.crossProd(abc), ao)) {
            return NSLine(simplex = {a, b}, dir);
        } else if (MathUtil::SameDirection(abc, ao)) {
            dir = abc;
        } else {
            simplex = {a, c, b};
            dir = abc.opposite();
        }

        return false;
    }

    bool NSTetrahedron(Simplex &simplex, V3d &dir) {
        V3d a = simplex[0];
        V3d b = simplex[1];
        V3d c = simplex[2];
        V3d d = simplex[3];

        V3d ab = b - a;
        V3d ac = c - a;
        V3d ad = d - a;
        V3d ao = a.opposite();

        V3d abc = ab.crossProd(ac);
        V3d acd = ac.crossProd(ad);
        V3d adb = ad.crossProd(ab);

        if (MathUtil::SameDirection(abc, ao)) {
            return NSTriangle(simplex = {a, b, c}, dir);
        } else if (MathUtil::SameDirection(acd, ao)) {
            return NSTriangle(simplex = {a, c, d}, dir);
        } else if (MathUtil::SameDirection(adb, ao)) {
            return NSTriangle(simplex = {a, d, b}, dir);
        } else return true;
    }

    bool NextSimplex(Simplex &simplex, V3d &dir) {
        switch (simplex.size()) {
            case 2: return NSLine(simplex, dir);
            case 3: return NSTriangle(simplex, dir);
            case 4: return NSTetrahedron(simplex, dir);
            default: return false; // Shouldn't happen
        }
    }
}

bool Collision::GJK(const Collider *a, const Collider *b) {
    // Initial support point (first direction is arbitrary)
    V3d sup = GreatestDiff(a, b, V3d::unit_x);

    // Create simplex to try and cover origin
    Simplex vertices;
    vertices.push_front(sup);

    // Dir towards origi
    V3d to_origin = sup.opposite();

    while (true) {
        sup = GreatestDiff(a, b, to_origin);

        // Current closest vertex is closest possible, no collision
        if (sup.dotProd(to_origin) <= 0.0f) return false;

        vertices.push_front(sup);

        if (NextSimplex(vertices, to_origin)) return true;
    }
}