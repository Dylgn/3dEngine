#include "CollideBroad.hpp"

template <typename BoundingVolume>
BVHNode<BoundingVolume>::BVHNode(BVHNode *parent, const BoundingVolume &volume, Rigidbody *body):
    parent{parent}, volume{volume}, body{body}
{
    children[0] = nullptr;
    children[1] = nullptr;
}

template <typename BoundingVolume>
BVHNode<BoundingVolume>::~BVHNode() {
    if (parent) {
        BVHNode<BoundingVolume> *sibling;
        if (this == parent->children[0]) sibling = parent->children[1];
        else sibling = parent->children[0];

        // Write to parent
        parent->volume = sibling->volume;
        parent->body = sibling->body;
        parent->children[0] = sibling->children[0];
        parent->children[1] = sibling->children[1];

        // Delete old sibling (now parent)
        sibling->parent = nullptr;
        sibling->body = nullptr;
        sibling->children[0] = nullptr;
        sibling->children[1] = nullptr;
        delete sibling;

        parent->recalculateBoundingVolume();
    }

    if (children[0]) {
        children[0]->parent = nullptr;
        delete children[0];
    }
    if (children[1]) {
        children[1]->parent = nullptr;
        delete children[1];
    }
}

template <typename BoundingVolume>
bool BVHNode<BoundingVolume>::isLeaf() const
{
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
void BVHNode<BoundingVolume>::insert(Rigidbody *newBody, const BoundingVolume &newVolume) {
    // If leaf, split into two with this and new node
    if (isLeaf()) {
        children[0] = new BVHNode(this, volume, body);
        children[1] = new BVHNode(this, newVolume, newBody);

        this->body = nullptr;

        recalculateBoundingVolume();

    // Otherwise insert into one of the children, whoever would grow the least
    } else if (children[0]->volume.getVolume(newVolume) < children[1]->volume.getGrowth(newVolume)) {
        children[0]->insert(newBody, newVolume);
    } else {
        children[1]->insert(newBody, newVolume);
    }

}

template <typename BoundingVolume>
void BVHNode<BoundingVolume>::recalculateBoundingVolume(bool recurse) {
    if (isLeaf()) return;

    volume = BoundingVolume(
        children[0]->volume, children[1]->volume
    );

    if (recurse && parent) parent->recalculateBoundingVolume(true);
}

template <typename BoundingVolume>
unsigned BVHNode<BoundingVolume>::getPotentialContactsWith(const BVHNode<BoundingVolume> *other, PotentialContact *contacts, unsigned limit) {
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
