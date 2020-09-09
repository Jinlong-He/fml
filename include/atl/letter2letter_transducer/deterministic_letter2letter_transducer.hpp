//
//  deterministic_letter2letter_transducer.hpp
//  atl 
//
//  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
//  SPDX-License-Identifier: MIT
//  Copyright (c) 2020 Jinlong He.
//

#ifndef atl_deterministic_letter2letter_transducer_hpp 
#define atl_deterministic_letter2letter_transducer_hpp

#include <atl/detail/letter2letter_transducer/deterministic_letter2letter_transducer.hpp>
#include <atl/detail/letter2letter_transducer/nondeterministic_letter2letter_transducer.hpp>
#include <atl/detail/finite_automaton/operate.hpp>
#include <atl/detail/letter2letter_transducer/operate.hpp>

namespace atl {
    template <class Symbol = char, 
              long epsilon_ = 0,
              class LabelProperty = no_type,
              class StateProperty = no_type, 
              class AutomatonProperty = no_type>
    class deterministic_letter2letter_transducer
        : public detail::deterministic_letter2letter_transducer_gen<Symbol, epsilon_,
                                                                    LabelProperty,
                                                                    StateProperty,
                                                                    AutomatonProperty> {
    public:
        typedef Symbol symbol_type;
        typedef detail::deterministic_letter2letter_transducer_gen<Symbol, epsilon_,
                                                                   LabelProperty,
                                                                   StateProperty,
                                                                   AutomatonProperty> Base;

        typedef detail::finite_automaton_gen<Symbol, epsilon_,
                                             LabelProperty,
                                             StateProperty,
                                             AutomatonProperty> fa_type;

        typedef typename Base::dfa_type dfa_type;
        typedef typename Base::nfa_type nfa_type;

        typedef detail::nondeterministic_letter2letter_transducer_gen<Symbol, epsilon_,
                                                                      LabelProperty,
                                                                      StateProperty,
                                                                      AutomatonProperty> nl2lt_type;

        typedef typename Base::state_property_type state_property_type;
        typedef typename Base::automaton_property_type automaton_property_type;
        typedef typename Base::transition_property_type transition_property_type;

        typedef typename Base::State State;
        typedef typename Base::StateSet StateSet;
        typedef typename Base::StatePair StatePair;
        typedef typename Base::SymbolSet SymbolSet;
        typedef typename Base::Transition Transition;
        typedef typename Base::TransitionMap TransitionMap;
        typedef typename Base::Symbol2StateMap Symbol2StateMap;
        typedef typename Base::Symbol2StatePairMap Symbol2StatePairMap;

    public:
        deterministic_letter2letter_transducer()
            : Base() {}

        deterministic_letter2letter_transducer(const SymbolSet& alphabet)
            : Base(alphabet) {}

        deterministic_letter2letter_transducer(const std::initializer_list<Symbol>& alphabet)
            : Base(alphabet) {}

        deterministic_letter2letter_transducer(const deterministic_letter2letter_transducer& x)
            : Base(x) {}

        ~deterministic_letter2letter_transducer() {}

        deterministic_letter2letter_transducer& 
        operator=(const deterministic_letter2letter_transducer& x) {
            if (&x != this) {
                Base::operator=(x);
            }
            return *this;
        }

        deterministic_letter2letter_transducer
        operator&(const deterministic_letter2letter_transducer& x) {
            deterministic_letter2letter_transducer out, dfa_lhs, dfa_rhs;
            minimize(*this, dfa_lhs);
            minimize(x, dfa_rhs);
            intersect_fa(dfa_lhs, dfa_rhs, out);
            return out;
        }

        deterministic_letter2letter_transducer
        operator&(const nfa_type& x) {
            deterministic_letter2letter_transducer out, dfa_lhs, dfa_rhs;
            minimize(*this, dfa_lhs);
            minimize(x, dfa_rhs);
            intersect_fa(dfa_lhs, dfa_rhs, out);
            return out;
        }
        
        deterministic_letter2letter_transducer
        operator|(const deterministic_letter2letter_transducer& x) {
            deterministic_letter2letter_transducer out, dfa_lhs, dfa_rhs;
            minimize(*this, dfa_lhs);
            minimize(x, dfa_rhs);
            union_fa(dfa_lhs, dfa_rhs, out);
            return out;
        }

        deterministic_letter2letter_transducer
        operator|(const nfa_type& x) {
            deterministic_letter2letter_transducer out, dfa_lhs, dfa_rhs;
            minimize(*this, dfa_lhs);
            minimize(x, dfa_rhs);
            union_fa(dfa_lhs, dfa_rhs, out);
            return out;
        }

        deterministic_letter2letter_transducer
        operator-(const deterministic_letter2letter_transducer& x) {
            deterministic_letter2letter_transducer out, dfa_lhs, dfa_rhs, dfa;
            minimize(*this, dfa_lhs);
            minimize(x, dfa_rhs);
            complement_fa(dfa_rhs, dfa);
            intersect_fa(dfa_lhs, dfa, out);
            return out;
        }

        deterministic_letter2letter_transducer
        operator-(const nfa_type& x) {
            deterministic_letter2letter_transducer out, dfa_lhs, dfa_rhs, dfa;
            minimize(*this, dfa_lhs);
            minimize(x, dfa_rhs);
            complement_fa(dfa_rhs, dfa);
            intersect_fa(dfa_lhs, dfa, out);
            return out;
        }
        
        deterministic_letter2letter_transducer
        operator+(const deterministic_letter2letter_transducer& x) {
            deterministic_letter2letter_transducer out, dfa_lhs, dfa_rhs;
            minimize(*this, dfa_lhs);
            minimize(x, dfa_rhs);
            concat_fa(dfa_lhs, dfa_rhs, out);
            return out;
        }

        deterministic_letter2letter_transducer
        operator+(const nfa_type& x) {
            deterministic_letter2letter_transducer out, dfa_lhs, dfa_rhs;
            minimize(*this, dfa_lhs);
            minimize(x, dfa_rhs);
            concat_fa(dfa_lhs, dfa_rhs, out);
            return out;
        }

        deterministic_letter2letter_transducer
        operator!() {
            deterministic_letter2letter_transducer dfa, out;
            minimize(*this, dfa);
            complement_fa(dfa, out);
            return out;
        }

        bool
        operator==(const deterministic_letter2letter_transducer& x) {
            deterministic_letter2letter_transducer dfa_lhs, dfa_rhs;
            minimize(*this, dfa_lhs);
            minimize(x, dfa_rhs);
            return equal_fa(dfa_lhs, dfa_rhs);
        }

        bool
        operator==(const nfa_type& x) {
            deterministic_letter2letter_transducer dfa_lhs, dfa_rhs;
            minimize(*this, dfa_lhs);
            minimize(x, dfa_rhs);
            return equal_fa(dfa_lhs, dfa_rhs);
        }

        deterministic_letter2letter_transducer
        operator*(const deterministic_letter2letter_transducer& x) {
            deterministic_letter2letter_transducer out, dl2lt_lhs, dl2lt_rhs, dl2lt;
            minimize(*this, dl2lt_lhs);
            minimize(x, dl2lt_rhs);
            composite_l2lt(dl2lt_lhs, dl2lt_rhs, out);
            return out;
        }
        
        deterministic_letter2letter_transducer
        operator*(const nl2lt_type& x) {
            deterministic_letter2letter_transducer out, dl2lt_lhs, dl2lt_rhs, dl2lt;
            minimize(*this, dl2lt_lhs);
            minimize(x, dl2lt_rhs);
            composite_l2lt(dl2lt_lhs, dl2lt_rhs, out);
        }
    private:
    };
}

#endif /* atl_deterministic_letter2letter_transducer_hpp */
