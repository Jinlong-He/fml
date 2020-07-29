#include <atl/regular_expression/algorithm.hpp>
#include <atl/automaton_utility.hpp>
using namespace atl;
int main(int argc, char* argv[]) {
    basic_regular_expression<> regEx(argv[1]);
    deterministic_finite_automaton<> dfa;
    make_fa(regEx, dfa);
    print_fa(dfa);
}
