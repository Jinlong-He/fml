#include <atl/finite_automaton/nondeterministic_finite_automaton.hpp>
#include <atl/finite_automaton/deterministic_finite_automaton.hpp>
#include "test.hpp"
using namespace atl;

namespace test {
    //test union for nondeterministic_finite_automaton<char> & 
    //                   nondeterministic_finite_automaton<char>
    bool test_union1() {
        nondeterministic_finite_automaton<> lhs({'a', 'b'});
        nondeterministic_finite_automaton<> rhs({'a', 'b'});
        add_initial_state(lhs);
        add_state(lhs);
        add_final_state(lhs);
        add_final_state(lhs);
        add_transition(lhs, 0, 0, char('a'));
        add_transition(lhs, 0, 1, char('a'));
        add_transition(lhs, 1, 2, char('b'));
        add_transition(lhs, 0, 3, char('b'));

        add_initial_state(rhs);
        add_state(rhs);
        add_final_state(rhs);
        add_transition(rhs, 0, 1, char('b'));
        add_transition(rhs, 1, 2, char('a'));
        deterministic_finite_automaton<> result = (lhs | rhs), expect({'a', 'b'});

        add_initial_state(expect);
        add_state(expect);
        add_final_state(expect);
        add_final_state(expect);
        add_transition(expect, 0, 1, char('a'));
        add_transition(expect, 0, 2, char('b'));
        add_transition(expect, 1, 1, char('a'));
        add_transition(expect, 1, 3, char('b'));
        add_transition(expect, 2, 3, char('a'));
        return (result == expect);
    }

    //test union for nondeterministic_finite_automaton<Symbol> &
    //                   nondeterministic_finite_automaton<Symbol>
    bool test_union2() {
        nondeterministic_finite_automaton<Symbol, 0, Symbol> lhs({Symbol("a"), Symbol("b")});
        nondeterministic_finite_automaton<Symbol, 0, Symbol> rhs({Symbol("a"), Symbol("b")});
        add_initial_state(lhs);
        add_final_state(lhs);
        add_transition(lhs, 0, 0, Symbol("a"), Symbol("1"));
        add_transition(lhs, 0, 1, Symbol("a"), Symbol("1"));

        add_initial_state(rhs);
        set_final_state(rhs, 0);
        add_transition(rhs, 0, 0, Symbol("a"), Symbol("1"));
        deterministic_finite_automaton<Symbol, 0, Symbol> lhs_dfa;
        deterministic_finite_automaton<Symbol, 0, Symbol> rhs_dfa;
        minimize(lhs, lhs_dfa);
        minimize(rhs, rhs_dfa);
        deterministic_finite_automaton<Symbol, 0, Symbol> result1, result2, expect1, expect2;
        result1 = (lhs | rhs);
        union_fa(lhs_dfa, rhs_dfa, result2, intersect_merge<Symbol>());
        add_initial_state(expect1);
        set_final_state(expect1, 0);
        add_transition(expect1, 0, 0, Symbol("a"), Symbol("1"));

        add_initial_state(expect2);
        set_final_state(expect2, 0);
        add_transition(expect2, 0, 0, Symbol("a"), Symbol("1"));
        std::vector<Symbol> word1({Symbol("a"), Symbol("a"), Symbol("a"), Symbol("a"), Symbol("a")});
        std::vector<Symbol> word2({Symbol("b")});
        return (result1 == expect1) && (result2 == expect2) &&
               accept(expect1, word1) && !accept(expect2, word2);
    }

