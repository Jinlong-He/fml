
# deterministic_finite_automaton < Symbol, epsilon, SymbolProperty, StateProperty, AutomatonProperty >
The deterministic_finite_automaton class implements a generalized deterministic finite automaton structure.

The `Symbol` template parameter of the `deterministic_finite_automaton` class controls what kind of type is used to represent the input symbol.
The `epsilon` template parameter of the `deterministic_finite_automaton` class controls what kind of value is used to represent the empty input.

The `SymbolProperty`, `StateProperty` and `AutomatonProperty` template parameter of the `deterministic_finite_automaton` class control what kind of type is used to represent the property of the input symbol, state and automaton.

|Parameter|Description|Default|
|-|-|-|
|Symbol| the selector for type used to represent the input symbol |char|
|epsilon|the selector for value used to represent the empty inpu |0L|
|SymbolProperty|the selector for type used to represent the property of the input symbol |no_type|
|StateProperty|the selector for type used to represent the property of the state |no_type|
|AutomatonProperty|the selector for type used to represent the property of the automaton |no_type|

## Associated Types
---
`deterministic_finite_automaton<Symbol, epsilon, SymbolProperty, StateProperty, AutomatonProperty>::State`

The type for the state descreptors associated with the `finite_automaton`

---
`deterministic_finite_automaton<Symbol, epsilon, SymbolProperty, StateProperty, AutomatonProperty>::Transition`

The type for the transition descreptors associated with the `finite_automaton`

---
`deterministic_finite_automaton<Symbol, epsilon, SymbolProperty, StateProperty, AutomatonProperty>::TransitionMap`

The type for the transition map descreptors associated with the `deterministic_finite_automaton`

---
`deterministic_finite_automaton<Symbol, epsilon, SymbolProperty, StateProperty, AutomatonProperty>::transition_property_type`

The type for the transition property descreptors associated with the `deterministic_finite_automaton`, if `SymbolProperty` is `no_type` then this type is `Symbol`, otherwise this type is `Property<Symbol, SymbolProperty>`

---
`deterministic_finite_automaton<Symbol, epsilon, SymbolProperty, StateProperty, AutomatonProperty>::StateSet`

The type for the set of state associated with the `deterministic_finite_automaton`

---
`deterministic_finite_automaton<Symbol, epsilon, SymbolProperty, StateProperty, AutomatonProperty>::SymbolSet`

The type for the set of symbol associated with the `deterministic_finite_automaton`

---
`deterministic_finite_automaton<Symbol, epsilon, SymbolProperty, StateProperty, AutomatonProperty>::StateIter`

The type for the iterators returned by `states()`

---
`deterministic_finite_automaton<Symbol, epsilon, SymbolProperty, StateProperty, AutomatonProperty>::TransitionIter`

The type for the iterators returned by `transitions()`

---
`deterministic_finite_automaton<Symbol, epsilon, SymbolProperty, StateProperty, AutomatonProperty>::InTransitionIter`

The type for the iterators returned by `in_transitions()`

---
`deterministic_finite_automaton<Symbol, epsilon, SymbolProperty, StateProperty, AutomatonProperty>::OutTransitionIter`

The type for the iterators returned by `out_transitions()`

---
## Member Function
---
```
deterministic_finite_automaton_gen()
```
Default constructor.Creates an empty deterministic finite automaton object with zero states ,zero transitions and empty alphabet.

---
```
deterministic_finite_automaton_gen(const SymbolSet& alphabet)
```
Creates a deterministic finite automaton object with zero states ,zero transitions and `alphabet`.

---
```
deterministic_finite_automaton_gen(const std::initializer_list<Symbol>& alphabet)
```
Creates a deterministic finite automaton object with zero states ,zero transitions and `alphabet`.

---
```
deterministic_finite_automaton_gen(const deterministic_finite_automaton_gen& x)
```
Copy constructor. Creates a new deterministic finite automaton that is a copy of deterministic finite automaton `x`.

---
```
deterministic_finite_automaton_gen& 
operator=(const deterministic_finite_automaton_gen& x)
```
Assignment operator. Makes this deterministic finite automaton a copy of deterministic finite automaton `x`.

---
```
deterministic_finite_automaton
operator&(const deterministic_finite_automaton& x)
```
And operator. Returns the intersection of this object and deterministic finite automaton `x`.

---
```
deterministic_finite_automaton
operator&(const nondeterministic_finite_automaton& x)
```
And operator. Returns the intersection of this object and nondeterministic finite automaton `x`.

---
```
deterministic_finite_automaton
operator|(const deterministic_finite_automaton& x)
```
Or operator. Returns the union of this object and deterministic finite automaton `x`.

---
```
deterministic_finite_automaton
operator|(const nondeterministic_finite_automaton& x)
```
And operator. Returns the union of this object and nondeterministic finite automaton `x`.

---
```
deterministic_finite_automaton
operator-(const deterministic_finite_automaton& x)
```
Minus operator. Returns the difference of deterministic finite automaton `x` from this object.

