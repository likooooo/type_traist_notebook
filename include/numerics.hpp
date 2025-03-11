#pragma once
#include <cmath>
#include <algorithm>
#include <numeric>
#include <array>

template <class T, size_t N>
using vec = std::array<T, N>;
template <class T> using vec2 = vec<T, 2>;
template <class T> using vec3 = vec<T, 3>;

#define NUMERIC_OPERATOR_DEFINE(__op, __ref)\
template <class T, size_t N, class TScalar>\
std::array<T, N> __ref operator __op(std::array<T, N> __ref a, TScalar scalar){\
    std::array<T, N>& result = a;\
    for(unsigned i = 0; i < result.size(); i++) result.at(i) = a.at(i) __op scalar;\
    return result;\
}\
template <class T1, class T2, size_t N>\
std::array<T1, N>__ref operator __op(std::array<T1, N> __ref a, const std::array<T2, N>& b){\
    std::array<T1, N>& result = a;\
    for(unsigned i = 0; i < a.size(); i++) result.at(i) = a.at(i) __op b.at(i);\
    return result;\
}\
template <class T, class TAlloc, class TScalar>\
std::vector<T, TAlloc> __ref operator __op(std::vector<T, TAlloc> __ref a, TScalar scalar){\
    std::vector<T, TAlloc>& result = a;\
    for(unsigned i = 0; i < result.size(); i++) result.at(i) = a.at(i) __op scalar;\
    return result;\
}\
template <class T, class TAlloc1, class TAlloc2>\
std::vector<T, TAlloc1> __ref operator __op(std::vector<T, TAlloc1> __ref a, const std::vector<T, TAlloc2>& b){\
    std::vector<T, TAlloc1>& result = a;\
    for(unsigned i = 0; i < a.size(); i++) result.at(i) = a.at(i) __op b.at(i);\
    return result;\
}

#define LOGIC_OPERATOR_DEFINE(__op)\
template <class T, size_t N, class TScalar>\
bool operator __op(const std::array<T, N>& container, TScalar scalar){\
    return std::all_of(container.begin(), container.end(), [&](const T& n){return n __op scalar;});\
}\
template <class T1, class T2, size_t N>\
bool operator __op(const std::array<T1, N>& a, const std::array<T2, N>& b){\
    bool flag = true;\
    for(unsigned i = 0; i < a.size(); i++) flag &= (a.at(i) __op b.at(i));\
    return flag;\
}\
template <class T, class TAlloc, class TScalar>\
bool operator __op(const std::vector<T, TAlloc>& container, TScalar scalar){\
    return std::all_of(container.begin(), container.end(), [&](const T& n){return n __op scalar;});\
}\
template <class T, class TAlloc1, class TAlloc2>\
bool operator __op(const std::vector<T, TAlloc1>& a, const std::vector<T, TAlloc2>& b){\
    if(a.size() != b.size()) return false;\
    bool flag = true;\
    for(unsigned i = 0; i < a.size(); i++) flag &= (a.at(i) __op b.at(i));\
    return flag;\
}


NUMERIC_OPERATOR_DEFINE(+=, &);
NUMERIC_OPERATOR_DEFINE(+, );
NUMERIC_OPERATOR_DEFINE(-=, &);
NUMERIC_OPERATOR_DEFINE(-, );
NUMERIC_OPERATOR_DEFINE(*=, &);
NUMERIC_OPERATOR_DEFINE(*, );
NUMERIC_OPERATOR_DEFINE(/=, &);
NUMERIC_OPERATOR_DEFINE(/, );
LOGIC_OPERATOR_DEFINE(>);
LOGIC_OPERATOR_DEFINE(>=);
LOGIC_OPERATOR_DEFINE(<);
LOGIC_OPERATOR_DEFINE(<=);
LOGIC_OPERATOR_DEFINE(==);
LOGIC_OPERATOR_DEFINE(!=);
LOGIC_OPERATOR_DEFINE(&&);
LOGIC_OPERATOR_DEFINE(||);

