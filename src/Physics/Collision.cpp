#include <algorithm>
#include <vector>
#include "Collision.hpp"
#include "MathUtility.hpp"

#define FLT_MAX __FLT_MAX__

typedef std::size_t size_t;

namespace {
    //
    // GJK 
    //
    bool NSLine(Simplex &simplex, V3d &dir) {
        V3d a = simplex[0];
        V3d b = simplex[1];

        V3d ab = b - a;
        V3d ao = -a;

        if (MathUtil::SameDirection(ab, ao)) dir = ab.cross(ao).cross(ab);
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
        V3d ao = -a;

        V3d abc = ab.cross(ac);

        if (MathUtil::SameDirection(abc.cross(ac), ao)) {
            if (MathUtil::SameDirection(ac, ao)) {
                simplex = {a, c};
                dir = ac.cross(ao).cross(ac);
            } else return NSLine(simplex = {a, b}, dir);
        } else if (MathUtil::SameDirection(ab.cross(abc), ao)) {
            return NSLine(simplex = {a, b}, dir);
        } else if (MathUtil::SameDirection(abc, ao)) {
            dir = abc;
        } else {
            simplex = {a, c, b};
            dir = -abc;
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
        V3d ao = -a;

        V3d abc = ab.cross(ac);
        V3d acd = ac.cross(ad);
        V3d adb = ad.cross(ab);

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

    /** Gets diff of furthest points in opposite directions to get a vertex on hull of A - B*/
    V3d GreatestDiff(const Collider *a, const Collider *b, V3d dir) {
        return a->FurthestPointIn(dir) - b->FurthestPointIn(-dir);
    }

    //
    // EPA
    //
    std::pair<std::vector<V3d>, size_t> GetFaceNormals(
        const std::vector<V3d> &polytope,
        const std::vector<size_t> &faces
    ) {
        std::vector<V3d> norms;
        size_t min_triangle = 0;
        float min_dist = FLT_MAX;

        for (size_t i = 0; i < faces.size(); i += 3) {
            V3d a = polytope[faces[i]];
            V3d b = polytope[faces[i + 1]];
            V3d c = polytope[faces[i + 2]];

            V3d norm = (b - a).cross(c - a).normalize();
            float dist = norm.dot(a);

            if (dist < 0) {
                norm *= -1;
                dist *= -1;
            }

            norm.w = dist;
            norms.push_back(norm);

            if (dist < min_dist) {
                min_triangle = i / 3;
                min_dist = dist;
            }
        }

        return { norms, min_triangle };
    }

    void AddIfUniqueEdge(
        std::vector<std::pair<size_t, size_t>> &edges,
        const std::vector<size_t> &faces, size_t a, size_t b
    ) {
        auto reverse = std::find(edges.begin(), edges.end(), std::make_pair(faces[b], faces[a]));

        if (reverse != edges.end()) edges.erase(reverse);
        else edges.emplace_back(faces[a], faces[b]);
    }
}

bool Collision::GJK(Simplex &vertices, const Collider *a, const Collider *b) {
    // Initial support point (first direction is arbitrary)
    V3d sup = GreatestDiff(a, b, V3d::unit_x);

    // Create simplex to try and cover origin
    vertices.push_front(sup);

    // Dir towards origin
    V3d to_origin = -sup;

    while (true) {
        sup = GreatestDiff(a, b, to_origin);

        // Current closest vertex is closest possible, no collision
        if (sup.dot(to_origin) <= 0) return false;

        vertices.push_front(sup);

        if (NextSimplex(vertices, to_origin)) return true;
    }
}

bool Collision::GJK(const Collider *a, const Collider *b) {
    Simplex vertices;
    return GJK(vertices, a, b);
}

V3d Collision::EPA(const Simplex &simplex, const Collider *a, const Collider *b) {
    std::vector<V3d> polytope(simplex.begin(), simplex.end());
    std::vector<size_t> faces = { 0, 1, 2,  0, 3, 1,  0, 2, 3,  1, 3, 2 };

    auto [norms, min_face] = GetFaceNormals(polytope, faces);

    V3d min_norm;
    float min_dist = FLT_MAX;

    while (min_dist == FLT_MAX) {
        min_norm = norms[min_face];
        min_dist = norms[min_face].w;

        V3d sup = GreatestDiff(a, b, min_norm);
        float s_dist = min_norm.dot(sup);

        if (std::abs(s_dist - min_dist) > 0.001f) {
            min_dist = FLT_MAX;

            std::vector<std::pair<size_t, size_t>> unique_edges;

            for (size_t i = 0; i < norms.size(); ++i) {
                size_t f = i * 3;
                if (MathUtil::SameDirection(norms[i], sup - polytope[faces[f]])) {
                    AddIfUniqueEdge(unique_edges, faces, f, f + 1);
                    AddIfUniqueEdge(unique_edges, faces, f + 1, f + 2);
                    AddIfUniqueEdge(unique_edges, faces, f + 2, f);

                    faces[f + 2] = faces.back();
                    faces.pop_back();
                    faces[f + 1] = faces.back();
                    faces.pop_back();
                    faces[f] = faces.back();
                    faces.pop_back();

                    norms[i] = norms.back();
                    norms.pop_back();

                    --i;
                }
            }

            if (unique_edges.size() == 0) break;

            std::vector<size_t> new_faces;
            for (auto [edge_index1, edge_index2] : unique_edges) {
                new_faces.push_back(edge_index1);
                new_faces.push_back(edge_index2);
                new_faces.push_back(polytope.size());
            }
            polytope.push_back(sup);

            auto [new_norms, new_min_face] = GetFaceNormals(polytope, new_faces);

            float old_min_dist = FLT_MAX;
            for (size_t i = 0; i < norms.size(); ++i) {
                if (norms[i].w < old_min_dist) {
                    old_min_dist = norms[i].w;
                    min_face = i;
                }
            }

            if (new_norms[new_min_face].w < old_min_dist) min_face = new_min_face + norms.size();

            faces.insert(faces.end(), new_faces.begin(), new_faces.end());
            norms.insert(norms.end(), new_norms.begin(), new_norms.end());
        }
    }

    return MathUtil::AdjustToLength(min_norm, min_dist + 0.001f);
}

V3d Collision::EPA(const Collider *a, const Collider *b) {
    Simplex simplex;
    GJK(simplex, a, b);
    return EPA(simplex, a, b);
}