#include "../include/type_traist.hpp"

#define define_test_with_numeric_types(func_without_input_args)  template<class ...T>\
    void func_without_input_args(){\
        using namespace infra;\
        debug_unclassified("* start %s\n", __PRETTY_FUNCTION__);\
        (func_without_input_args##_impl <T>(), ...);\
    }

namespace infra
{
    template<class T> void template_type_convert_impl()
    {
        using rT = real_t<T>;
        using cT = complex_t<T>;
        using print_type = std::tuple<std::string, std::string>;
        debug_unclassified("test of <%s>\n", TypeReflection<T>().c_str());
        debug_unclassified(std::vector<print_type>{print_type(TypeReflection<cT>(), TypeReflection<rT>())}, {" cT", "rT"});
        debug_unclassified("\n");
        static_assert(std::is_same_v<complex_t<rT>, cT>);
        static_assert(std::is_same_v<real_t<cT>, rT>);
    }
    template<class ...T> void __template_type_check(bool is_real, bool is_complex)
    {
        // ((temp &= (is_real == is_real_v<T>)),...)
        std::array<bool, sizeof...(T)> is_real_flag{is_real_v<T>...};
        std::array<bool, sizeof...(T)> is_complex_flag{is_complex_v<T>...};

        // full_compare{}::equal
        if(is_real_flag != is_real) throw std::runtime_error("check real failed");
        if(is_complex_flag != is_complex) throw std::runtime_error("check complex failed");
    }
    template<class ...rT>void template_type_check()
    {
        debug_unclassified("* start %s\n", __PRETTY_FUNCTION__);
        __template_type_check<rT...>(true, false);
        __template_type_check<complex_t<rT>...>(false, true);
    }
    template<class ...T> void tuple_test()
    {
        using TTuple    = std::tuple<T...>;
        using TInvTuple = inverse_tuple_t<TTuple>;
        debug_unclassified("* start %s\n", __PRETTY_FUNCTION__);
        tuple_memory_check();
        using print_type = std::tuple<std::string, std::string>;
        debug_unclassified(
            std::vector<print_type>{print_type(TypeReflection<TTuple>(), TypeReflection<TInvTuple>())}, 
            {"tuple", "inverse tuple"}
        );
        static_assert(std::is_same_v<TTuple, inverse_tuple_t<TInvTuple>>);
    }
    template<class ...T> void is_vector_test()
    {
        debug_unclassified("* start %s\n", __PRETTY_FUNCTION__);
        using print_type = std::tuple<std::string, std::string>;
        std::array<std::string, 2> status{"NO", "YES"};
        debug_unclassified(
            std::vector<print_type>{print_type(TypeReflection<T>(), status[is_vector_v<T>])...}, 
            {"", "is vector "}
        );
    }
}

define_test_with_numeric_types(template_type_convert)

int main()
{
    debug_unclassified::verbose() = true;
    template_type_convert<int
        , float, std::complex<double>
        // , std::array<int,2> //error
    >();
    infra::template_type_check<int, float, double, long double>();
    infra::tuple_test<int, float>();
    infra::is_vector_test<std::array<int,2>, std::vector<int>>();
    
    debug_unclassified::verbose() = false;
    error_unclassified(
        "\n"
        "**********************************\n"
        "*       inf all test success     *\n"
        "**********************************\n"
        "\n"
    );
}