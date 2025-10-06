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
    debug_unclassified::print_table(rows, {"lhs", "op", "rhs", "result"});
}

void test_matrix_multiply()
{
    const size_t N = 2;
    const size_t P = 3;
    matrix<double, N, P> matrix1 = {{
        {{1.0, 2.0, 3.0}},
        {{4.0, 5.0, 6.0}}
    }};

    const size_t M = 2;
    matrix<double, P, M> matrix2 = {{
        {{7.0, 8.0}},
        {{9.0, 10.0}},
        {{11.0, 12.0}}
    }};
    // auto result_matrix = matrix_multiply<double,N, M, P>(matrix1, matrix2);
    auto result_matrix = matrix_multiply(matrix1, matrix2);

    std::cout << "Matrix 1:" << std::endl;
    print_matrix(matrix1);
    std::cout << "\nMatrix 2:" << std::endl;
    print_matrix(matrix2);
    
    std::cout << "\nResult (Matrix 1 * Matrix 2):" << std::endl;
    print_matrix(result_matrix);
    assert((result_matrix == matrix2x2<double>{58,64,139,154}));
}

template<class T, size_t Dim>
void test_rotate_matrix()
{
    using rT = real_t<T>;
    using cT = complex_t<T>;
    vec<rT, rotation_combinations<Dim>().size()> angles{0};
    // auto& rotate_z = angles.at(std::min<size_t>(angles.size() - 1, 1));
    // rotate_z = 1_PI/6;
    angles.fill(1_PI/6);
    std::cout <<"* test_rotate_matrix" 
        "\n    result          =" << rotate_matrix<rT, Dim>(angles) <<
        "\n    plane-definition=" << rotation_combinations<Dim>() <<
        "\n    angles          =" << angles << std::endl << std::endl;
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
    test_matrix_multiply();

    test_rotate_matrix<std::complex<float>, 2>();
    test_rotate_matrix<double, 3>();
    test_rotate_matrix<float, 4>();
    bool run_matlab = false;
    if(run_matlab){
        const char* cmd = "matlab.exe -batch \""
            "theta = pi/6;"
            "R = [cos(theta) -sin(theta); sin(theta) cos(theta)];"
            "I = [0;1];I1 = [0,0;0,1];"
            "A = R*I; A1 = R*I1;"
            "\"R,A,A1;";
        return system(cmd);
    }
    return 0;
}