#include "AnchorSpringGenerator.hpp"

AnchorSpringGenerator::AnchorSpringGenerator(V3d *anchor, float spring_const, float rest_length): anchor{anchor}, spring_const{spring_const}, rest_length{rest_length} {}

void AnchorSpringGenerator::UpdateForce(Particle *p, float elapsed_time) {
    V3d force = p->GetPos();
    force += -*anchor;

    float magnitude = force.length();
    magnitude = (rest_length - magnitude) * spring_const;

    force = force.normalize() * magnitude;
    p->ApplyForce(force);
}
