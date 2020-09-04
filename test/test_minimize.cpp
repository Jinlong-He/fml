#include <atl/finite_automaton/nondeterministic_finite_automaton.hpp>
#include <atl/finite_automaton/deterministic_finite_automaton.hpp>
#include "test.hpp"
using namespace atl;

namespace test {
    //test minimize for nondeterministic_finite_automaton<char>
    bool test_minimize1() {
        nondeterministic_finite_automaton<> nfa({'a', 'b'});
        add_initial_state(nfa);
        add_state(nfa);
        add_state(nfa);
        add_final_state(nfa);
        add_final_state(nfa);
        add_final_state(nfa);
        add_final_state(nfa);
        add_transition(nfa, 0, 1, char('a'));
        add_transition(nfa, 0, 2, char('b'));
        add_transition(nfa, 1, 2, char('b'));
        add_transition(nfa, 1, 3, char('a'));
        add_transition(nfa, 2, 1, char('a'));
        add_transition(nfa, 2, 4, char('b'));
        add_transition(nfa, 3, 3, char('a'));
        add_transition(nfa, 3, 5, char('b'));
        add_transition(nfa, 4, 4, char('b'));
        add_transition(nfa, 4, 6, char('a'));
        add_transition(nfa, 5, 5, char('b'));
        add_transition(nfa, 5, 6, char('a'));
        add_transition(nfa, 6, 5, char('b'));
        add_transition(nfa, 6, 3, char('a'));
        deterministic_finite_automaton<> result;
        minimize(nfa, result);
        deterministic_finite_automaton<> expect({'a', 'b'});
        add_initial_state(expect);
        add_state(expect);
        add_state(expect);
        add_final_state(expect);
        add_transition(expect, 0, 1, char('a'));
        add_transition(expect, 0, 2, char('b'));
        add_transition(expect, 1, 2, char('b'));
        add_transition(expect, 1, 3, char('a'));
        add_transition(expect, 2, 1, char('a'));
        add_transition(expect, 2, 3, char('b'));
        add_transition(expect, 3, 3, char('a'));
        add_transition(expect, 3, 3, char('b'));
        return (result == expect) && 
               !accept(nfa, "a") && accept(nfa, "abbbbbbb") &&
               !accept(expect, "a") && accept(expect, "abbbbbbb");
    }

    //test minimize for nondeterministic_finite_automaton<Symbol>
    bool test_minimize2() {
        nondeterministic_finite_automaton<Symbol> nfa({Symbol("a"), Symbol("b")});
        add_initial_state(nfa);
        set_final_state(nfa, 0);
        add_final_state(nfa);
        add_state(nfa);
        add_state(nfa);
        add_state(nfa);
        add_state(nfa);

        add_transition(nfa, 0, 1, Symbol("a"));
        add_transition(nfa, 0, 2, Symbol("b"));
        add_transition(nfa, 1, 1, Symbol("a"));
        add_transition(nfa, 1, 4, Symbol("b"));
        add_transition(nfa, 2, 1, Symbol("a"));
        add_transition(nfa, 2, 3, Symbol("b"));
        add_transition(nfa, 3, 3, Symbol("a"));
        add_transition(nfa, 3, 2, Symbol("b"));
        add_transition(nfa, 4, 0, Symbol("a"));
        add_transition(nfa, 4, 5, Symbol("b"));
        add_transition(nfa, 5, 5, Symbol("a"));
        add_transition(nfa, 5, 4, Symbol("b"));
        deterministic_finite_automaton<Symbol> result;
        minimize(nfa, result);
        deterministic_finite_automaton<Symbol> expect({Symbol("a"), Symbol("b")});
        add_initial_state(expect);
        set_final_state(expect, 0);
        add_state(expect);
        add_state(expect);
        add_transition(expect, 0, 0, Symbol("a"));
        add_transition(expect, 0, 1, Symbol("b"));
        add_transition(expect, 1, 0, Symbol("a"));
        add_transition(expect, 1, 2, Symbol("b"));
        add_transition(expect, 2, 2, Symbol("a"));
        add_transition(expect, 2, 1, Symbol("b"));
        std::vector<Symbol> word1({Symbol("b"), Symbol("b"), Symbol("b")});
        std::vector<Symbol> word2({Symbol("a")});
        return (result == expect) && 
               !accept(nfa, word1) && accept(nfa, word2) &&
               !accept(expect, word1) && accept(expect, word2);
    }

