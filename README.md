# Welcome to fml!
---
#### fml is a C++ template library for fomal method: automaton, regular expression, logic.
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
# Data Structure 

#####deterministic_finite_automaton< Symbol, epsilon, SymbolProperty, StateProperty, AutomatonProperty >

#####nondeterministic_finite_automaton< Symbol, epsilon, SymbolProperty, StateProperty, AutomatonProperty >

#####push_down_system< Symbol, epsilon, Symbolproperty, StateProperty, AutomatonProperty >

#####deterministic_letter2letter_transducer< Symbol, epsilon, LabelProperty, StateProperty, AutomatonProperty >

#####nondeterministic_letter2letter_transducer< Symbol, epsilon, LabelProperty, StateProperty, AutomatonProperty >

|||
|-|-|
|Symbol|the symbol type of the alphabet, *default char*|
|epsilon|the epsilon symbol, *default 0*|
|SymbolProperty|the property of symbol in an edge, *default no_type*|
|LabelProperty|the property of label in an edge, *default no_type*|
|StateProperty|the property of state, *default no_type*|
|AutomatonProperty|the property of automaton, *default no_type*|


