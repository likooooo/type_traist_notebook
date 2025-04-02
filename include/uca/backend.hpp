#pragma once
#include "../type_traist.hpp"

namespace uca
{    
    template<class T>struct backend
    {
        bool enable {false};
        // y += x
        void (*VtAdd)(const int n, const T *x, T *y);
        void (*integral_x)(vec2<size_t>, T*);
        void (*integral_y)(vec2<size_t>, T*);
    };
}