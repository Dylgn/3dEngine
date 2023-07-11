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

        void push_front(const V3d &v);
        unsigned int size() const;

        auto begin() const { return m_vertices.begin(); };
        auto end() const { return m_vertices.end() - (4 - m_size); }; 
};