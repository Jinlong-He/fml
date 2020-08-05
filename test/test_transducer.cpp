#include <atl/automaton_utility.hpp>
#include <atl/letter2letter_transducer/deterministic_letter2letter_transducer.hpp>
#include "test.hpp"
using namespace atl;

void test_transducer() {
    deterministic_letter2letter_transducer<> dft1;
    deterministic_letter2letter_transducer<> dft2;
    auto s0 = add_initial_state(dft1);
    auto s1 = add_state(dft1);
    auto s2 = add_state(dft1);
    auto s3 = add_final_state(dft1);
    //add_transition(dft1, s0, s1, 'a', 'a');
    add_transition(dft1, s0, s1, 0, 'a');
    add_transition(dft1, s1, s1, 'a', 'a');
    add_transition(dft1, s1, s3, 'd', 'd');

    add_initial_state(dft2);
    add_state(dft2);
    add_state(dft2);
    add_final_state(dft2);
    add_transition(dft2, s0, s1, 'a', 'b');
    add_transition(dft2, s1, s3, 'd', 'b');

    //print_fa((dft1 & dft2));
    print_fa((dft1 * dft2));
    cout << endl;
}