    //test minimize for nondeterministic_finite_automaton<Symbol>
    bool test_minimize3() {
        nondeterministic_finite_automaton<Symbol> nfa({Symbol("a"), Symbol("b")});
        add_initial_state(nfa);
        add_state(nfa);
        add_state(nfa);
        add_state(nfa);
        add_state(nfa);
        add_state(nfa);
        add_state(nfa);
        add_state(nfa);
        add_state(nfa);
        add_state(nfa);
        add_final_state(nfa);

        add_transition(nfa, 0, 1, Symbol(0));
        add_transition(nfa, 0, 7, Symbol(0));
        add_transition(nfa, 1, 2, Symbol(0));
        add_transition(nfa, 1, 4, Symbol(0));
        add_transition(nfa, 2, 3, Symbol("a"));
        add_transition(nfa, 3, 6, Symbol(0));
        add_transition(nfa, 4, 5, Symbol("b"));
        add_transition(nfa, 5, 6, Symbol(0));
        add_transition(nfa, 6, 1, Symbol(0));
        add_transition(nfa, 6, 7, Symbol(0));
        add_transition(nfa, 7, 8, Symbol("a"));
        add_transition(nfa, 8, 9, Symbol("b"));
        add_transition(nfa, 9, 10, Symbol("b"));
        deterministic_finite_automaton<Symbol> result;
        minimize(nfa, result);
        deterministic_finite_automaton<Symbol> expect({Symbol("a"), Symbol("b")});
        add_initial_state(expect);
        add_state(expect);
        add_state(expect);
        add_final_state(expect);
        add_transition(expect, 0, 1, Symbol("a"));
        add_transition(expect, 0, 0, Symbol("b"));
        add_transition(expect, 1, 1, Symbol("a"));
        add_transition(expect, 1, 2, Symbol("b"));
        add_transition(expect, 2, 1, Symbol("a"));
        add_transition(expect, 2, 3, Symbol("b"));
        add_transition(expect, 3, 1, Symbol("a"));
        add_transition(expect, 3, 0, Symbol("b"));
        std::vector<Symbol> word1({Symbol("c")});
        std::vector<Symbol> word2({Symbol("a"), Symbol("b"), Symbol("b")});
        return (result == expect) && 
               !accept(nfa, word1) && accept(nfa, word2) &&
               !accept(expect, word1) && accept(expect, word2);
    }

