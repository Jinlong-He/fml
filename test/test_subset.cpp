#include <fml/atl/finite_automaton/nondeterministic_finite_automaton.hpp>
#include <fml/atl/finite_automaton/deterministic_finite_automaton.hpp>
#include <fml/atl/letter2letter_transducer/deterministic_letter2letter_transducer.hpp> 
#include <fml/atl/letter2letter_transducer/nondeterministic_letter2letter_transducer.hpp>
#include "test.hpp"
using namespace atl;

namespace test {
    //test minimize for nondeterministic_finite_automaton<char>
    bool test_subset1() {
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
        return (result <= expect) && (expect <= result) &&
               !accept(nfa, "a") && accept(nfa, "abbbbbbb") &&
               !accept(expect, "a") && accept(expect, "abbbbbbb");
    }

    bool test_subset2() {
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
        add_final_state(expect);
        add_transition(expect, 0, 1, Symbol("a"));
        std::vector<Symbol> word1({Symbol("b"), Symbol("b"), Symbol("b")});
        std::vector<Symbol> word2({Symbol("a")});
        return (result <= expect) && !(expect <= result) &&
               !accept(nfa, word1) && accept(nfa, word2) &&
               !accept(expect, word1) && accept(expect, word2);
    }

    bool test_subset3() {
        nondeterministic_letter2letter_transducer<char> t1({'a','b'});
        add_initial_state(t1);
        add_state(t1);
        add_final_state(t1);
        add_transition(t1,0,0,'b','b');
        add_transition(t1,0,1,'a','b');
        add_transition(t1,1,1,'a','b');
        add_transition(t1,1,1,'b','b');
        add_transition(t1,1,2,'b','b');
        add_transition(t1,2,2,'a','a');
        add_transition(t1,2,2,'b','b');
        deterministic_letter2letter_transducer t2 = (t1 * t1);
        return (t2 <= t1);
    }

}
