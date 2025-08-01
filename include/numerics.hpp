#pragma once
#include <cmath>
#include <algorithm>
#include <numeric>
#include <array>
#include <random>

constexpr long double operator"" _PI(long double value) {
    return value * M_PI;
}
constexpr long double operator"" _PI(unsigned long long value) {
    return value * M_PI;
}
constexpr std::complex<float> operator"" _I(long double value) {
    return std::complex<float>(0, value);
}
constexpr std::complex<float> operator"" _I(unsigned long long value) {
    return std::complex<float>(0, value);
}
constexpr std::complex<float> operator"" _PI_I(long double value) {
    return std::complex<float>(0, value * M_PI);

}
constexpr std::complex<float> operator"" _PI_I(unsigned long long value) {
    return std::complex<float>(0, value * M_PI);
}

template <class T, size_t N>
using vec = std::array<T, N>;
template <class T> using vec2 = vec<T, 2>;
template <class T> using vec3 = vec<T, 3>;
template <class T> using dual_vec = vec2<T>;

#define NUMERIC_OPERATOR_DEFINE(__op, __ref)\
template <class T, size_t N, class TScalar, class = std::enable_if_t<is_type_in_container_v<TScalar, std::array<T, N>> || is_real_or_complex_v<TScalar> >>\
std::array<T, N> __ref operator __op(std::array<T, N> __ref a, TScalar scalar){\
    std::array<T, N>& result = a;\
    for(unsigned i = 0; i < result.size(); i++) result.at(i) = a.at(i) __op scalar;\
    return result;\
}\
template <class T, size_t N, class TScalar, class = std::enable_if_t<is_type_in_container_v<TScalar, std::array<T, N>> || is_real_or_complex_v<TScalar> >>\
std::array<T, N> __ref operator __op(TScalar scalar, std::array<T, N> __ref a){\
    std::array<T, N>& result = a;\
    for(unsigned i = 0; i < result.size(); i++) result.at(i) = scalar __op a.at(i) ;\
    return result;\
}\
template <class T1, class T2, size_t N>\
std::array<T1, N>__ref operator __op(std::array<T1, N> __ref a, const std::array<T2, N>& b){\
    std::array<T1, N>& result = a;\
    if constexpr(std::is_same_v<T1, std::array<T2, N>>)\
        for(unsigned i = 0; i < result.size(); i++) result.at(i) = a.at(i) __op b;\
    else\
        for(unsigned i = 0; i < a.size(); i++) result.at(i) = a.at(i) __op b.at(i);\
    return result;\
}\
 \
 \
template <class T, class TAlloc, class TScalar, class = std::enable_if_t<is_type_in_container_v<TScalar, std::vector<T, TAlloc>> || is_real_or_complex_v<TScalar> >>\
std::vector<T, TAlloc> __ref operator __op(std::vector<T, TAlloc> __ref a, TScalar scalar){\
    std::vector<T, TAlloc>& result = a;\
    for(unsigned i = 0; i < result.size(); i++) result.at(i) = a.at(i) __op scalar;\
    return result;\
}\
template <class T, class TAlloc, class TScalar, class = std::enable_if_t<is_type_in_container_v<TScalar, std::vector<T, TAlloc>> || is_real_or_complex_v<TScalar> >>\
std::vector<T, TAlloc> __ref operator __op(TScalar scalar, std::vector<T, TAlloc> __ref a){\
    std::vector<T, TAlloc>& result = a;\
    for(unsigned i = 0; i < result.size(); i++) result.at(i) = scalar __op a.at(i);\
    return result;\
}\
template <class T, class TAlloc1, class TAlloc2>\
std::vector<T, TAlloc1> __ref operator __op(std::vector<T, TAlloc1> __ref a, const std::vector<T, TAlloc2>& b){\
    std::vector<T, TAlloc1>& result = a;\
    for(unsigned i = 0; i < a.size(); i++) result.at(i) = a.at(i) __op b.at(i);\
    return result;\
}
NUMERIC_OPERATOR_DEFINE(+=, &);
NUMERIC_OPERATOR_DEFINE(+, );
NUMERIC_OPERATOR_DEFINE(-=, &);
NUMERIC_OPERATOR_DEFINE(-, );
NUMERIC_OPERATOR_DEFINE(*=, &);
NUMERIC_OPERATOR_DEFINE(*, );
NUMERIC_OPERATOR_DEFINE(/=, &);
NUMERIC_OPERATOR_DEFINE(/, );