    //test minimize for nondeterministic_finite_automaton<Symbol, 0, Symbol>
    bool test_minimize4() {
        nondeterministic_finite_automaton<Symbol, 0, Symbol> nfa({Symbol("a"), Symbol("b")});
        add_initial_state(nfa);
        add_state(nfa);
        add_state(nfa);
        add_state(nfa);
        add_state(nfa);
        add_state(nfa);
        add_state(nfa);
        add_state(nfa);
        add_state(nfa);
        add_state(nfa);
        add_final_state(nfa);

        add_transition(nfa, 0, 1, Symbol(0), Symbol("[01]"));
        add_transition(nfa, 0, 7, Symbol(0), Symbol("[07]"));
        add_transition(nfa, 1, 2, Symbol(0), Symbol("[12]"));
        add_transition(nfa, 1, 4, Symbol(0), Symbol("[14]"));
        add_transition(nfa, 2, 3, Symbol("a"), Symbol("[23]"));
        add_transition(nfa, 3, 6, Symbol(0), Symbol("[36]"));
        add_transition(nfa, 4, 5, Symbol("b"), Symbol("[45]"));
        add_transition(nfa, 5, 6, Symbol(0), Symbol("[56]"));
        add_transition(nfa, 6, 1, Symbol(0), Symbol("[61]"));
        add_transition(nfa, 6, 7, Symbol(0), Symbol("[67]"));
        add_transition(nfa, 7, 8, Symbol("a"), Symbol("[78]"));
        add_transition(nfa, 8, 9, Symbol("b"), Symbol("[89]"));
        add_transition(nfa, 9, 10, Symbol("b"), Symbol("[910]"));
        deterministic_finite_automaton<Symbol, 0, Symbol> result;
        minimize(nfa, result);
        deterministic_finite_automaton<Symbol, 0, Symbol> expect;
        add_initial_state(expect);
        add_state(expect);
        add_state(expect);
        add_final_state(expect);
        add_transition(expect, 0, 0, Symbol("a"), Symbol("[01]&[12]&[23]"));
        add_transition(expect, 0, 1, Symbol("a"), Symbol("[07]&[78]"));
        add_transition(expect, 0, 0, Symbol("b"), Symbol("[01]&[14]&[45]"));
        add_transition(expect, 0, 0, Symbol("a"), Symbol("[36]&[61]&[12]&[23]"));
        add_transition(expect, 0, 1, Symbol("a"), Symbol("[36]&[67]&[78]"));
        add_transition(expect, 0, 0, Symbol("b"), Symbol("[36]&[61]&[14]&[45]"));
        add_transition(expect, 0, 0, Symbol("a"), Symbol("[56]&[61]&[12]&[23]"));
        add_transition(expect, 0, 1, Symbol("a"), Symbol("[56]&[67]&[78]"));
        add_transition(expect, 0, 0, Symbol("b"), Symbol("[56]&[61]&[14]&[45]"));
        add_transition(expect, 1, 2, Symbol("b"), Symbol("[89]"));
        add_transition(expect, 2, 3, Symbol("b"), Symbol("[910]"));
        std::vector<Symbol> word1({Symbol("a"), Symbol("b")});
        std::vector<Symbol> word2({Symbol("a"), Symbol("b"), Symbol("b")});
        return (result == expect) && 
               !accept(nfa, word1) && accept(nfa, word2) &&
               !accept(expect, word1) && accept(expect, word2);
    }

    //test minimize for nondeterministic_finite_automaton<Symbol, 0, Symbol, bool, Symbol>
    bool test_minimize5() {
        nondeterministic_finite_automaton<Symbol, 0, Symbol, bool, Symbol> nfa({Symbol("a"), Symbol("b")});
        atl::set_property(nfa, Symbol("test5"));
        add_initial_state(nfa, false);
        add_state(nfa, false);
        add_state(nfa, false);
        add_state(nfa, false);
        add_state(nfa, false);
        add_state(nfa, false);
        add_state(nfa, false);
        add_state(nfa, false);
        add_state(nfa, false);
        add_state(nfa, false);
        add_final_state(nfa, true);

        add_transition(nfa, 0, 1, Symbol(0), Symbol(0));
        add_transition(nfa, 0, 7, Symbol(0), Symbol(0));
        add_transition(nfa, 1, 2, Symbol(0), Symbol(0));
        add_transition(nfa, 1, 4, Symbol(0), Symbol(0));
        add_transition(nfa, 2, 3, Symbol("a"), Symbol("a"));
        add_transition(nfa, 3, 6, Symbol(0), Symbol(0));
        add_transition(nfa, 4, 5, Symbol("b"), Symbol("b"));
        add_transition(nfa, 5, 6, Symbol(0), Symbol(0));
        add_transition(nfa, 6, 1, Symbol(0), Symbol(0));
        add_transition(nfa, 6, 7, Symbol(0), Symbol(0));
        add_transition(nfa, 7, 8, Symbol("a"), Symbol("a"));
        add_transition(nfa, 8, 9, Symbol("b"), Symbol("b"));
        add_transition(nfa, 9, 10, Symbol("b"), Symbol("b"));
        deterministic_finite_automaton<Symbol, 0, Symbol, bool, Symbol> result;
        minimize(nfa, result);
        deterministic_finite_automaton<Symbol, 0, Symbol, bool, Symbol> expect({Symbol("a"), Symbol("b")});
        atl::set_property(expect, Symbol("test5"));
        add_initial_state(expect, false);
        add_state(expect, false);
        add_state(expect, false);
        add_final_state(expect, true);
        add_transition(expect, 0, 1, Symbol("a"), Symbol("a"));
        add_transition(expect, 0, 0, Symbol("b"), Symbol("b"));
        add_transition(expect, 1, 1, Symbol("a"), Symbol("a"));
        add_transition(expect, 1, 2, Symbol("b"), Symbol("b"));
        add_transition(expect, 2, 1, Symbol("a"), Symbol("a"));
        add_transition(expect, 2, 3, Symbol("b"), Symbol("b"));
        add_transition(expect, 3, 1, Symbol("a"), Symbol("a"));
        add_transition(expect, 3, 0, Symbol("b"), Symbol("b"));
        std::vector<Symbol> word1({Symbol("a"), Symbol("b")});
        std::vector<Symbol> word2({Symbol("a"), Symbol("b"), Symbol("b")});
        return (result == expect) && 
               !accept(nfa, word1) && accept(nfa, word2) &&
               !accept(expect, word1) && accept(expect, word2);
    }

