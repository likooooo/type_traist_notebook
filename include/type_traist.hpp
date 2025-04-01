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

template<class T = void> struct unreachable_constexpr_if{unreachable_constexpr_if(){static_assert(std::is_same_v<T, T>, "template unreachable");}}; 

template<class ...T> struct is_complex: std::false_type{};
template<class T> struct is_complex <std::complex<T>>: std::true_type{};
template<class T> constexpr static bool is_complex_v = is_complex<T>::value; 
// template<class T> constexpr static bool is_complex_v = std::is_same_v<T, complex_t<T>>;
template<class T> constexpr static bool is_real_v = std::is_floating_point_v<T> || std::is_integral_v<T>;
template<class T> constexpr static bool is_real_or_complex_v = is_complex_v<T> ||  is_real_v<T>;

template <class T> struct complex_type { 
    static_assert(is_real_v<T>);
    using type = std::complex<T>;
};
template <class T> struct real_type {
    static_assert(is_real_or_complex_v<T>);
    using type = std::conditional_t<is_real_v<T>, T, std::complex<T>>;
};

//== real & complex number
template <class T> using complex_t = typename complex_type<T>::type;
template <class T> using real_t = typename real_type<T>::type;
// template specialization
template <class T> struct real_type<std::complex<T>> { using type = T;};
template <class T> struct complex_type<std::complex<T>> {using type = std::complex<T>;};

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


template<size_t i, size_t size, class TCallback, class ...TArgs>
static inline constexpr void static_for(TCallback&& callback, TArgs&& ...args)
{
    if constexpr( i < size){
        callback(i, std::forward<TArgs>(args)...);
        static_for<i+1, size>(std::forward<TCallback>(callback), std::forward<TArgs>(args)...);
    }
}


template <class T> struct is_vector : std::false_type {};
template <class T, class A> struct is_vector<std::vector<T, A>> : std::true_type {};
template <class T> constexpr bool is_vector_v = is_vector<T>::value;
#include "pretty_print.hpp"
#include "numerics.hpp"
#include "convert.hpp"