#define LOGIC_OPERATOR_DEFINE(__op)\
template <class T, size_t N, class TScalar>\
bool operator __op(const std::array<T, N>& container, TScalar scalar){\
    return std::all_of(container.begin(), container.end(), [&](const T& n){return n __op scalar;});\
}\
template <class T, class TAlloc, class TScalar>\
bool operator __op(const std::vector<T, TAlloc>& container, TScalar scalar){\
    return std::all_of(container.begin(), container.end(), [&](const T& n){return n __op scalar;});\
}
    LOGIC_OPERATOR_DEFINE(>);
    LOGIC_OPERATOR_DEFINE(>=);
    LOGIC_OPERATOR_DEFINE(<);
    LOGIC_OPERATOR_DEFINE(<=);
    LOGIC_OPERATOR_DEFINE(==);
    LOGIC_OPERATOR_DEFINE(!=);
    LOGIC_OPERATOR_DEFINE(&&);
    LOGIC_OPERATOR_DEFINE(||);

//== be careful std::lexicographical_compare
template<class T1, class T2> struct full_compare{
    static bool greater (const T1& a, const T2& b){return a > b;}
    static bool greater_equal (const T1& a, const T2& b){return a >= b;}
    static bool less (const T1& a, const T2& b){return a < b;}
    static bool less_equal (const T1& a, const T2& b){return a <= b;}
    static bool equal (const T1& a, const T2& b){return a == b;}
    static bool not_equal (const T1& a, const T2& b){return a != b;}
};
template<class _T1, class _T2, class A1, class A2>
struct full_compare<std::vector<_T1, A1>, std::vector<_T2, A2>>{
    using T1 = std::vector<_T1, A1>;
    using T2 = std::vector<_T2, A2>;
    static bool greater (const T1& a, const T2& b){
        if(a.size() != b.size()) return false;
        for(size_t i = 0; i < a.size(); i++)
            if(!full_compare<_T1, _T2>::greater(a.at(i), b.at(i))) return false;
        return true;
    }
    static bool greater_equal (const T1& a, const T2& b){
        if(a.size() != b.size()) return false;
        for(size_t i = 0; i < a.size(); i++)
            if(!full_compare<_T1, _T2>::greater_equal(a.at(i), b.at(i))) return false;
        return true;
    }
    static bool less (const T1& a, const T2& b){
        if(a.size() != b.size()) return false;
        for(size_t i = 0; i < a.size(); i++)
            if(!full_compare<_T1, _T2>::less(a.at(i), b.at(i))) return false;
        return true;
    }
    static bool less_equal (const T1& a, const T2& b){
        if(a.size() != b.size()) return false;
        for(size_t i = 0; i < a.size(); i++)
            if(!full_compare<_T1, _T2>::less_equal(a.at(i), b.at(i))) return false;
        return true;
    }
    static bool equal (const T1& a, const T2& b){
        if(a.size() != b.size()) return false;
        for(size_t i = 0; i < a.size(); i++)
            if(!full_compare<_T1, _T2>::equal(a.at(i), b.at(i))) return false;
        return true;
    }
    static bool not_equal (const T1& a, const T2& b){
        if(a.size() != b.size()) return false;
        for(size_t i = 0; i < a.size(); i++)
            if(!full_compare<_T1, _T2>::not_equal(a.at(i), b.at(i))) return false;
        return true;
    }
};
template<class _T1, class _T2, size_t N>
struct full_compare<std::array<_T1, N>, std::array<_T2, N>>{
    using T1 = std::array<_T1, N>;
    using T2 = std::array<_T2, N>;
    static bool greater (const T1& a, const T2& b){
        if(a.size() != b.size()) return false;
        for(size_t i = 0; i < a.size(); i++)
            if(!full_compare<_T1, _T2>::greater(a.at(i), b.at(i))) return false;
        return true;
    }
    static bool greater_equal (const T1& a, const T2& b){
        if(a.size() != b.size()) return false;
        for(size_t i = 0; i < a.size(); i++)
            if(!full_compare<_T1, _T2>::greater_equal(a.at(i), b.at(i))) return false;
        return true;
    }
    static bool less (const T1& a, const T2& b){
        if(a.size() != b.size()) return false;
        for(size_t i = 0; i < a.size(); i++)
            if(!full_compare<_T1, _T2>::less(a.at(i), b.at(i))) return false;
        return true;
    }
    static bool less_equal (const T1& a, const T2& b){
        if(a.size() != b.size()) return false;
        for(size_t i = 0; i < a.size(); i++)
            if(!full_compare<_T1, _T2>::less_equal(a.at(i), b.at(i))) return false;
        return true;
    }
    static bool equal (const T1& a, const T2& b){
        if(a.size() != b.size()) return false;
        for(size_t i = 0; i < a.size(); i++)
            if(!full_compare<_T1, _T2>::equal(a.at(i), b.at(i))) return false;
        return true;
    }
    static bool not_equal (const T1& a, const T2& b){
        if(a.size() != b.size()) return false;
        for(size_t i = 0; i < a.size(); i++)
            if(!full_compare<_T1, _T2>::not_equal(a.at(i), b.at(i))) return false;
        return true;
    }
};