    //test union for nondeterministic_finite_automaton<Symbol> &
    //                   nondeterministic_finite_automaton<Symbol>
    bool test_union3() {
        nondeterministic_finite_automaton<Symbol, 0, Symbol> lhs({Symbol("a"), Symbol("b")});
        nondeterministic_finite_automaton<Symbol, 0, Symbol> rhs({Symbol("a"), Symbol("b")});
        add_initial_state(lhs);
        add_final_state(lhs);
        add_transition(lhs, 0, 0, Symbol("a"), Symbol("1"));
        add_transition(lhs, 0, 1, Symbol("a"), Symbol("1"));

        add_initial_state(rhs);
        set_final_state(rhs, 0);
        add_transition(rhs, 0, 0, Symbol("a"), Symbol("2"));
        deterministic_finite_automaton<Symbol, 0, Symbol> lhs_dfa;
        deterministic_finite_automaton<Symbol, 0, Symbol> rhs_dfa;
        minimize(lhs, lhs_dfa);
        minimize(rhs, rhs_dfa);
        deterministic_finite_automaton<Symbol, 0, Symbol> result1, result2, 
                                                          expect1({Symbol("a"), Symbol("b")}), 
                                                          expect2({Symbol("a"), Symbol("b")});
        result1 = (lhs | rhs);
        union_fa(lhs_dfa, rhs_dfa, result2, intersect_merge<Symbol>());
        add_initial_state(expect1);
        set_final_state(expect1, 0);
        add_final_state(expect1);
        add_final_state(expect1);
        add_transition(expect1, 0, 1, Symbol("a"), Symbol("1"));
        add_transition(expect1, 0, 2, Symbol("a"), Symbol("2"));
        add_transition(expect1, 1, 1, Symbol("a"), Symbol("1"));
        add_transition(expect1, 2, 2, Symbol("a"), Symbol("2"));

        add_initial_state(expect2);
        set_final_state(expect2, 0);
        add_final_state(expect2);
        add_final_state(expect2);
        add_transition(expect2, 0, 1, Symbol("a"), Symbol("1"));
        add_transition(expect2, 0, 2, Symbol("a"), Symbol("2"));
        add_transition(expect2, 1, 1, Symbol("a"), Symbol("1"));
        add_transition(expect2, 2, 2, Symbol("a"), Symbol("2"));
        std::vector<Symbol> word1({});
        std::vector<Symbol> word2({Symbol("b")});
        return (result1 == expect1) && (result2 == expect2) &&
               accept(expect1, word1) && !accept(expect2, word2);
    }

    //test union for nondeterministic_finite_automaton<Symbol> &
    //                   nondeterministic_finite_automaton<Symbol>
    bool test_union4() {
        nondeterministic_finite_automaton<Symbol, 0, Symbol, bool> lhs({Symbol("a"), Symbol("b")});
        nondeterministic_finite_automaton<Symbol, 0, Symbol, bool> rhs({Symbol("a"), Symbol("b")});
        add_initial_state(lhs, true);
        add_final_state(lhs, true);
        add_transition(lhs, 0, 0, Symbol("a"), Symbol("1"));
        add_transition(lhs, 0, 1, Symbol("a"), Symbol("1"));

        add_initial_state(rhs, false);
        set_final_state(rhs, 0);
        add_transition(rhs, 0, 0, Symbol("a"), Symbol("1"));
        deterministic_finite_automaton<Symbol, 0, Symbol, bool> lhs_dfa;
        deterministic_finite_automaton<Symbol, 0, Symbol, bool> rhs_dfa;
        minimize(lhs, lhs_dfa);
        minimize(rhs, rhs_dfa);
        deterministic_finite_automaton<Symbol, 0, Symbol, bool> result1, result2, expect1, expect2;
        result1 = (lhs | rhs);
        union_fa(lhs_dfa, rhs_dfa, result2, intersect_merge<Symbol>(), intersect_merge<bool>());
        add_initial_state(expect1, true);
        set_final_state(expect1, 0);
        add_transition(expect1, 0, 0, Symbol("a"), Symbol("1"));

        add_initial_state(expect2, false);
        set_final_state(expect2, 0);
        add_transition(expect2, 0, 0, Symbol("a"), Symbol("1"));
        std::vector<Symbol> word1({Symbol("a"), Symbol("a"), Symbol("a"), Symbol("a"), Symbol("a")});
        std::vector<Symbol> word2({Symbol("b")});
        return (result1 == expect1) && (result2 == expect2) &&
               accept(expect1, word1) && !accept(expect2, word2);
    }

    bool test_union5() {
        nondeterministic_finite_automaton<> empty;
        nondeterministic_finite_automaton<> nfa({'a', 'b'});
        add_initial_state(nfa);
        add_state(nfa);
        add_final_state(nfa);
        add_state(nfa);
        set_final_state(nfa, 0);
        add_transition(nfa, 0, 0, char('a'));
        add_transition(nfa, 0, 1, char('a'));
        add_transition(nfa, 1, 2, char('b'));
        add_transition(nfa, 2, 2, char('b'));
        add_transition(nfa, 0, 3, char('a'));
        return (nfa == (empty | nfa)) && (nfa == (nfa | empty));
    }
}
