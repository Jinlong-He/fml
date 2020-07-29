#include "../../atl/regular_expression/algorithm.hpp"
#include "../../atl/automaton_utility.hpp"
using namespace atl;
int main(int argc, char* argv[]) {
    deterministic_finite_automaton<> dfa;
    make_fa(basic_regular_expression<>(argv[1]), dfa);
    print_fa(dfa);
}
