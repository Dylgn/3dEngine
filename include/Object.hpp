#pragma once

#include "Body.hpp"
#include "Mesh.hpp"
#include "Texture.hpp"
#include "Transform.hpp"

class Object {
    protected:
        Body *body; // Physics Bounding Body
        Mesh *mesh; // Graphic Mesh w/ Texture Coordinates
        Texture *texture; // Texture used by Mesh
        Transform *transform; // Transform (location & rotation)
    public:
        Object();
        Object(std::string mesh_file_path, std::string texture_file_path);
        Object(Transform *transform, std::string mesh_file_path, std::string texture_file_path);
        virtual ~Object();

        void SetBody(Body *body);
        Body *GetBody() const;
        Mesh *GetMesh() const;
        
        virtual V3d GetCollisionNormal(const Object &other);
        virtual void Move(const V3d &dir);
        virtual void SetPos(const V3d &dir);
        virtual V3d GetPos() const;
};