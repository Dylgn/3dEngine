#include <iterator>
#include "Object.hpp"
#include "PolyCollider.hpp"
#include "Collision.hpp"

Object::Object() : body{nullptr}, mesh{nullptr}, texture{nullptr}, transform{} {}

Object::Object(std::string mesh_file_path, std::string texture_file_path):
    mesh{new Mesh()}, texture{new Texture{texture_file_path}}, transform{new Transform{}} {
    body = new Body(new PolyCollider(mesh->LoadObject(mesh_file_path, true)));
}

Object::Object(Transform *transform, std::string mesh_file_path, std::string texture_file_path): 
    mesh{new Mesh()}, texture{new Texture{texture_file_path}}, transform{transform} {
    body = new Body{new PolyCollider{mesh->LoadObject(mesh_file_path, true)}};
}

Object::~Object() {}

void Object::SetBody(Body *body) {
    if (this->body) delete this->body;
    this->body = body;
}

Body *Object::GetBody() const {
    return body;
}

Mesh *Object::GetMesh() const {
    return mesh;
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

V3d Object::GetPos() const {
    return transform->pos;
}

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

bool Object::operator==(const Object &o) {
    return body == o.body && mesh == o.mesh && texture == o.texture && transform == o.transform;
}

void Object::OnCollision(Object &other) {
    collision_function(*this, other);
}

void Object::SetOnCollision(const std::function<void(Object &, Object &)> &func) {
    collision_function = func;
}

std::pair<bool, int> Object::GetIndexOf(int property) {
    int min = 0;
    int max = properties.size();
    while (true) {
        // Index in middle of [min,max] interval
        int i = (max - min) / 2 + min;
        // Only 1 element left in interval
        if (min >= max) return {property == properties[i], i};
        // Check if element is equal
        else if (property == properties[i]) return {true, i};
        // Element is greater than target, target must be in lower half of remaining interval
        else if (property < properties[i]) max = i;
        // Element is less than target, target must be in upper half of remaining interavl
        else min = i + 1;
    }
}

bool Object::AddProperty(int property) {
    auto ret = GetIndexOf(property);
    if (ret.first) return false;
    else {
        auto it = properties.begin();
        std::advance(it, ret.second);
        properties.insert(it, property);
        return true;
    }
}

bool Object::RemoveProperty(int property) {
    auto ret = GetIndexOf(property);
    if (ret.first) return false;
    else {
        auto it = properties.begin();
        std::advance(it, ret.second);
        properties.erase(it);
        return true;
    }
}

bool Object::ContainsProperty(int property) {
    return GetIndexOf(property).first;
}