template<class T> constexpr inline std::enable_if_t<std::is_arithmetic_v<T>, T> conj (T n){return n;} 
template<class T> constexpr inline std::enable_if_t<is_complex_v<T>, T> conj (T n){return std::conj(n);} 
template<class T, size_t N> constexpr inline std::array<T, N> conj(std::array<T, N> vec){
    if constexpr(is_complex_v<T>) for(auto& n : vec) n = std::conj(n);
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
    if constexpr(is_complex_v<T>) for(auto& n : vec) n = std::conj(n);
    return vec;
}
template<class T, size_t N> constexpr inline std::array<T, N> floor(std::array<T, N> n){
    if constexpr(std::is_floating_point_v<T>){
        for(auto& x:n) x = std::floor(x);
    }
    return n;
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

template <class T, size_t fast_dim, size_t... dims>
struct matrix_type
{
    using type = vec<
        typename matrix_type<T, dims...>::type
        , fast_dim
    >;
};
template <class T, size_t fast_dim>
struct matrix_type<T, fast_dim>
{
    using type = vec<T, fast_dim>;
};

template <class T, size_t slow_dim, size_t... dims>
using matrix = typename matrix_type<T, slow_dim, dims...>::type;
template <class T> using matrix2x3 = matrix<T, 2, 3>; 
template <class T> using matrix2x2 = matrix<T, 2, 2>; 
template <class T> using matrix3x3 = matrix<T, 3, 3>; 

template<class T, size_t N> constexpr inline matrix<T, N, N> set_matrix_row_major(const matrix<T, N, N>& m) {
    matrix<T, N, N> result{};
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < N; ++j) {
            result[i][j] = m[j][i];
        }
    }
    return result;
}
// is_matrix, is_vector

template<class T, size_t N> constexpr inline T operator | (vec<T, N> row_major_matrix_or_vec, T col_major_vec_or_scalar)
{
    col_major_vec_or_scalar = conj(col_major_vec_or_scalar);
    if constexpr (std::is_arithmetic_v<T> || is_complex_v<T>)
    {
        row_major_matrix_or_vec *= col_major_vec_or_scalar;
        return std::accumulate(row_major_matrix_or_vec.begin(), row_major_matrix_or_vec.end(), T(0));
    }
    else{
        T vec_or_scalar{0};
        int i = 0; 
        for(auto& row : row_major_matrix_or_vec)
        {
            row *= col_major_vec_or_scalar;
            vec_or_scalar.at(i) = std::accumulate(row.begin(), row.end(), typename T::value_type(0));
            i++;
        }
        return vec_or_scalar;
    }
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


template <typename T, typename = void>
struct is_distribution : std::false_type {};

template <typename T>
struct is_distribution<T, std::void_t<typename T::param_type>> : std::true_type {};
template<class T, class = void> struct uniform_distribution;
template<class T> struct uniform_distribution<T, std::enable_if_t<std::is_integral_v<T>>>{
    using type = std::uniform_int_distribution<T>;
};   
template<class T> struct uniform_distribution<T, std::enable_if_t<std::is_floating_point_v<T>>>{
    using type = std::uniform_real_distribution<T>;
};

template <class T, class TDistribution, class TEngine = std::mt19937>
struct random_callable {
    static_assert(is_distribution<TDistribution>::value, "TDistribution must be a distribution type");
    using result_type = typename TDistribution::result_type;

    random_callable(result_type lb = result_type{0}, result_type ub = result_type{1}) : engine_(std::random_device{}()), distribution_(TDistribution(lb, ub)){}
    T operator()(){
        if constexpr(is_complex_v<T>){
            return T{distribution_(engine_), distribution_(engine_)};
        }else{
            return distribution_(engine_);
        }
    }
private:
    TEngine engine_;
    TDistribution distribution_;
};

template<class T> using uniform_random = random_callable<T, typename uniform_distribution<real_t<T>>::type, std::mt19937>;