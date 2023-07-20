#pragma once

#include "Body.hpp"
#include "Mesh.hpp"
#include "Texture.hpp"
#include "Transform.hpp"

struct Object {
    Body *body;
    Mesh *mesh;
    Texture *texture;
    Transform *transform;

    Object();
    Object(std::string mesh_file_path, std::string texture_file_path);
    Object(Transform *transform, std::string mesh_file_path, std::string texture_file_path);
    virtual ~Object();

    virtual V3d GetCollisionNormal(const Object &other);
    virtual void Move(const V3d &dir);
    virtual void SetPos(const V3d &dir);
};