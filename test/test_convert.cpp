#include "../include/type_traist.hpp"

namespace infra
{
    template<class TFrom, class ...TTo>
    void __convert_test(TFrom in)
    {
        using Tupe = std::tuple<TTo...>;
        Tupe result(convert_to<TTo>(in)...);
        using print_type = std::tuple<std::string, std::string>;
        debug_unclassified::print_table(
            std::vector<print_type>{
                print_type{to_string(in), to_string(result)}
            }, {
                TypeReflection<TFrom>(), TypeReflection<Tupe>()
            }
        );
    };
    void convert_test()
    {
        debug_unclassified("* begin convert test\n");
        __convert_test<int, float, double, std::complex<float>, std::string>(23);
        __convert_test<
            std::array<int, 3>, 
            std::array<double, 3>,
            std::vector<std::complex<float>>
        >({1,2,3});
    }
}

int main()
{
    debug_unclassified::verbose() = true;
    infra::convert_test();
}