#include "../include/type_traist.hpp"
#include <assert.h>

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
template<class T1, class T2>void vector_logic_test(std::vector<T1> a, std::vector<T2> b)
{
    std::array<std::string, 2> status{"NO", "YES"};
    using print_type = std::tuple<std::string, std::string, std::string, std::string>;
    std::vector<print_type> rows;
    rows.reserve(a.size());
    assert(a.size() == b.size());
    for(size_t i = 0; i < a.size(); i++){
        rows.push_back(
            print_type{
                to_string(a.at(i)), ">", 
                to_string(b.at(i)), status[full_compare<T1, T2>::greater(a.at(i), b.at(i))]
            }
        );
        rows.push_back(
            print_type{
                to_string(a.at(i)), ">=", 
                to_string(b.at(i)), status[full_compare<T1, T2>::greater_equal(a.at(i), b.at(i))]
            }
        );
        
        rows.push_back(
            print_type{
                to_string(a.at(i)), "<", 
                to_string(b.at(i)), status[full_compare<T1, T2>::less(a.at(i), b.at(i))]
            }
        );
        
        rows.push_back(
            print_type{
                to_string(a.at(i)), "<=", 
                to_string(b.at(i)), status[full_compare<T1, T2>::less_equal(a.at(i), b.at(i))]
            }
        );
    }
    debug_unclassified(rows, {"lhs", "op", "rhs", "result"});
}
int main()
{
    debug_unclassified::verbose() = true;
    test<float>();
    test<std::complex<float>>();
    vector_logic_test(std::vector<std::vector<float>>{
        std::vector<float>{1, 2}, std::vector<float>{1, 2},
    }, std::vector<std::vector<float>>{
        std::vector<float>{3, 2}, std::vector<float>{3, 1},
    });
    vector_logic_test(std::vector<std::array<float,2>>{
        std::array<float, 2>{1, 2}, std::array<float, 2>{1, 2},
    }, std::vector<std::array<double, 2>>{
        std::array<double, 2>{3, 2}, std::array<double, 2>{3, 1},
    });
    vector_logic_test(
        std::vector<std::vector<float>>{
            std::vector<float>{-1, 2},
            std::vector<float>{0, 1},
            std::vector<float>{1, 2}
        },  std::vector<size_t>{0, 1, 3}
    );
}