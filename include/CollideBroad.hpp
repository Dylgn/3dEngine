#pragma once

#include "Rigidbody.hpp"

struct PotentialContact {
    Rigidbody *body[2];
};

// A node in a bounding volume hierarchy using a binary tree
template<typename BoundingVolume> class BVHNode {
    public:
        BVHNode *parent; // Parent of this node
        BVHNode *children[2]; // Children of this node
        BoundingVolume volume; // Bounding volume containing all descendants
        Rigidbody *body; // Body contained at this node. Only leaf nodes have bodies

        BVHNode(BVHNode *parent, const BoundingVolume &volume, Rigidbody *body = nullptr);

        /** Also deletes children. (But not the rigidbodies). Recalculates parent's bounding volume. */
        ~BVHNode();

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

        /** Inserts the given rigid body and bounding volume into the hierarchy. */
        void insert(Rigidbody *body, const BoundingVolume &volume);

        /** Recalculates bounding volume based on bounding volumes of its children, if any. */
        void recalculateBoundingVolume(bool recurse = true);

    private:
        unsigned getPotentialContactsWith(const BVHNode<BoundingVolume> *other, PotentialContact *contacts, unsigned limit);
};