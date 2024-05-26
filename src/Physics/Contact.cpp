#include "Contact.hpp"

void Contact::SetBodyData(Rigidbody *one, Rigidbody *two, float friction, float restitution) {
    body[0] = one;
    body[1] = two;
    this->friction = friction;
    this->restitution = restitution;
}