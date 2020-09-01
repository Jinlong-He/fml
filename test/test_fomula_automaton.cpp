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
        int_variable b("b", int_value(1), int_value(10));
        int_variable c("c", 1, 10);
        add_control_state(foa, a, (a==1)&(c==2));
        add_control_state(foa, b, (b==1));
        add_input_state(foa, c);
        add_transition(foa, a, a+1, a==1);
        add_transition(foa, a, int_value(1), a>1);
        add_transition(foa, a, int_value(1), atomic_proposition("TRUE"));
        add_transition(foa, b, int_value(2), c>1);
        translate_nuxmv(foa, cout);
    }
}
