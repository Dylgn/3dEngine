#include "Object.hpp"
#include "PolyCollider.hpp"
#include "Collision.hpp"

Object::Object(): body{nullptr}, mesh{nullptr}, texture{nullptr}, transform{} {}

Object::Object(std::string mesh_file_path, std::string texture_file_path):
    mesh{new Mesh()}, texture{new Texture{texture_file_path}}, transform{new Transform{}} {
    body = new Body(new PolyCollider(mesh->LoadObject(mesh_file_path, true)));
}

Object::Object(Transform *transform, std::string mesh_file_path, std::string texture_file_path): 
    mesh{new Mesh()}, texture{new Texture{texture_file_path}}, transform{transform} {
    body = new Body{new PolyCollider{mesh->LoadObject(mesh_file_path, true)}};
}

Object::~Object() {}

V3d Object::GetCollisionNormal(const Object &other) {
    if (!body || !other.body) return V3d::origin;

    Simplex simplex;
    return Collision::GJK(simplex, other.body->collider, body->collider) ?
        Collision::EPA(simplex, other.body->collider, body->collider) :
        V3d::origin;
}

void Object::Move(const V3d &dir) {
    if (body) body->Move(dir);
    if (mesh) mesh->Move(dir);
    transform->pos += dir;
}

void Object::SetPos(const V3d &dir) {
    if (body) {
        V3d center = body->collider->GetCenter();
        body->Move(dir - center);
    }
    if (mesh) {
        V3d center = body->collider->GetCenter();
        body->Move(dir - center);
    }
    transform->pos = dir;
}