---
```
deterministic_finite_automaton
operator-(const nondeterministic_finite_automaton& x)
```
Minus operator. Returns the difference of nondeterministic finite automaton `x` from this object.

---
```
deterministic_finite_automaton
operator+(const deterministic_finite_automaton& x)
```
Plus operator. Returns the concatenation of this object and deterministic finite automaton `x`.

---
```
deterministic_finite_automaton
operator+(const nondeterministic_finite_automaton& x)
```
Plus operator. Returns the concatenation of this object and nondeterministic finite automaton `x`.

---
```
deterministic_finite_automaton
operator!()
```
Boolean negation operator. Returns the complementation of this object.

---
```
bool
operator==(const deterministic_finite_automaton& x)
```
Equality operator. Compares the equality of this object and deterministic finite automaon `x`.

---
```
bool
operator==(const nondeterministic_finite_automaton& x)
```
Equality operator. Compares the equality of this object and nondeterministic finite automaon `x`.

---
## Non-Member Function
### Structure Access
---
```
const SymbolSet&
alphabet(const deterministic_finite_automaton& dfa)
```
Returns a set providing access to the input symbol alphabet of deterministic finite automaton `dfa`.

---
```
State
initial_state(const deterministic_finite_automaton& dfa)
```
Returns a set providing access to the input symbol alphabet of deterministic finite automaton `dfa`.

---
```
const StateSet&
state_set(const deterministic_finite_automaton& dfa)
```
Returns a set providing access to the states of deterministic finite automaton `dfa`.

---
```
const StateSet&
final_state_set(const deterministic_finite_automaton& fa)
```
Returns a set providing access to the final states of deterministic finite automaton `dfa`.

---
```
const TransitionMap&
transition_map(const deterministic_finite_automaton& dfa)
```
Returns a map providing access to the transition map of deterministic finite automaton `dfa`.

---
```
const Symbol&
epsilon(const deterministic_finite_automaton& dfa)
```
Returns a symbol providing access to the empty input of deterministic finite automaton `dfa`.

---
```
const transition_property_type&
epsilon_transition(const deterministic_finite_automaton& dfa)
```
Returns a transition property providing access to the empty input transition of deterministic finite automaton `dfa`.

---
```
std::pair<StateIter, StateIter>&
states(const deterministic_finite_automaton& dfa)
```
Returns a iterator-range providing access to the vertex set of deterministic finite automaton `dfa`.

---
```
std::pair<TransitionIter, TransitionIter>&
transitions(const deterministic_finite_automaton& dfa)
```
Returns a iterator-range providing access to the transition set of deterministic finite automaton `dfa`.

---
```
std::pair<InTransitionIter, InTransitionIter>&
in_transitions(const deterministic_finite_automaton& dfa,
               State s)
```
Returns a iterator-range providing access to the in-transitions of state `s` in deterministic finite automaton `dfa`.

---
```
std::pair<OutTransitionIter, OutTransitionIter>&
out_transitions(const deterministic_finite_automaton& dfa,
                State s)
```
Returns a iterator-range providing access to the out-transitions of state `s` in deterministic finite automaton `dfa`.

---
```
std::pair<OutTransitionIter, OutTransitionIter>&
out_transitions(const deterministic_finite_automaton& dfa,
                State s)
```
Returns a iterator-range providing access to the out-transitions of state `s` in deterministic finite automaton `dfa`.

---
```
State
source(const deterministic_finite_automaton& dfa,
       Transition t)
```
Returns the source state of transition `t`.

---
```
State
target(const deterministic_finite_automaton& dfa,
       Transition t)
```
Returns the target state of transition `t`.

---
### Structure Modification 
---
```
State
add_state(deterministic_finite_automaton& dfa)
```
Adds a state to the deterministic finite automaton and returns the state descriptor for the new state.

---
```
State
add_state(deterministic_finite_automaton& dfa,
          const StateProperty& p)
```
Adds a state to the deterministic finite automaton with the property `p` and returns the state descriptor for the new state.

---
```
State
add_initial_state(deterministic_finite_automaton& dfa)
```
Adds a initial state to the deterministic finite automaton and returns the state descriptor for the new state.

---
```
State
add_initial_state(deterministic_finite_automaton& dfa,
          const StateProperty& p)
```
Adds a initial state to the deterministic finite automaton with the property `p` and returns the state descriptor for the new state.

---
```
State
add_final_state(deterministic_finite_automaton& dfa)
```
Adds a final state to the deterministic finite automaton and returns the state descriptor for the new state.

---
```
State
add_final_state(deterministic_finite_automaton& dfa,
          const StateProperty& p)
```
Adds a final state to the deterministic finite automaton with the property `p` and returns the state descriptor for the new state.

---
```
std::pair<Transition, bool>
add_transition(deterministic_finite_automaton& dfa,
               State s, State t,
               const Symbol& c)
```
Adds transition `(s,c,t)` to the deterministic finite automaton and returns the transition descriptor for the new transition, if the transition is already in the deterministic finite automaton then duplicate will not be added and the `bool` flag will be `false`.

