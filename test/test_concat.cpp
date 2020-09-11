#include <atl/finite_automaton/nondeterministic_finite_automaton.hpp>
#include <atl/finite_automaton/deterministic_finite_automaton.hpp>
#include "test.hpp"
using namespace atl;

namespace test {
    //test concat for nondeterministic_finite_automaton<char> & 
    //                   nondeterministic_finite_automaton<char>
    bool test_concat1() {
        nondeterministic_finite_automaton<> lhs({'a', 'b', 'c'});
        nondeterministic_finite_automaton<> rhs({'a', 'b', 'c'});
        add_initial_state(lhs);
        add_final_state(lhs);
        add_final_state(lhs);
        add_transition(lhs, 0, 1, char('a'));
        add_transition(lhs, 0, 2, char('b'));

        add_initial_state(rhs);
        add_final_state(rhs);
        add_transition(rhs, 0, 1, char('c'));
        deterministic_finite_automaton<> result = (lhs + rhs), expect({'a', 'b', 'c'});

        add_initial_state(expect);
        add_state(expect);
        add_final_state(expect);
        add_transition(expect, 0, 1, char('a'));
        add_transition(expect, 0, 1, char('b'));
        add_transition(expect, 1, 2, char('c'));
        return (result == expect) &&
               accept(expect , "ac") && !accept(expect, "");
    }

    //test concat for nondeterministic_finite_automaton<Symbol> &
    //                   nondeterministic_finite_automaton<Symbol>
    bool test_concat2() {
        nondeterministic_finite_automaton<Symbol, 0, Symbol> lhs({Symbol("a"), Symbol("b")});
        nondeterministic_finite_automaton<Symbol, 0, Symbol> rhs({Symbol("a"), Symbol("b")});
        add_initial_state(lhs);
        add_final_state(lhs);
        add_transition(lhs, 0, 0, Symbol("a"), Symbol("1"));
        add_transition(lhs, 0, 1, Symbol("a"), Symbol("1"));

        add_initial_state(rhs);
        set_final_state(rhs, 0);
        add_transition(rhs, 0, 0, Symbol("b"), Symbol("2"));
        deterministic_finite_automaton<Symbol, 0, Symbol> lhs_dfa;
        deterministic_finite_automaton<Symbol, 0, Symbol> rhs_dfa;
        minimize(lhs, lhs_dfa);
        minimize(rhs, rhs_dfa);
        deterministic_finite_automaton<Symbol, 0, Symbol> result1, result2, expect1, expect2;
        result1 = (lhs + rhs);
        concat_fa(lhs_dfa, rhs_dfa, result2, intersect_merge<Symbol>());
        add_initial_state(expect1);
        add_final_state(expect1);
        add_final_state(expect1);
        add_transition(expect1, 0, 1, Symbol("a"), Symbol("1"));
        add_transition(expect1, 1, 1, Symbol("a"), Symbol("1"));
        add_transition(expect1, 1, 2, Symbol("b"), Symbol("2"));
        add_transition(expect1, 2, 2, Symbol("b"), Symbol("2"));

        add_initial_state(expect2);
        add_final_state(expect2);
        add_final_state(expect2);
        add_transition(expect2, 0, 1, Symbol("a"), Symbol("1"));
        add_transition(expect2, 1, 1, Symbol("a"), Symbol("1"));
        add_transition(expect2, 1, 2, Symbol("b"), Symbol("2"));
        add_transition(expect2, 2, 2, Symbol("b"), Symbol("2"));
        std::vector<Symbol> word1({Symbol("a"), Symbol("a"), Symbol("a"), Symbol("a"), Symbol("b")});
        std::vector<Symbol> word2({Symbol("b")});
        return (result1 == expect1) && (result2 == expect2) &&
               accept(expect1, word1) && !accept(expect2, word2);
    }

    //test concat for nondeterministic_finite_automaton<Symbol> &
    //                   nondeterministic_finite_automaton<Symbol>
    bool test_concat3() {
        nondeterministic_finite_automaton<Symbol, 0, Symbol> lhs({Symbol("a"), Symbol("b")});
        nondeterministic_finite_automaton<Symbol, 0, Symbol> rhs({Symbol("a"), Symbol("b")});
        add_initial_state(lhs);
        add_final_state(lhs);
        add_transition(lhs, 0, 1, Symbol("a"), Symbol("1"));

        add_initial_state(rhs);
        set_final_state(rhs, 0);
        add_transition(rhs, 0, 0, Symbol("a"), Symbol("1"));
        deterministic_finite_automaton<Symbol, 0, Symbol> lhs_dfa;
        deterministic_finite_automaton<Symbol, 0, Symbol> rhs_dfa;
        minimize(lhs, lhs_dfa);
        minimize(rhs, rhs_dfa);
        deterministic_finite_automaton<Symbol, 0, Symbol> result1, result2, 
                                                          expect1({Symbol("a"), Symbol("b")}), 
                                                          expect2({Symbol("a"), Symbol("b")});
        result1 = (lhs + rhs);
        concat_fa(lhs_dfa, rhs_dfa, result2, intersect_merge<Symbol>());
        add_initial_state(expect1);
        add_final_state(expect1);
        add_transition(expect1, 0, 1, Symbol("a"), Symbol("1"));
        add_transition(expect1, 1, 1, Symbol("a"), Symbol("1"));

        add_initial_state(expect2);
        add_final_state(expect2);
        add_transition(expect2, 0, 1, Symbol("a"), Symbol("1"));
        add_transition(expect2, 1, 1, Symbol("a"), Symbol("1"));
        std::vector<Symbol> word1({Symbol("a")});
        std::vector<Symbol> word2({});
        return (result1 == expect1) && (result2 == expect2) &&
               accept(expect1, word1) && !accept(expect2, word2);
    }
}
