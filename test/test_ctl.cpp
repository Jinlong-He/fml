#include <fml/ll/computation_tree_logic.hpp>
#include <iostream>
#include "test.hpp"
using namespace ll;
namespace test {
    bool test_ctl() {
        auto a = int_variable("a");
        auto b = int_variable("b");
        auto expr = a+1;
        string result = (make_EU((b==0),(a>1))&make_AG(expr == 4)).to_string();
        string expect = "(E[b=0Ua>1]&AG[(a+1)=4])";
        return (result == expect);
    }
}
