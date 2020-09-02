#include <ll/propositional_logic.hpp>
#include <iostream>
#include "test.hpp"
using namespace ll;
namespace test {
    bool test_propositional_logic() {
        auto a = int_variable("a");
        auto b = int_variable("b");
        auto expr = a+1;
        string result = (((b==0)|(a>1))&(expr == 4)).to_string();
        string expect = "((b=0|a>1)&(a+1)=4)";
        return (result == expect);
    }
}
