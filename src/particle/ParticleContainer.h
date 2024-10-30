#include <vector>
#include "Particle.h"



//struct Iterator {
//    using iterator_categor = std::forward_iterator_tag;
//    using difference_type = std::ptrdiff_t;
//    using value_type = Particle;
//    using pointer = Particle*;
//    using reference = Particle&;
//
//    Iterator(pointer ptr, int size) : m_ptr(ptr) {}
//
//    reference operator*() const { return *m_ptr; }
//    pointer operator->() { return m_ptr; }
//
//    Iterator& operator++() {
//        if(i < particles.size()) {
//
//        }
//    }
//    Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }
//
//    friend bool operator== (const Iterator& a, const Iterator& b) { return a.m_ptr == b.m_ptr; };
//    friend bool operator!= (const Iterator& a, const Iterator& b) {return a.m_ptr != b.m_ptr; };
//
//private:
//
//    pointer m_ptr;
//    pointer n_ptr;
//    int i;
//    int j;
//
//};