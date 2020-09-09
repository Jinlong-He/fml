# Welcome to fml!
---
#### fml is a C++ template library for formal method: automaton, regular expression, logic.
---
# Overview
The fml includes:
- atl: automaton template library, a library for:
    - finite automaton
    - push down system
    - letter to letter finite transducer
    - regular expression
- ll: logic library, a library for:
    - propositional logic
    - computation tree logic
    - linear temporal logic
---
# How to build 
- To compile in fml, you need a directory containing the fml/include subdirectory and boost/ subdirectory in your #include path.
- fml is based on c++17, your compiler need to support.
- fml is a Header-Only libraries, there is nothing to build.
---

# Build a Simple Program Using fml
The following program builds a deterministic finite automaton and outputs it.
```
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
```
---
# Data Structure 

##### deterministic_finite_automaton< Symbol, epsilon, SymbolProperty, StateProperty, AutomatonProperty >
##### nondeterministic_finite_automaton< Symbol, epsilon, SymbolProperty, StateProperty, AutomatonProperty >
##### push_down_system< Symbol, epsilon, Symbolproperty, StateProperty, AutomatonProperty >
##### deterministic_letter2letter_transducer< Symbol, epsilon, LabelProperty, StateProperty, AutomatonProperty >
##### nondeterministic_letter2letter_transducer< Symbol, epsilon, LabelProperty, StateProperty, AutomatonProperty >

|||
|-|-|
|Symbol|the symbol type of the alphabet, *default char*|
|epsilon|the epsilon symbol, *default 0*|
|SymbolProperty|the property of symbol in an edge, *default no_type*|
|LabelProperty|the property of label in an edge, *default no_type*|
|StateProperty|the property of state, *default no_type*|
|AutomatonProperty|the property of automaton, *default no_type*|


