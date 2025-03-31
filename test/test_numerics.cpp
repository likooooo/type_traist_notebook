#include "../include/type_traist.hpp"

template<class rT>
void test()
{
    matrix2x2<rT> identity{0 ,1, 1, 0};
    vec2<rT> v {3,5};
    std::cout << (identity | v) << std::endl;
    std::cout << (vec<vec2<rT>, 1>{v} | v) << std::endl;
    std::cout << (v | rT(2)) << std::endl;
    std::cout << convert_to<long double>(rT(123)) << std::endl;;
}
int main()
{
    test<float>();
    test<std::complex<float>>();
}