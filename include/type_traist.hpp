#pragma once 
#include <type_traits>
#include <complex>

template<class T> struct unreachable_constexpr_if{unreachable_constexpr_if(){static_assert(std::is_same_v<T, void>, "template unreachable");}}; 

namespace std{
    template<class ...T> struct is_complex: std::false_type{};
    template<class T> struct is_complex <std::complex<T>>: std::true_type{};
    template<class T> constexpr static bool is_complex_v = is_complex<T>::value; 

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
}

template <class T> struct complex_type { 
    static_assert(std::is_floating_point_v<T> || std::is_integral_v<T> || std::is_complex_v<T>);
    using type = std::complex<T>;
};
template <class T> struct real_type {
    static_assert(std::is_floating_point_v<T> || std::is_integral_v<T> || std::is_complex_v<T>);
    using type = T;
};

//== real & complex number
template <class T> using complex_t = typename complex_type<T>::type;
template <class T> using real_t = typename real_type<T>::type;
template<class T> constexpr static bool is_complex_v = std::is_same_v<T, complex_t<T>>;
template<class T> constexpr static bool is_real_v = std::is_same_v<T, real_t<T>>;
// template specialization
template <class T> struct real_type<std::complex<T>> { using type = T;};
template <class T> struct complex_type<std::complex<T>> {using type = std::complex<T>;};

//== is_tuple_v
template <class T>struct is_tuple : std::false_type {};
template <class... Ts> struct is_tuple<std::tuple<Ts...>> : std::true_type {};
template<class T> constexpr bool is_tuple_v = is_tuple<T>::value;

#if __cplusplus < 202002L
namespace std{
    template<typename T> using remove_cvref_t = std::remove_cv_t<std::remove_reference_t<T>>;
}
#endif

template<size_t i, size_t size, class TCallback, class ...TArgs>
static inline constexpr void static_for(TCallback&& callback, TArgs&& ...args)
{
    if constexpr( i < size){
        callback(i, std::forward<TArgs>(args)...);
        static_for<i+1, size>(std::forward<TCallback>(callback), std::forward<TArgs>(args)...);
    }
}

#include "pretty_print.hpp"
#include "numerics.hpp"