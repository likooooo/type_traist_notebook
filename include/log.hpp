#pragma once
#include <chrono>
#include <string>
#include <sstream>
#include <iomanip>

template<class Derived>
struct basic_print 
{

    basic_print() = default;
    basic_print(const char *fmt, ...) 
    {
        if (!Derived::verbose()) return;
        va_list ap;
        va_start(ap, fmt);

        std::ostream& os = Derived::print_to();

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
    template<class... Args>
    static void print_table(
        const std::vector<std::tuple<Args...>>& lines, 
        const std::array<std::string, sizeof...(Args)>& titles, 
        size_t truncate_width = 50) 
    {
        if (!Derived::verbose()) return;
        ::print_table(Derived::print_to(), lines, titles, truncate_width);
    }
    template<class... Args>
    static void out(Args&& ...args) {
        if (!Derived::verbose()) return;
        ((Derived::print_to() << args), ...) << std::endl;
    }
};

template<class T = void> struct debug_print : public basic_print<debug_print<T>> {
    static bool& verbose() {
        static bool vb = false;
        return vb;
    }
    static std::ostream& print_to() {
        static std::ostream& os = std::cout;
        return os;
    }
    using basic_print<debug_print<T>>::basic_print;
    debug_print(const char *fmt, ...) {
        va_list ap;
        va_start(ap, fmt);
        typename  basic_print<debug_print<T>>::basic_print(fmt, ap);
        va_end(ap);
    }
};

template<class T = void> struct error_print : public basic_print<error_print<T>> {
    static bool& verbose() {
        static bool vb = true;
        return vb;
    }
    static std::ostream& print_to() {
        static std::ostream& os = std::cerr;
        return os;
    }
    using basic_print<error_print<T>>::basic_print;
    error_print(const char *fmt, ...) {
        va_list ap;
        va_start(ap, fmt);
        typename basic_print<error_print<T>>::basic_print(fmt, ap);
        va_end(ap);
    }
};
template<class T = void> struct trace_print : public basic_print<trace_print<T>> {
    static bool& verbose() {
        static bool vb = true;
        return vb;
    }
    static std::ostream& print_to() {
        static std::ostream& os = std::cout;
        return os;
    }
    static int& get_depth()
    {
        thread_local static int depth = 0;
        return depth;
    }
    template<class ...Args>trace_print(const char* function_name, const char* file, int line, Args&& ...args) 
        : function_name_(function_name), file_(file), line_(line) 
    {
        depth_ = get_depth()++;
        log_entry();
        if constexpr(sizeof...(Args)) print_to() << std::make_tuple(std::forward<Args>(args)...) << std::endl;
        start_time_ = std::chrono::high_resolution_clock::now();
    }
    
    ~trace_print() 
    {
        using namespace std::chrono_literals;

        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
            end_time - start_time_);
        std::string time_str;
        if(duration > 1h){
            auto h = std::chrono::duration_cast<std::chrono::hours>(duration);
            auto m = std::chrono::duration_cast<std::chrono::minutes>(duration - h);
            std::ostringstream oss;
            oss << h.count() << "h:" << m.count() << "min";
            time_str = oss.str();
        }
        else if(duration > 1min){
            auto m = std::chrono::duration_cast<std::chrono::minutes>(duration);
            auto s = std::chrono::duration_cast<std::chrono::seconds>(duration - m);
            std::ostringstream oss;
            oss << m.count() << "min:" << s.count() << "s";
            time_str = oss.str();
        }
        else if(duration > 1s){
            auto s = std::chrono::duration_cast<std::chrono::seconds>(duration);
            auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(duration - s);
            std::ostringstream oss;
            oss << s.count() << "s:" << ms.count() << "ms";
            time_str = oss.str();
        }
        else{
            std::ostringstream oss;
            oss << duration.count() << "ms";
            time_str = oss.str();
        }
        log_exit(time_str);
        get_depth()--;
    }
    
private:
    void log_entry() const
    {
        std::ostringstream oss;
        oss << "[" << depth_ << "] " << get_indent() << "→ " << function_name_;
        
        if (!file_.empty()) {
            oss << " (" << file_;
            if (line_ > 0) {
                oss << ":" << line_;
            }
            oss << ")";
        }
        print_to() << oss.str() << std::endl;
    }
    void log_exit(const std::string& time_str) const 
    {
        std::ostringstream oss;
        oss << "[" << depth_ << "] " << get_indent() << "← " << function_name_;
        oss << " (time: " << time_str <<")";
        print_to() << oss.str() << std::endl;
    }
    std::string get_indent() const 
    {
        return std::string(depth_ * 2, ' '); 
    }
    
    std::string function_name_;
    std::string file_;
    int line_;
    int depth_;
    std::chrono::high_resolution_clock::time_point start_time_;
};

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
