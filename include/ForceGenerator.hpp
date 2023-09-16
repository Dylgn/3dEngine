#pragma once

#include <vector>
#include "Rigidbody.hpp"

struct ForceGenerator {
    virtual void UpdateForce(Rigidbody *b, float elapsed_time) = 0;
};

class ForceRegistry {
    protected:
        struct ForceRegistration {
            Rigidbody *b;
            ForceGenerator *fg;
        };

        typedef std::vector<ForceRegistration> Registry;
        Registry registrations;
    public:
        /** Registers given particle and force generator pair. */
        void Add(Rigidbody *b, ForceGenerator *fg);

        /** Removes given particle and force generator pair. No effect if pair is not registered */
        void Remove(Rigidbody *b, ForceGenerator *fg);

        /** Clears all registrations from registry. */
        void Clear();

        /** Update particles in registry with their corresponding force generators. */
        void Update(float elapsed_time);
};

class Aerodynamics: public ForceGenerator {
    protected:
        M3x3 tensor; // Aerodynamic tensor for surface in body space
        V3d pos; // Relative pos of aerodynamic surface in body coords
        const V3d *wind_speed; // Wind speed of environment

    public:
        Aerodynamics(const M3x3 &tensor, const V3d &pos, const V3d *wind_speed);

        /** Applies foorce to given body */
        virtual void UpdateForce(Rigidbody *b, float elapsed_time);

    protected:
        /** Updates force on given rigid body using given tensor. */
        void UpdateForceFromTensor(Rigidbody *b, float elapsed_time, const M3x3 &tensor);
};

class AeroControl: public Aerodynamics {
    protected:
        M3x3 max_tensor; // Tensor for surface when control is at max value
        M3x3 min_tensor; // when at min value
        float control; // Current pos of control for surface. Between [-1,1]

    private:
        M3x3 GetTensor(); // Calculates final aerodynamic tensor for current control setting

    public:
        AeroControl(const M3x3 &base, const M3x3 &min, const M3x3 &max, const V3d &pos, const V3d *wind_speed);

        /** Set control position. Should be between [-1, 1], otherwise gives undefined results. */
        void SetControl(float value);

        /** Applies force to given rigidbody. */
        virtual void UpdateForce(Rigidbody *b, float elapsed_time);
};

class Buoyancy: public ForceGenerator {
    float max_depth; // Max depth of object before it generates max buoyancy force
    float volume; // Volume of object
    float water_height; // Height of water plane above y=0, parallel to XZ plane
    float liquid_density; // Density of the liquid. Pure water density is 1000kg/m3
    V3d centre_buoyancy; // Centre of buoyancy of rigidbody in body coords

    public:
        Buoyancy(const V3d &centre_buoyancy, float max_depth, float volume, float water_height, float liquid_density = 1000.0f);

        /** Applies force to given rigidbody. */
        virtual void UpdateForce(Rigidbody *b, float elapsed_time);
};