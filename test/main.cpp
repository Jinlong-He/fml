#include "test.hpp"
#include <iostream>
using namespace test;

int main(int argc, char* argv[]) {
    //test_determinize1();
    //test_determinize2();
    //test_determinize3();
    //test_determinize4();
    
    std::cout << test_minimize1() << std::endl;
    std::cout << test_minimize2() << std::endl;
    std::cout << test_minimize3() << std::endl;
    std::cout << test_minimize4() << std::endl;
    std::cout << test_minimize5() << std::endl;

    std::cout << test_basic_regular_expression1() << std::endl;

    //std::cout << test_minimize5() << std::endl;

    //test_intersect1();
    //test_intersect2();
    //test_intersect3();
    //test_intersect4();

    //test_union1();
    //test_union2();
    //test_union3();
    //test_union4();
    //test_equal();

    //test_ltl();
    //test_ctl();
    //test_propositional_logic();
    //test_tempalte();
    //test_state_prop();
    //test_poststar();
    //test_transducer();
}
