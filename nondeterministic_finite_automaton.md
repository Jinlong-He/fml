---
```
void
minimize(const NFA& a_in,
         DFA& a_out)
```
Minimizes nondeterministic finite automaton `a_in` to `a_out`.

---
```
void
minimize(const NFA& a_in,
         DFA& a_out,
         SymbolPropertyMerge symbol_property_merge)
```
Minimizes nondeterministic finite automaton `a_in` to `a_out` with merging symbol property by `symbol_property_merge`.

---
```
void
minimize(const NFA& a_in,
         DFA& a_out,
         SymbolPropertyMerge symbol_property_merge,
         StatePropertyMerge state_property_merge)
```
Minimizes nondeterministic finite automaton `a_in` to `a_out` with merging symbol property by `symbol_property_merge`, merging state property be `state_property_merge`.


