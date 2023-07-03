#include "Simplex.hpp"

Simplex::Simplex(): m_vertices{{0,0,0,0}}, m_size{0} {}

Simplex &Simplex::operator=(std::initializer_list<V3d> list) {
    for (auto v = list.begin(); v != list.end(); v++) {
        m_vertices[std::distance(list.begin(), v)] = *v;
    }
    m_size = list.size();
    // const std::size_t size = list.size();
    // std::size_t i = 0;

    // for (auto v : list) {
    //     m_vertices[i] = v;
    // }

    // m_size = list.size();

    return *this;
}

V3d &Simplex::operator[](unsigned int i) { return m_vertices[i]; }

void Simplex::push_front(V3d v) {
    m_vertices = {v, m_vertices[0], m_vertices[1], m_vertices[2]};
    m_size = std::min(m_size + 1, 4u);
}

unsigned int Simplex::size() const { return m_size; }

auto Simplex::begin() const { return m_vertices.begin(); }
auto Simplex::end() const { return m_vertices.end() - (4 - m_size); }