#pragma once
namespace infra
{
    template<class TFrom, class TTo>
    struct convert{
        constexpr TTo operator()(const TFrom& from){
            if constexpr(is_complex_v<TFrom> && is_real_or_complex_v<TTo> ) // Specifically, for backward compatibility
            {
                if constexpr(is_complex_v<TTo>) return TTo(from.real(), from.imag());
                else if constexpr(is_real_v<TTo>) return TTo(std::abs(from));
                else unreachable_constexpr_if<>();
            }
            else{
                return TTo(from);
            }
        }
    };
    template<class TFrom, class TTo, size_t N>
    struct convert<std::array<TFrom, N>, std::array<TTo, N>>{
        constexpr std::array<TTo,N> operator()(const std::array<TFrom,N>& from){
            std::array<TTo,N> to;
            for(size_t i = 0; i < N; i++){
                to.at(i) = convert<TFrom, TTo>{}(from.at(i)); 
            }
            return to;
        }
    };
    template<class TFrom, class TTo, class TAlloc1, class TAlloc2>
    struct convert<std::vector<TFrom, TAlloc1>, std::vector<TTo, TAlloc2>>{
        constexpr std::vector<TTo, TAlloc2> operator()(const std::vector<TFrom, TAlloc1>& from){
            std::vector<TTo, TAlloc2> to; to.reserve(from.size());
            for(size_t i = 0; i < from.size(); i++){
                to.push_back(convert<TFrom, TTo>{}(from.at(i))); 
            }
            return to;
        }
    };
    template<class TFrom, class TTo, size_t N>
    struct convert<std::vector<TFrom>, std::array<TTo, N>>{
        constexpr std::array<TTo,N> operator()(const std::vector<TFrom>& from){
            if(from.size() != N) throw std::length_error("convert failed. size not equal " + to_string(std::make_tuple(from.size(), N)));
            std::array<TTo,N> to;
            for(size_t i = 0; i < N; i++){
                to.at(i) = convert<TFrom, TTo>{}(from.at(i)); 
            }
            return to;
        }
    };
    template<class TFrom, class TTo, size_t N, class TAlloc>
    struct convert<std::array<TFrom, N>, std::vector<TTo, TAlloc>>{
        constexpr std::vector<TTo, TAlloc> operator()(const std::array<TFrom, N>& from){
            std::vector<TTo, TAlloc> to; to.reserve(from.size());
            for(size_t i = 0; i < from.size(); i++){
                to.push_back(convert<TFrom, TTo>{}(from.at(i))); 
            }
            return to;
        }
    };
    template<class TTo, class TFrom> TTo convert_to(const TFrom& from){return convert<TFrom, TTo>{}(from);}
}