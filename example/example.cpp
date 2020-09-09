#include <atl/finite_automaton/deterministic_finite_automaton.hpp>
using namespace atl;

int main() {
    deterministic_finite_automaton<> dfa;
    add_initial_state(dfa);
    add_state(dfa);
    add_final_state(dfa);
    add_transition(dfa, 0, 1, 'a');
    add_transition(dfa, 1, 2, 'b');
    print_fa(dfa);
}
