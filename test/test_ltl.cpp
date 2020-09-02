#include <ll/linear_temporal_logic.hpp>
#include <iostream>
#include "test.hpp"
using namespace ll;
namespace test {
    bool test_ltl() {
        auto a = int_variable("a");
        auto b = int_variable("b");
        auto expr = a+1;
        string result = (make_U((b==0),(a>1))&make_G(expr == 4)).to_string();
        string expect = "([b=0Ua>1]&G[(a+1)=4])";
        return (result == expect);
    }
}
