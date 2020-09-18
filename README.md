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
Copy the text of this program into a file called example.cpp.
Now, in the directory where you saved example.cpp, issue the following command:
```
c++ -I path/to/boost -I path/to/fml/include -std=c++17 example.cpp -o example
```
---
# Data Structures
The fml currently provides five automaton classes:
- deterministic_finite_automaton
- nondeterministic_finite_automaton
- push_down_system
- deterministic_letter2letter_transducer
- nondeterministic_letter2letter_transducer

### deterministic_finite_automaton < Symbol, epsilon, SymbolProperty, StateProperty, AutomatonProperty >
### nondeterministic_finite_automaton< Symbol, epsilon, SymbolProperty, StateProperty, AutomatonProperty >
The deterministic_finite_automaton and nondeterministic_finite_automaton classes implements two generalized finite automaton structures.

The `Symbol` template parameter of the `deterministic_finite_automaton` and
`nondeterministic_finite_automaton` classes controls what kind of type is used to represent the input symbol.
The `epsilon` template parameter of the `deterministic_finite_automaton` and
`nondeterministic_finite_automaton` classes controls what kind of value is used to represent the empty input.

The `SymbolProperty`, `StateProperty` and `AutomatonProperty` template parameter of the `deterministic_finite_automaton` and
`nondeterministic_finite_automaton` classes control what kind of type is used to represent the property of the input symbol, state and automaton.

|Parameter|Description|Default|
|-|-|-|
|Symbol| the selector for type used to represent the input symbol |char|
|epsilon|the selector for value used to represent the empty inpu |0L|
|SymbolProperty|the selector for type used to represent the property of the input symbol |no_type|
|StateProperty|the selector for type used to represent the property of the state |no_type|
|AutomatonProperty|the selector for type used to represent the property of the automaton |no_type|

#### Associated Types
---
`deterministic_finite_automaton::State`

The type for the state descreptors associated with the `deterministic_finite_automaton`

---

#### Non-Member Function
##### Structure Access
---
```
const boost::unordered_set<Symbol>&
alphabet(const finite_automaton& fa)
```
Returns a set providing access to the input symbol alphabet of finite automaton `fa`.

---
```
State
initial_state(const finite_automaton& fa)
```
Returns a set providing access to the input symbol alphabet of finite automaton `fa`.

---



### push_down_system< Symbol, epsilon, Symbolproperty, StateProperty, AutomatonProperty >
### deterministic_letter2letter_transducer< Symbol, epsilon, LabelProperty, StateProperty, AutomatonProperty >
### nondeterministic_letter2letter_transducer< Symbol, epsilon, LabelProperty, StateProperty, AutomatonProperty >

|||
|-|-|
|Symbol|the symbol type of the alphabet, *default char*|
|epsilon|the epsilon symbol, *default 0*|
|SymbolProperty|the property of symbol in an edge, *default no_type*|
|LabelProperty|the property of label in an edge, *default no_type*|
|StateProperty|the property of state, *default no_type*|
|AutomatonProperty|the property of automaton, *default no_type*|


