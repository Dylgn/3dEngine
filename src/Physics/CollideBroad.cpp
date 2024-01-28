#include "CollideBroad.hpp"

template <typename BoundingVolume>
bool BVHNode<BoundingVolume>::isLeaf() const {
    return body;
}

template <typename BoundingVolume>
unsigned BVHNode<BoundingVolume>::getPotentialContacts(PotentialContact *contacts, unsigned limit) const {
    if (isLeaf() || limit == 0) return 0;

    return children[0]->getPotentialContactsWith(
        children[1], contacts, limit
    );
}

template <typename BoundingVolume>
bool BVHNode<BoundingVolume>::overlaps(const BVHNode<BoundingVolume> *other) {
    return volume->overlaps(other->volume);
}

template <typename BoundingVolume>
unsigned BVHNode<BoundingVolume>::getPotentialContactsWith(const BVHNode<BoundingVolume> *other, PotentialContact *contacts) {
    if (!overlaps(other) || limit == 0) return 0;

    // Potential Contact if both leaves
    if (isLeaf() && other->isLeaf()) {
        contacts->body[0] = body;
        contacts->body[1] = other->body;
        return 1;
    }

    // Detemine which node to descend into. If none are leaves, picks one with largest volume
    if (other->isLeaf() || (!isLeaf() && volume->getVolume() >= other->volume->getVolume())) {
        unsigned count = children[0]->getPotentialContactsWith(other, contacts, limit); // Recurse

        // Check for enough slots to do other side
        if (limit > count) {
            return count + children[1]->getPotentialContactsWith(other, contacts + count, limit - count);
        } else return count;
    } else {
        // Recurse other node
        unsigned count = children[0]->getPotentialContactsWith(other->children[0], contacts, limit);

        if (limit > count) {
            return count + getPotentialContactsWith(other->children[1], contacts + count, limit - count);
        } else return count;
    }
}
