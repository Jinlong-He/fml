#include <ll/computation_tree_logic.hpp>
#include <iostream>
#include "test.hpp"
using namespace ll;
void test_ctl() {
    auto a = int_variable("a");
    auto b = a+1;
    std::cout << (make_EU((b==9), (a>1))|(make_AG(b==0))).to_string() << std::endl;
}
