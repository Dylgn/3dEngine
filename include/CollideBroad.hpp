#pragma once

#include "Rigidbody.hpp"

struct PotentialContact {
    Rigidbody *body[2];
};

// A node in a bounding volume hierarchy using a binary tree
template<typename BoundingVolume> class BVHNode {
    public:
        BVHNode *children[2]; // Children of this node
        BoundingVolume volume; // Bounding volume containing all descendants
        Rigidbody *body; // Body contained at this node. Only leaf nodes have bodies

        /** Checks if node is a leaf. Its a leaf if it contains a body. */
        bool isLeaf() const;

        /** Checks for potential contacts in this bvh, placing them in the given array
         * @param contacts The array of pootential contacts
         * @param limit Maximum number of potential contacts it will find
         * @returns Number of potential contacts it found
        */
        unsigned getPotentialContacts(PotentialContact *contacts, unsigned limit) const;

        /** Checks if this node's bounding volume is colliding with the given node's bounding volume. */
        bool overlaps(const BVHNode<BoundingVolume> *other);
    private:
        unsigned getPotentialContactsWith(const BVHNode<BoundingVolume> *other, PotentialContact *contacts);
};