#pragma once
#include <string>
#include <iostream>
#include <cxxabi.h>
#include <vector>
#include <array>
#include <tuple>
#include <iomanip>
#include <fstream>
#include <memory>
#include <mutex>
#include <cstdarg>
#include <functional>

//== type to string
template<class T> inline std::string TypeReflection() {
    const char* mangled_name = typeid(T).name();
    int status = -1;
    char* realname = abi::__cxa_demangle(mangled_name, nullptr, nullptr, &status);
    if (status == 0 && realname != nullptr) {
        std::string result(realname);
        std::free(realname);
        return result;
    } else {
        return mangled_name;
    }
}
//==
inline void use_same_format(std::ostream& a, const std::ostream& b) 
{
    a.flags(b.flags());
    a.precision(b.precision());
    a.width(b.width());
    a.fill(b.fill());
}
template<class T, size_t N> inline std::ostream& operator<<(std::ostream& out, const std::array<T, N>& vec);
template<class T, class TAlloc> inline std::ostream& operator<<(std::ostream& out, const std::vector<T, TAlloc>& vec)
{
    out << "[";
    if(!vec.empty()){
        for(int i = 0; i < vec.size() - 1; i++) out << vec[i] << ",";
        out << vec.back();
    }
    out <<"]";
    return out;
}
template<class T, size_t N> inline std::ostream& operator<<(std::ostream& out, const std::array<T, N>& vec)
{
    out << "[";
    if(!vec.empty()){
        for(int i = 0; i < vec.size() - 1; i++) out << vec[i] << ",";
        out << vec.back();
    }
    out <<"]";
    return out;
}
namespace details{
    template<typename TTuple, size_t... Is> inline std::ostream& _to_stream(std::ostream& out, const TTuple& t, std::index_sequence<Is...>, const std::string& separator = ", ")
    {
        constexpr int N = sizeof...(Is);
        if constexpr(N == 0){
            out << std::get<0>(t);
        }
        else if constexpr(N > 0){
            ((out << std::get<Is>(t) << separator), ...);
            out << std::get<N>(t);
        }
        return out;
    }
}
template<class ...Args> inline std::ostream& operator<<(std::ostream& out, const std::tuple<Args...>& t)
{
    using TTuple = std::tuple<Args...>;
    using namespace details;
    out << "(";
    _to_stream(out, t,  std::make_index_sequence<std::tuple_size_v<TTuple> - 1>{});
    out << ")";
    return out;
}
namespace details{
    inline size_t get_invisable_length(const std::string& value){
        int length = 0;
        bool inEscapeSequence = false;
        for (char ch : value) {
            if (inEscapeSequence) {
                if (ch == 'm') {
                    inEscapeSequence = false;
                }
                length++;
            } else {
                if (ch == '\033' || ch == '\x1b') {
                    // 开始一个ANSI转义序列
                    inEscapeSequence = true;
                    length++;
                } 
            }
        }
        return length;
    }
    template<typename T> inline  size_t get_visible_length(const std::ostream& buf, const T& value) {
        std::ostringstream oss;
        use_same_format(oss, buf);
        oss << value;
        return oss.str().length() - get_invisable_length(oss.str());
    }
    template<typename T> inline  void compute_column_width(const std::ostream& buf, const T& value, size_t& max_width, size_t truncate_width) {
        max_width = std::min(truncate_width, std::max(max_width, get_visible_length(buf, value)));
    }
    template<typename Tuple, size_t... Is> inline   void compute_column_widths(const std::ostream& buf, Tuple&& tuple, std::index_sequence<Is...>, std::vector<size_t>& column_widths, size_t truncate_width) {
        (compute_column_width(buf, std::get<Is>(tuple), column_widths[Is], truncate_width), ...);
    }
    template<class T>inline std::string truncate_with_ellipsis(const std::ostream& buf, const T& t, size_t max_length, size_t ellipsis_length = 3) {
        std::ostringstream oss; 
        use_same_format(oss, buf);
        oss <<t;
        std::string str = oss.str(); 
        if (str.length() <= max_length) {
            return str;
        }
        size_t half_max_length = (max_length - ellipsis_length) / 2;
        if (half_max_length * 2 + ellipsis_length != max_length) {
            half_max_length++; 
        }
        std::string truncated = str.substr(0, half_max_length) + std::string(ellipsis_length, '.')  + str.substr(str.length() - half_max_length);
        return truncated;
    }
    template<typename Tuple, size_t... Is> inline  void print_row(std::ostream& buf, Tuple&& tuple, std::index_sequence<Is...>, const std::vector<size_t>& column_widths, size_t truncate_width) {
        auto processed_tuple = std::make_tuple(
            truncate_with_ellipsis(buf, std::get<Is>(tuple), truncate_width)...
        );
        ((buf << std::setw(column_widths[Is] + get_invisable_length(std::get<Is>(processed_tuple)))
                    << std::left << std::get<Is>(processed_tuple) << "  "), ...);
                    buf << std::endl;
    }
    template<typename... Args> inline std::vector<size_t> cal_column_width(const std::ostream& buf, const std::vector<std::tuple<Args...>>& lines, const std::array<std::string, sizeof...(Args)>& titles, size_t truncate_width = 50)
    {
        using namespace details;
        //== cal aligin size
        std::vector<size_t> column_widths(sizeof...(Args));
        compute_column_widths(buf, titles, std::make_index_sequence<sizeof...(Args)>(), column_widths, truncate_width);
        for (const auto& line : lines) {
            compute_column_widths(buf, line, std::make_index_sequence<sizeof...(Args)>(), column_widths, truncate_width);
        }
        return column_widths;
    }
    template<class TContainer>inline  void print_column_names(std::ostream& buf, const std::vector<size_t>& column_widths, const TContainer& titles){
        for (size_t i = 0; i < column_widths.size(); ++i) {
            buf << std::setw(column_widths[i]) << std::left << titles.at(i) <<"  ";
        }
    }
    inline void print_split_line (std::ostream& buf, const std::vector<size_t>& column_widths){
        for (size_t i = 0; i < column_widths.size(); ++i) {
            buf << std::string(column_widths[i] + 2, '-') << "-";
        }
    };
}
template<typename... Args> inline  std::vector<size_t> print_table(std::ostream& buf, const std::vector<std::tuple<Args...>>& lines, const std::array<std::string, sizeof...(Args)>& titles, size_t truncate_width = 50) {
    using namespace details;
    auto column_widths = cal_column_width(buf, lines, titles, truncate_width);
    print_column_names(buf, column_widths, titles); buf << std::endl;
    print_split_line(buf, column_widths); buf << std::endl;
    for (const auto& line : lines) {
        print_row(buf, line, std::make_index_sequence<sizeof...(Args)>(), column_widths, truncate_width);
    }
    return column_widths;
}
template<typename... Args> inline  std::vector<size_t> print_table(const std::vector<std::tuple<Args...>>& lines, const std::array<std::string, sizeof...(Args)>& titles, size_t truncate_width = 50) {
    return print_table(std::cout, lines, titles, truncate_width);
}

