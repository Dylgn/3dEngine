#pragma once

#include <array>
#include "V3d.hpp"

class Simplex {
    std::array<V3d, 4> m_vertices;
    unsigned int m_size;

    public:
        Simplex();

        Simplex &operator=(std::initializer_list<V3d> list);
        V3d &operator[](unsigned int i);

        void push_front(V3d v);
        unsigned int size() const;

        auto begin() const;
        auto end() const; 
};