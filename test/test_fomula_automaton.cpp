#include <atl/fomula_automaton/fomula_automaton.hpp>
#include <iostream>
#include "test.hpp"
#include <atl/automaton_utility.hpp>
using namespace atl;
using namespace ll;
namespace test {
    void test_fomula_automaton() {
        fomula_automaton<> foa;
        int_variable a("a");
        add_control_state(foa, a, (a==1));
        add_transition(foa, a, a+1, a==1);
        print_automaton(foa);
    }
}
