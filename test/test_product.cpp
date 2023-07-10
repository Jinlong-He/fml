#include <fml/atl/finite_automaton/nondeterministic_finite_automaton.hpp>
#include <fml/atl/finite_automaton/deterministic_finite_automaton.hpp>
#include "test.hpp"
using namespace atl;

namespace test {
    //test intersect for nondeterministic_finite_automaton<char> & 
    //                   nondeterministic_finite_automaton<char>
    bool test_product1() {
        nondeterministic_finite_automaton<> lhs({'a', 'b'});
        nondeterministic_finite_automaton<> rhs({'a', 'b'});
        add_initial_state(lhs);
        add_state(lhs);
        add_final_state(lhs);
        add_state(lhs);
        set_final_state(lhs, 0);
        add_transition(lhs, 0, 0, char('a'));
        add_transition(lhs, 0, 1, char('a'));
        add_transition(lhs, 1, 2, char('b'));
        add_transition(lhs, 2, 2, char('b'));
        add_transition(lhs, 0, 3, char('a'));

        add_initial_state(rhs);
        add_state(rhs);
        add_final_state(rhs);
        set_final_state(rhs, 0);
        add_transition(rhs, 0, 1, char('a'));
        add_transition(rhs, 0, 2, char('a'));
        add_transition(rhs, 1, 2, char('b'));
        nondeterministic_finite_automaton<> result, expect({'a', 'b'});
        product(lhs, rhs, result);
        print_fa(result);
        add_initial_state(expect);
        add_final_state(expect);
        add_final_state(expect);
        set_final_state(expect, 0);
        add_transition(expect, 0, 1, char('a'));
        add_transition(expect, 1, 2, char('b'));
    }
}
