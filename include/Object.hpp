#pragma once

#include <functional>
#include "Body.hpp"
#include "Mesh.hpp"
#include "Texture.hpp"
#include "Transform.hpp"

namespace Obj {
    enum Property {
        gravity = 0, // takes gravity
        walks = 1 // is walking (to step up stairs)
    };
}

class Object {
    std::vector<int> properties;

    std::pair<bool, int> GetIndexOf(int property);
    protected:
        std::function<void(Object &, Object &)> collision_function = [](Object&,Object&){};
        
        Body *body; // Physics Bounding Body
        Mesh *mesh; // Graphic Mesh w/ Texture Coordinates
        Texture *texture; // Texture used by Mesh
        Transform *transform; // Transform (location & rotation)
    public:
        Object();
        Object(std::string mesh_file_path, std::string texture_file_path);
        Object(Transform *transform, std::string mesh_file_path, std::string texture_file_path);
        virtual ~Object();

        /** Set body of object */
        void SetBody(Body *body);
        /** Get body of object */
        Body *GetBody() const;
        /** Get mesh of object */
        Mesh *GetMesh() const;
        /** Set position of object */
        virtual void SetPos(const V3d &dir);
        /** Get position of object*/
        virtual V3d GetPos() const;
        /** Get collision normal with other
         * @return Collision normal. If there is no collision, return V3d::origin
        */
        virtual V3d GetCollisionNormal(const Object &other);
        /** Move object in given direction */
        virtual void Move(const V3d &dir);

        virtual bool operator==(const Object &o);

        /** Method that is called on collision */
        virtual void OnCollision(Object &other);

        /** Set function that is called by default OnCollision method */
        void SetOnCollision(const std::function<void(Object &, Object &)> &func);

        /** Adds property to list of properties
         * @return True if property isn't already in list, false otherwise
        */
        bool AddProperty(int property);
        /** Remove property from list of properties
         * @return True if property is removevd, false if property is not in list
        */
        bool RemoveProperty(int property);
        /** Checks if property is contained in list of properties
         * @return True if property is removed, false if property is not in list
        */
        bool ContainsProperty(int property);
};