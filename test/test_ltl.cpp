#include <ll/linear_temporal_logic.hpp>
#include <iostream>
#include "test.hpp"
using namespace ll;
void test_ltl() {
    auto a = int_variable("a");
    auto b = a+1;
    std::cout << (make_U((b==9), (a>1))|(make_G(b==0))).to_string() << std::endl;
}
