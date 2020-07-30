#include <ll/propositional_logic.hpp>
#include <iostream>
#include "test.hpp"
using namespace ll;
void test_propositional_logic() {
    auto a = int_variable("a");
    //auto v = int_value(1);
    auto b = a+1;
    std::cout << ((b==0)|(b>true)&(b<1)).to_string() << std::endl;
}