template<class T, size_t N> constexpr inline std::array<T, N> conj(std::array<T, N> vec){
    for(auto& n : vec) n = std::conj(n);
    return vec;
}
template<class T, class TAlloc> constexpr inline std::vector<T, TAlloc> exp(std::vector<T, TAlloc> vec){
    for(auto& n : vec) n = std::exp(n);
    return vec;
}
template<class T, size_t N> constexpr inline std::array<T, N> exp(std::array<T, N> vec){
    for(auto& n : vec) n = std::exp(n);
    return vec;
}
template<class T, class TAlloc> constexpr inline std::vector<T, TAlloc> conj(std::vector<T, TAlloc> vec){
    for(auto& n : vec) n = std::conj(n);
    return vec;
}
template<class T, size_t N> constexpr inline std::array<real_t<T>, N> norm(const std::array<T, N>& n){
    std::array<real_t<T>, N> vec;
    std::transform(n.begin(), n.end(), std::back_inserter(vec), [](T n){return std::norm(n);});
    return vec;
}
template<class T, class TAlloc> constexpr inline auto norm(const std::vector<T, TAlloc>& n){
    using alloc_rebind = typename std::allocator_traits<TAlloc>::template rebind_alloc<real_t<T>>;
    std::vector<real_t<T>, alloc_rebind> vec;
    std::transform(n.begin(), n.end(), std::back_inserter(vec), [](T n){return std::norm(n);});
    return vec;
}
template<class T, size_t N> constexpr inline std::array<real_t<T>, N> abs(const std::array<T, N>& n){
    std::array<real_t<T>, N> vec;
    std::transform(n.begin(), n.end(), std::back_inserter(vec), [](T n){return std::abs(n);});
    return vec;
}
template<class T, class TAlloc> constexpr inline auto abs(const std::vector<T, TAlloc>& n){
    using alloc_rebind = typename std::allocator_traits<TAlloc>::template rebind_alloc<real_t<T>>;
    std::vector<real_t<T>, alloc_rebind> vec;
    std::transform(n.begin(), n.end(), std::back_inserter(vec), [](T n){return std::abs(n);});
    return vec;
}
// norm(x, y) = x^2 + y^2
template<class T, size_t N> constexpr inline real_t<T> vector_norm(const std::array<T, N>& n){
    using rT = real_t<T>;
    return std::accumulate(n.begin(), n.end(), rT(0), 
        [](rT sum, T n){return sum + std::norm(n);}
    );
}

template <class T, size_t... dims>
struct matrix;
template <class T, size_t xsize>
struct matrix<T, xsize> : public vec<T, xsize>
{
    matrix<T, xsize>& operator=(const vec<T, xsize>& v){
        static_cast<vec<T, xsize>&>(*this) = v;
        return *this;
    }
};
template <class T, size_t fast_dim, size_t... dims>
struct matrix<T, fast_dim, dims...> : public vec<matrix<T, dims...>, fast_dim>
{
};

template <class T> using matrix2x3 = matrix<T, 2, 3>; 

constexpr long double operator"" _PI(long double value) {
    return value * M_PI;
}
constexpr long double operator"" _PI(unsigned long long value) {
    return value * M_PI;
}

template<class T> inline bool is_almost_equal(T a, T b, T epsion = std::numeric_limits<T>::epsilon())
{
    static_assert(std::is_floating_point_v<T>);
    return epsion > std::fabs(a-b);
}


namespace private_space
{
    template<class ...TTo, std::size_t... I>
    inline void decompose_impl(const std::tuple<TTo...>* pFrom, std::tuple<TTo*...> to, int i, std::index_sequence<I...>){
        // 和 tuple 内存布局有关
        ((std::get<I>(to)[i] = std::get<sizeof...(TTo) - 1 - I>(pFrom[i])), ...); 
    }
}
template<class TFrom, class ...TTo>
inline void decompose(const TFrom* from, size_t element_size, TTo* ...to)
{
    using pod_type = std::tuple<TTo...>;//decltype(std::make_tuple(TTo*{}...));
    static_assert(sizeof(pod_type) == sizeof(TFrom));

    auto pFrom = reinterpret_cast<const pod_type*>(from);
    for (int i = 0; i < element_size; i++) {
        private_space::decompose_impl(pFrom, std::tuple<TTo*...>(to...), i, std::index_sequence_for<TTo...>());
    }
}

namespace private_space
{
    template<class ...T> struct decompose_helper
    {
        template<class TTuple, size_t... I> constexpr static auto _helper(TTuple t, std::index_sequence<I...>)
        {
            return std::make_pair(std::get<0>(t), std::make_tuple(std::get<1 + I>(t)...));
        }
        using ZipT = decltype(_helper(std::tuple<T...>{}, std::make_index_sequence<sizeof...(T) - 1>{}));
        using TIn = typename ZipT::first_type;
        using TOut = typename ZipT::second_type;
    };

    template<class TOUT, class TIN, size_t...I> constexpr inline TOUT decompose_impl(const TIN& in, std::index_sequence<I...>)
    {
        TOUT output;
        (std::get<I>(output).resize(in.size()), ...);
        decompose(in.data(), in.size(), std::get<I>(output).data()...);
        return output;
    }
}
template<class ...T> inline auto decompose_from(const typename private_space::decompose_helper<std::vector<T>...>::TIn& in)
{
    return private_space::decompose_impl<typename private_space::decompose_helper<std::vector<T>...>::TOut>(
        in, std::make_index_sequence<sizeof...(T)-1>{}
    );
}