---
```
std::pair<Transition, bool>
add_transition(deterministic_finite_automaton& dfa,
               State s, State t,
               const Symbol& c,
               const SymbolProperty& p)
```
Adds transition `(s,c,t)` to the deterministic finite automaton and attaches `p` as the property. Also see the previous `add_transition()` non-menber function for more details.

---
```
void
set_alphabet(deterministic_finite_automaton& dfa,
             const SymbolSet& set)
```
Sets `set` as the alphabet of deterministic finite automaton.

---
```
void
set_initial_state(deterministic_finite_automaton& dfa,
                  State state)
```
Sets `state` as the initial state of deterministic finite automaton.

---
```
void
set_final_state(deterministic_finite_automaton& dfa,
                State state)
```
Sets `state` as the final state of deterministic finite automaton.

---
```
void
remove_final_state(deterministic_finite_automaton& dfa,
                   State state)
```
Removes `state` from final states of deterministic finite automaton.

---
```
void
clear_finale_state_set(deterministic_finite_automaton& dfa)
```
Clears final states of deterministic finite automaton.

---
### Property Access
---
```
const AutomatonProperty&
get_property(const deterministic_finite_automaton& dfa)
```
Returns the property of deterministic finite automaton `dfa`.

---
```
void
set_property(deterministic_finite_automaton& dfa,
             const AutomatonProperty& p)
```
Sets `p` as the property of deterministic finite automaton `dfa`.

---
```
const StateProperty&
get_property(const deterministic_finite_automaton& dfa,
             State s)
```
Returns the property of state `s` in deterministic finite automaton `dfa`.

---
```
void
set_property(deterministic_finite_automaton& dfa,
             State s,
             const StateProperty& p)
```
Sets `p` as the property of state `s` in deterministic finite automaton `dfa`.

---
```
const StateProperty&
get_property(const deterministic_finite_automaton& dfa,
             Transition t)
```
Returns the property of transition `t` in deterministic finite automaton `dfa`.

---
```
void
set_property(deterministic_finite_automaton& dfa,
             Transition t,
             const StateProperty& p)
```
Sets `p` as the property of transition `t` in deterministic finite automaton `dfa`.

---
### Finite Automaton Algorithm
---
```
void
reachable_closure(const DFA& dfa,
                  const StateSet& states_in,
                  StateSet& states_out,
                  detail::Direction direction = detail::forward)
```
Sets `states_out` as the reachable closure from the state set `states_in` in deterministic finite automaton `dfa` with `direction`.

---
```
void
reachable_closure(const DFA& dfa,
                  StateSet& states_out)
```
Sets `states_out` as the reachable closure in deterministic finite automaton `dfa`.

---
```
void
minimize(const DFA& a_in,
         DFA& a_out)
```
Minimizes deterministic finite automaton `a_in` to `a_out`.

---
```
void
intersect_fa(const DFA& a_lhs,
             const DFA& a_rhs,
             DFA& a_out)
```
Intersects deterministic finite automaton `a_lhs` and deterministic finite automaton `a_rhs` to `a_out`.

---
```
void
intersect_fa(const DFA& a_lhs,
             const DFA& a_rhs,
             DFA& a_out,
             SymbolPropertyMerge symbol_property_merge)
```
Intersects deterministic finite automaton `a_lhs` and deterministic finite automaton `a_rhs` with merging symbol property by `symbol_property_merge` to `a_out`. 
If `SymbolProperty` is not `no_type`, and `(s1,c{p1},t1)` in `a_lhs` and `(s2,c{p2},t2)` in `a_rhs`, then `(<s1,s2>,c{symbol_property_merge(p1,p2)},<t1,t2>)` in `a_out`.

---
```
void
intersect_fa(const DFA& a_lhs,
             const DFA& a_rhs,
             DFA& a_out,
             SymbolPropertyMerge symbol_property_merge,
             StatePropertyMerge state_property_merge)
```
Intersects deterministic finite automaton `a_lhs` and deterministic finite automaton `a_rhs` with merging symbol property by `symbol_property_merge` and merging state property bt `state_property_merge` to `a_out`. 
If `SymbolProperty` is not `no_type`, and `(s1{sp1},c{p1},t1{tp1})` in `a_lhs` and `(s2{sp2},c{p2},t2{tp2})` in `a_rhs`, then `(<s1,s2>{sp},c{symbol_property_merge(p1,p2)},<t1,t2>{tp})` in `a_out`, where `sp` is `state_property_merge(sp1,sp2)`.


---
```
void
intersect_fa(const DFA& a_lhs,
             const DFA& a_rhs,
             DFA& a_out,
             SymbolPropertyMerge symbol_property_merge,
             StatePropertyMerge state_property_merge,
             AutomatonPropertyMerge automaton_property_merge)
```
Intersects deterministic finite automaton `a_lhs` and deterministic finite automaton `a_rhs` with merging symbol property by `symbol_property_merge` and merging state property bt `state_property_merge` and merging automaton property by `automaton_property_merge` to `a_out`. 
Also see `intersect_fa()` for more details.

---