    //test minimize for nondeterministic_finite_automaton<Symbol, 0, Symbol, bool, Symbol>
    bool test_minimize6() {
        nondeterministic_finite_automaton<Symbol, 0, Symbol, bool, Symbol> nfa({Symbol("a"), Symbol("b")});
        atl::set_property(nfa, Symbol("test6"));
        add_initial_state(nfa, false);
        add_state(nfa, false);
        add_final_state(nfa, false);
        add_state(nfa, false);

        add_transition(nfa, 0, 0, Symbol("a"), Symbol("0"));
        add_transition(nfa, 0, 1, Symbol("a"), Symbol("1"));
        add_transition(nfa, 0, 3, Symbol("a"), Symbol("3"));
        add_transition(nfa, 1, 2, Symbol("b"), Symbol("2"));
        add_transition(nfa, 2, 2, Symbol("b"), Symbol("2"));
        deterministic_finite_automaton<Symbol, 0, Symbol, bool, Symbol> result;
        minimize(nfa, result);

        deterministic_finite_automaton<Symbol, 0, Symbol, bool, Symbol> expect({Symbol("a"), Symbol("b")});
        atl::set_property(expect, Symbol("test6"));
        add_initial_state(expect, false);
        add_state(expect, false);
        add_final_state(expect, false);
        add_state(expect, false);

        add_transition(expect, 0, 0, Symbol("a"), Symbol("0"));
        add_transition(expect, 0, 1, Symbol("a"), Symbol("1"));
        add_transition(expect, 1, 2, Symbol("b"), Symbol("2"));
        add_transition(expect, 2, 2, Symbol("b"), Symbol("2"));
        std::vector<Symbol> word1({Symbol("a"), Symbol("b"), Symbol("a")});
        std::vector<Symbol> word2({Symbol("a"), Symbol("b"), Symbol("b")});
        return (result == expect) && 
               !accept(nfa, word1) && accept(nfa, word2) &&
               !accept(expect, word1) && accept(expect, word2);
    }

    //test minimize for nondeterministic_finite_automaton<>
    bool test_minimize7() {
        nondeterministic_finite_automaton<> nfa({'a', 'b'});
        add_initial_state(nfa);
        add_state(nfa);
        add_final_state(nfa);
        add_state(nfa);
        add_transition(nfa, 0, 0, char('a'));
        add_transition(nfa, 0, 1, char('a'));
        add_transition(nfa, 1, 2, char('b'));
        add_transition(nfa, 2, 2, char('b'));
        add_transition(nfa, 0, 3, char('b'));
        deterministic_finite_automaton<> expect, result;
        minimize(nfa, result);
        add_initial_state(expect);
        add_state(expect);
        add_final_state(expect);
        add_transition(expect, 0, 1, char('a'));
        add_transition(expect, 1, 1, char('a'));
        add_transition(expect, 1, 2, char('b'));
        add_transition(expect, 2, 2, char('b'));
        return (result == expect) && 
               !accept(nfa, "aaaaaa") && accept(nfa, "aabbbbbbbb") &&
               !accept(expect, "aaaaaa") && accept(expect, "aabbbbbbbb");
    }
}
