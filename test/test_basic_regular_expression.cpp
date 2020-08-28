#include <atl/regular_expression/basic_regular_expression.hpp>
#include <atl/finite_automaton/nondeterministic_finite_automaton.hpp>
#include <atl/finite_automaton/deterministic_finite_automaton.hpp>
#include <atl/automaton_utility.hpp>
#include "test.hpp"
using namespace atl;
namespace test {
    // test basic regular expression for char.
    bool test_basic_regular_expression1() {
        basic_regular_expression<> regEx("a");
        deterministic_finite_automaton<> result, expect;
        make_fa(regEx, result);
        add_initial_state(expect);
        add_final_state(expect);
        add_transition(expect, 0, 1, 'a');
        return (result == expect);
    }

    // test basic regular expression for char.
    bool test_basic_regular_expression2() {
        basic_regular_expression<> regEx("(a*b+)|(a+)");
        deterministic_finite_automaton<> result, expect;
        make_fa(regEx, result);
        add_initial_state(expect);
        add_final_state(expect);
        add_final_state(expect);
        add_transition(expect, 0, 1, 'a');
        add_transition(expect, 0, 2, 'b');
        add_transition(expect, 1, 1, 'a');
        add_transition(expect, 1, 2, 'b');
        add_transition(expect, 2, 2, 'b');
        return (result == expect);
    }

    // test basic regular expression for char.
    bool test_basic_regular_expression3() {
        std::vector<Symbol> word({Symbol("a")});
        basic_regular_expression<Symbol> regEx(word);
        deterministic_finite_automaton<Symbol> result, expect;
        make_fa(regEx, result);
        add_initial_state(expect);
        add_final_state(expect);
        add_transition(expect, 0, 1, Symbol("a"));
        return (result == expect);
    }

    // test basic regular expression for char.
    bool test_basic_regular_expression4() {
        basic_regular_expression<> regEx("a(b|c)?");
        deterministic_finite_automaton<> result, expect;
        make_fa(regEx, result);
        add_initial_state(expect);
        add_final_state(expect);
        add_final_state(expect);
        add_transition(expect, 0, 1, 'a');
        add_transition(expect, 1, 2, 'b');
        add_transition(expect, 1, 2, 'c');
        return (result == expect);
    }
}