template<class T = void> struct debug_print
{
    static bool& verbose()
    {
        static bool vb = false;
        return vb;
    };     
    static std::ostream& print_to()
    {
        static std::ostream& print_to = std::cout;
        return print_to;
    };   
    debug_print() = default;
    debug_print(const char *fmt, ...) 
    {
        if(!verbose()) return;
        va_list ap;
        va_start(ap, fmt);
    
        std::ostream& os = print_to();
    
        va_list ap_copy;
        va_copy(ap_copy, ap);
        const int needed_size = vsnprintf(nullptr, 0, fmt, ap_copy) + 1;
        va_end(ap_copy);
    
        std::vector<char> buffer(needed_size);
        vsnprintf(buffer.data(), buffer.size(), fmt, ap);
        
        os << buffer.data();
        os.flush();
    
        va_end(ap);
    }
    template<class... Args> debug_print(const std::vector<std::tuple<Args...>>& lines, const std::array<std::string, sizeof...(Args)>& titles, size_t truncate_width = 50) {
        if(!verbose()) return;
        print_table(print_to(), lines, titles, truncate_width);
    }
    template<class... Args> static void out(Args&& ...args)
    {
        if(!verbose()) return;
        ((print_to() << args), ...) << std::endl;
    }
};
template<class T = void> struct error_print 
{
    static bool& verbose()
    {
        static bool vb = true;
        return vb;
    };     
    static std::ostream& print_to(){
        static std::ostream& print_to = std::cerr;
        return print_to;
    };   
    error_print() = default;
    error_print(const char *fmt, ...) 
    {
        if(!verbose()) return;
        va_list ap;
        va_start(ap, fmt);
    
        std::ostream& os = print_to();
    
        va_list ap_copy;
        va_copy(ap_copy, ap);
        const int needed_size = vsnprintf(nullptr, 0, fmt, ap_copy) + 1;
        va_end(ap_copy);
    
        std::vector<char> buffer(needed_size);
        vsnprintf(buffer.data(), buffer.size(), fmt, ap);
        
        os << buffer.data();
        os.flush();
    
        va_end(ap);
    }
    template<class... Args> error_print(const std::vector<std::tuple<Args...>>& lines, const std::array<std::string, sizeof...(Args)>& titles, size_t truncate_width = 50) {
        if(!verbose()) return;
        print_table(print_to(), lines, titles, truncate_width);
    }
    template<class... Args> static void out(Args&& ...args)
    {
        if(!verbose()) return;
        ((print_to() << args), ...) << std::endl;
    }
};

template <class T> inline std::string to_string(T&& input){
    std::stringstream ss;
    ss << input;
    return ss.str();
}
using debug_unclassified = debug_print<void>;
using error_unclassified = error_print<void>;

template<class T> struct verbose_guard
{
    bool backup;
    verbose_guard(bool print_enable) : backup(print_enable)
    {
        std::swap(T::verbose(), backup);
    }
    ~verbose_guard()
    {
        std::swap(T::verbose(), backup);
    }
};
