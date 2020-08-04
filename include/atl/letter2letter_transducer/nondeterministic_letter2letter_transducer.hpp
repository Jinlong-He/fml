//
//  nondeterministic_letter2letter_transducer.hpp
//  atl 
//
//  Created by Jinlong He on 2019/10/27.
//  Copyright © 2019年 Ruting-Team. All rights reserved.
//

#ifndef atl_nondeterministic_letter2letter_transducer_hpp 
#define atl_nondeterministic_letter2letter_transducer_hpp

#include "../detail/letter2letter_transducer/nondeterministic_letter2letter_transducer.hpp"
#include "../detail/letter2letter_transducer/deterministic_letter2letter_transducer.hpp"
#include "../detail/finite_automaton/operate.hpp"

namespace atl {
    template <class Symbol = char, 
              long epsilon_ = 0,
              class LabelProperty = no_type,
              class StateProperty = no_type, 
              class AutomatonProperty = no_type>
    class nondeterministic_letter2letter_transducer
        : public detail::nondeterministic_letter2letter_transducer_gen<Symbol, epsilon_,
                                                                       LabelProperty,
                                                                       StateProperty,
                                                                       AutomatonProperty> {
        public:
            typedef Symbol symbol_type;
            typedef detail::nondeterministic_letter2letter_transducer_gen<Symbol, epsilon_,
                                                                          LabelProperty,
                                                                          StateProperty,
                                                                          AutomatonProperty> Base;

            typedef detail::finite_automaton_gen<Symbol, epsilon_,
                                                 LabelProperty,
                                                 StateProperty,
                                                 AutomatonProperty> fa_type;

            typedef typename Base::dfa_type dfa_type;
            typedef typename Base::nfa_type nfa_type;

            typedef detail::deterministic_letter2letter_transducer_gen<Symbol, epsilon_,
                                                                       LabelProperty,
                                                                       StateProperty,
                                                                       AutomatonProperty> dl2lt_type;

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
            nondeterministic_letter2letter_transducer()
                : Base() {}

            nondeterministic_letter2letter_transducer(const SymbolSet& alphabet)
                : Base(alphabet) {}

            nondeterministic_letter2letter_transducer(const std::initializer_list<Symbol>& alphabet)
                : Base(alphabet) {}

            nondeterministic_letter2letter_transducer(const nondeterministic_letter2letter_transducer& x)
                : Base(x) {}

            ~nondeterministic_letter2letter_transducer() {}

            nondeterministic_letter2letter_transducer& 
            operator=(const nondeterministic_letter2letter_transducer& x) {
                if (&x != this) {
                    Base::operator=(x);
                }
                return *this;
            }

            nondeterministic_letter2letter_transducer
            operator&(const nondeterministic_letter2letter_transducer& x) {
                nondeterministic_letter2letter_transducer out, dfa_lhs, dfa_rhs;
                minimize(*this, dfa_lhs);
                minimize(x, dfa_rhs);
                intersect_fa(dfa_lhs, dfa_rhs, out);
                return out;
            }

            nondeterministic_letter2letter_transducer
            operator&(const nfa_type& x) {
                nondeterministic_letter2letter_transducer out, dfa_lhs, dfa_rhs;
                minimize(*this, dfa_lhs);
                minimize(x, dfa_rhs);
                intersect_fa(dfa_lhs, dfa_rhs, out);
                return out;
            }
            
            nondeterministic_letter2letter_transducer
            operator|(const nondeterministic_letter2letter_transducer& x) {
                nondeterministic_letter2letter_transducer out, dfa_lhs, dfa_rhs;
                minimize(*this, dfa_lhs);
                minimize(x, dfa_rhs);
                union_fa(dfa_lhs, dfa_rhs, out);
                return out;
            }

            nondeterministic_letter2letter_transducer
            operator|(const nfa_type& x) {
                nondeterministic_letter2letter_transducer out, dfa_lhs, dfa_rhs;
                minimize(*this, dfa_lhs);
                minimize(x, dfa_rhs);
                union_fa(dfa_lhs, dfa_rhs, out);
                return out;
            }

            nondeterministic_letter2letter_transducer
            operator-(const nondeterministic_letter2letter_transducer& x) {
                nondeterministic_letter2letter_transducer out, dfa_lhs, dfa_rhs, dfa;
                minimize(*this, dfa_lhs);
                minimize(x, dfa_rhs);
                complement_fa(dfa_rhs, dfa);
                intersect_fa(dfa_lhs, dfa, out);
                return out;
            }

            nondeterministic_letter2letter_transducer
            operator-(const nfa_type& x) {
                nondeterministic_letter2letter_transducer out, dfa_lhs, dfa_rhs, dfa;
                minimize(*this, dfa_lhs);
                minimize(x, dfa_rhs);
                complement_fa(dfa_rhs, dfa);
                intersect_fa(dfa_lhs, dfa, out);
                return out;
            }
            
            nondeterministic_letter2letter_transducer
            operator+(const nondeterministic_letter2letter_transducer& x) {
                nondeterministic_letter2letter_transducer out, dfa_lhs, dfa_rhs;
                minimize(*this, dfa_lhs);
                minimize(x, dfa_rhs);
                concat_fa(dfa_lhs, dfa_rhs, out);
                return out;
            }

            nondeterministic_letter2letter_transducer
            operator+(const nfa_type& x) {
                nondeterministic_letter2letter_transducer out, dfa_lhs, dfa_rhs;
                minimize(*this, dfa_lhs);
                minimize(x, dfa_rhs);
                concat_fa(dfa_lhs, dfa_rhs, out);
                return out;
            }

            nondeterministic_letter2letter_transducer
            operator!() {
                nondeterministic_letter2letter_transducer dfa, out;
                minimize(*this, dfa);
                complement_fa(dfa, out);
                return out;
            }

            bool
            operator==(const nondeterministic_letter2letter_transducer& x) {
                nondeterministic_letter2letter_transducer dfa_lhs, dfa_rhs;
                minimize(*this, dfa_lhs);
                minimize(x, dfa_rhs);
                return equal_fa(dfa_lhs, dfa_rhs);
            }

            bool
            operator==(const nfa_type& x) {
                nondeterministic_letter2letter_transducer dfa_lhs, dfa_rhs;
                minimize(*this, dfa_lhs);
                minimize(x, dfa_rhs);
                return equal_fa(dfa_lhs, dfa_rhs);
            }
        private:
        };
}

#endif /* atl_nondeterministic_letter2letter_transducer_hpp */
