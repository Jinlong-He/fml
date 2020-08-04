#include <atl/automaton_utility.hpp>
#include <atl/letter2letter_transducer/deterministic_letter2letter_transducer.hpp>
#include "test.hpp"
using namespace atl;

void test_transducer() {
    deterministic_letter2letter_transducer<char, 0, char> dft;
    auto s0 = add_initial_state(dft);
    auto s1 = add_state(dft);
    auto s2 = add_state(dft);
    auto s3 = add_final_state(dft);
    add_transition(dft, s0, s1, 'a', 'a');
    add_transition(dft, s0, s2, 'a', 'b');
    add_transition(dft, s2, s3, 'b', 'a');
    add_transition(dft, s1, s3, 'd', 'd');
    print_fa(dft);
    deterministic_letter2letter_transducer<char, 0, char> res = (dft&dft);
    print_fa(res);
    cout << endl;
}
