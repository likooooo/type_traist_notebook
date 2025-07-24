#include "../include/type_traist.hpp"
#include <complex>
#include <cstring>

template<class T> bool test_array()
{
    matrix3x3<T> m{1,2,3,4,5,6,7,8,9};
    vec3<T> v{1,2,3};
    m -= v;
    // std::cout << m << std::endl;
    matrix3x3<T> golden{0,0,0,3,3,3,6,6,6};
    bool ret = full_compare<matrix3x3<T>, matrix3x3<T>>::equal(m, golden);
    if(!ret) std::cerr << "    test matrix-vector failed in " << __PRETTY_FUNCTION__ << std::endl;
    m += T(1);
    m -= m;
    std::memset(golden.data(), 0, sizeof(golden));
    ret &= full_compare<matrix3x3<T>, matrix3x3<T>>::equal(m, golden);
    if(!ret) std::cerr << "    test matrix-matrix failed in " << __PRETTY_FUNCTION__ << std::endl;
    return ret;
}
template<class T> bool test_vector()
{
    using point = vec2<T>;
    using vertex = std::vector<point>;
    using polys = std::vector<vertex>;
    static_assert(is_type_in_container_v<point, vertex>);
    point center {1, 1};
    vertex v {point{0,0}, point{1,1}, point{2,2}, point{3,3}};
    // std::cout << (v - center) << std::endl;
    // std::cout << (v + T(1)) << std::endl;

    polys p{v - center, v + T(1)};
    // std::cout << p << std::endl;
    // std::cout << p + center << std::endl;
    // std::cout << p - T(0.5) << std::endl;
    bool ret = full_compare<vertex, vertex>::equal(p.at(0) + T(2), p.at(1));
    if(!ret) std::cerr << "    test vector failed in " << __PRETTY_FUNCTION__ << std::endl;
    return true;
}


int main()
{
    bool success = true;
    success &= test_array<float>();
    success &= test_array<int>();
    success &= test_array<std::complex<double>>();
    test_vector<std::complex<double>>();

    std::cout << "test stl operator " << vec2<const char*>{"failed", "success"}[size_t(success)] << std::endl;
    return success ? 0 : 1;
}