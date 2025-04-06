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
        void (*fft)(real_t<T>*, size_t* shape, size_t n);
        void (*ifft)(real_t<T>*, size_t* shape, size_t n);
        void (*fft_outplace)(complex_t<T>*, complex_t<T>*,size_t* shape, size_t n);
        void (*ifft_outplace)(complex_t<T>*, complex_t<T>*, size_t* shape, size_t n);
    };
}