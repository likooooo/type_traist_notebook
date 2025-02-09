#include "../include/type_traist_notebook.hpp"

void message_test_success(const char* s){
    printf("test success: %s\n\n", s);
}

template<class ...T>
void test_to_string(){
    ((std::cout << to_string<T>(T()) << std::endl), ...);
    message_test_success(__PRETTY_FUNCTION__);
}

void test_print_table()
{
    std::vector<std::tuple<std::string, std::string>> msg{
        std::make_tuple(std::string("task   name:"), "print task"),
        std::make_tuple(std::string("output type:"), "void"),
    };
    print_table(msg, {"* table(2x2) test", ""}, 80);
    message_test_success(__PRETTY_FUNCTION__);
}

int main(){
    test_print_table();
    test_to_string<int, void*, std::vector<float>, std::tuple<std::string, std::complex<double>>>();
}