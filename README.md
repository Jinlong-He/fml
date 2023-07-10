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
- boost is required. It is recommended to install boost to your system.
- fml is based on c++17, your compiler need to support.
- fml is a Header-Only libraries, there is nothing to build.

Before using our library, please install the headers files by the following commands.

```sh
mkdir build
cd build
cmkae ..
sudo make install
```

Then, you can include files directly.


---

# Build a Simple Program Using fml
The following program builds a deterministic finite automaton and outputs it.
```
#include <fml/atl/finite_automaton/deterministic_finite_automaton.hpp>
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
g++ -I path/to/boost -I path/to/fml/include -std=c++17 example.cpp -o example
```

Or just compile if you install our library into system.
---
# Data Structures
The fml currently provides five automaton classes:
- [deterministic_finite_automaton](https://github.com/Jinlong-He/fml/blob/master/deterministic_finite_automaton.md)
- nondeterministic_finite_automaton
- push_down_system
- deterministic_letter2letter_transducer
- nondeterministic_letter2letter_transducer

