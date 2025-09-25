#pragma once 
#include <type_traits>
#include <complex>
#include <vector>

namespace std{
    template<class T> struct tuple_size<std::complex<T>> : integral_constant<size_t, 2> {};
    template<size_t I, class T> struct tuple_element<I, std::complex<T>> {using type = T;};
    template<size_t I, class T> T& get(std::complex<T>& c) 
    {
        static_assert(std::is_reference_v<decltype(c.real())>, "return value is NOT reference type");
        if constexpr (I == 0)
            return c.real();
        else
            return c.imag();
    }
    template<size_t I, class T> T get(const std::complex<T>& c) 
    {
        if constexpr (I == 0)
            return c.real();
        else
            return c.imag();
    }
    #if __cplusplus < 202002L
        template<typename T> using remove_cvref_t = std::remove_cv_t<std::remove_reference_t<T>>;
    #endif
}

template<class T = void> struct unreachable_constexpr_if{unreachable_constexpr_if(){static_assert(!std::is_same_v<T, T>, "template unreachable");}}; 

template<class ...T> struct is_complex: std::false_type{};
template<class T> struct is_complex <std::complex<T>>: std::true_type{};
template<class T> constexpr static bool is_complex_v = is_complex<T>::value; 
template<class T> constexpr static bool is_real_v = std::is_floating_point_v<T> || std::is_integral_v<T>;
template<class T> constexpr static bool is_real_or_complex_v = is_complex_v<T> ||  is_real_v<T>;

//== type-traist of complex_t & real_t 
template <class T, class Enable = void>struct complex_type;
namespace details
{
    template <class, class = void> struct has_cT : std::false_type {};
    template <class, class = void> struct has_rT : std::false_type {};

    template <class T>struct has_cT<T, std::void_t<class T::cT>> : std::true_type {};
    template <class T>struct has_rT<T, std::void_t<class T::rT>> : std::true_type {};

    template <class T> inline constexpr bool has_cT_v = has_cT<T>::value;
    template <class T> inline constexpr bool has_rT_v = has_rT<T>::value;
}
template <class T> struct complex_type<T, std::enable_if_t<details::has_cT_v<T>>> {
    using type = class T::cT;
};
template <class U> struct complex_type<std::complex<U>> {
    using type = std::complex<U>;
};
template <class T> struct complex_type<T, std::enable_if_t<std::is_arithmetic_v<T>>> {
    using type = std::complex<T>;
};
template <class T, class Enable = void> struct real_type;
template <class T>struct real_type<T, std::enable_if_t<details::has_rT_v<T>>> {
    using type = typename T::rT;
};
template <class U> struct real_type<std::complex<U>> {
    using type = U;
};
template <class T>struct real_type<T, std::enable_if_t<std::is_arithmetic_v<T>>> {
    using type = T;
};
template <class T> using complex_t = typename complex_type<T>::type;
template <class T> using real_t = typename real_type<T>::type;

//== type-traist of MKL
template <class T>
constexpr static bool is_s = std::is_same_v<float, std::remove_cv_t<T>>;
template <class T>
constexpr static bool is_d = std::is_same_v<double, std::remove_cv_t<T>>;
template <class T>
constexpr static bool is_c = std::is_same_v<std::complex<float>, std::remove_cv_t<T>>;
template <class T>
constexpr static bool is_z = std::is_same_v<std::complex<double>, std::remove_cv_t<T>>;
template <class T> constexpr const char* get_numerical_type_str_suffix() 
{
    if constexpr(is_s<T>) return "s";
    if constexpr(is_d<T>) return "d";
    if constexpr(is_c<T>) return "c";
    if constexpr(is_z<T>) return "z";
    if constexpr(std::is_same_v<T, int32_t>) return "n";
    return "";
}

//== is_tuple_v
template <class T>struct is_tuple : std::false_type {};
template <class... Ts> struct is_tuple<std::tuple<Ts...>> : std::true_type {};
template<class T> constexpr bool is_tuple_v = is_tuple<T>::value;
template <class TTuple, class = std::make_index_sequence<std::tuple_size_v<TTuple>>> struct inverse_tuple_impl;
template <class TTuple, std::size_t... I> struct inverse_tuple_impl<TTuple, std::index_sequence<I...>> {
    static_assert(is_tuple_v<TTuple>);
    using type = std::tuple<
        std::tuple_element_t<sizeof...(I) - 1 - I, TTuple>...
    >;
};
template <class TTuple>using inverse_tuple_t = typename inverse_tuple_impl<TTuple>::type;
template<int N, class ...TArgs> struct select_type {
    static_assert(N < sizeof...(TArgs), "out of range");
    using type = std::tuple_element_t<N, std::tuple<TArgs...>>;
}; 
template<int N, class ...TArgs> using select_type_t = typename select_type<N, TArgs...>::type;
struct tuple_memory_check
{
    //== 运行时检查 tuple 实现是否于预期一致, 目前还无法做到编译时
    constexpr static bool __tuple_memory_check() {
        struct S {
            static constexpr std::tuple<size_t,size_t> value() { return std::make_tuple(101, 202); }
        };
        union U{
            std::tuple<size_t,size_t> t;
            size_t a[2];
        }u{S::value()};
        return (u.a[0] == 202) && (u.a[1] = 101);
    }
    tuple_memory_check(){if(!__tuple_memory_check()) throw std::runtime_error("tuple implemnt error");}
};

//== static loop
template<size_t i, size_t size, class TCallback, class ...TArgs>
static inline constexpr void static_for(TCallback&& callback, TArgs&& ...args)
{
    if constexpr( i < size){
        callback(i, std::forward<TArgs>(args)...);
        static_for<i+1, size>(std::forward<TCallback>(callback), std::forward<TArgs>(args)...);
    }
}

//== type-traist of stl-container
template <class T> struct is_vector : std::false_type {};
template <class T, class A> struct is_vector<std::vector<T, A>> : std::true_type {};
template <class T> constexpr bool is_vector_v = is_vector<T>::value;
namespace std{
    template <class T, size_t N> inline constexpr bool is_array_v<std::array<T, N>> = true;
}
template <class T> constexpr bool is_vec_or_array_v = std::is_array_v<T> || is_vector_v<T>;
template<class Tcell, class T, class Enable = void>
struct is_type_in_container : std::false_type {}; 
template<class Tcell, class T> struct is_type_in_container<Tcell, T, std::void_t<typename T::value_type>>
    : std::conditional_t<
        std::is_same_v<Tcell, typename T::value_type>,
        std::true_type,
        is_type_in_container<Tcell, typename T::value_type>
      >
{};
template <class Tcell, class T> constexpr bool is_type_in_container_v = is_type_in_container<Tcell, T>::value;

#include "pretty_print.hpp"
#include "numerics.hpp"
#include "convert.hpp"
#include "log.hpp"

