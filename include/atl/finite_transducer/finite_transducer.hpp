//
//  finite_transducer.hpp
//  atl 
//
//  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
//  SPDX-License-Identifier: MIT
//  Copyright (c) 2020 Jinlong He.
//

#ifndef atl_finite_transducer_hpp 
#define atl_finite_transducer_hpp

#include <fml/atl/detail/finite_transducer/finite_transducer.hpp>

namespace atl {
    template <class UpperSymbol = char,
              class LowerSymbol = char,
              class LabelProperty = no_type,
              class StateProperty = no_type, 
              class AutomatonProperty = no_type>
    class finite_transducer
        : public detail::finite_transducer_gen<UpperSymbol,
                                               LowerSymbol,
                                               LabelProperty,
                                               StateProperty,
                                               AutomatonProperty> {
        public:
            typedef Symbol symbol_type;
            typedef detail::finite_transducer_gen<UpperSymbol,
                                                  LowerSymbol,
                                                  SymbolProperty,
                                                  StateProperty,
                                                  AutomatonProperty> Base;
            typedef detail::finite_automaton_gen<Symbol,
                                                 SymbolProperty,
                                                 StateProperty,
                                                 AutomatonProperty> fa_type;
            typedef Base nfa_type;
            typedef detail::deterministic_finite_automaton_gen<Symbol,
                                                               SymbolProperty,
                                                               StateProperty,
                                                               AutomatonProperty> dfa_type;

            typedef typename Base::state_property_type state_property_type;
            typedef typename Base::automaton_property_type automaton_property_type;
            typedef typename Base::transition_property_type transition_property_type;

            typedef typename Base::State State;
            typedef typename Base::SymbolSet SymbolSet;
            typedef typename Base::Transition Transition;
            typedef typename Base::StateSetMap StateSetMap;
            typedef typename Base::TransitionMap TransitionMap;
            typedef typename Base::Symbol2StateSetMap Symbol2StateSetMap;

        public:
            finite_transducer()
                : Base() {}

            finite_transducer(const SymbolSet& alphabet)
                : Base(alphabet) {}

            finite_transducer(const std::initializer_list<Symbol>& alphabet)
                : Base(alphabet) {}

            finite_transducer(const finite_transducer& x)
                : Base(x) {}

            finite_transducer(const Base& x)
                : Base(x) {}

            ~finite_transducer() {}

            finite_transducer& 
            operator=(const nfa_type& x) {
                if (&x != this) {
                    Base::operator=(x);
                }
                return *this;
            }

            dfa_type
            operator&(const dfa_type& x) {
                dfa_type out, dfa_lhs, dfa_rhs;
                minimize(*this, dfa_lhs);
                minimize(x, dfa_rhs);
                intersect_fa(dfa_lhs, dfa_rhs, out);
                return out;
            }

            dfa_type
            operator&(const nfa_type& x) {
                dfa_type out, dfa_lhs, dfa_rhs;
                minimize(*this, dfa_lhs);
                minimize(x, dfa_rhs);
                intersect_fa(dfa_lhs, dfa_rhs, out);
                return out;
            }

            dfa_type
            operator|(const dfa_type& x) {
                dfa_type out, dfa_lhs, dfa_rhs;
                minimize(*this, dfa_lhs);
                minimize(x, dfa_rhs);
                union_fa(dfa_lhs, dfa_rhs, out);
                return out;
            }

            dfa_type
            operator|(const nfa_type& x) {
                dfa_type out, dfa_lhs, dfa_rhs;
                minimize(*this, dfa_lhs);
                minimize(x, dfa_rhs);
                union_fa(dfa_lhs, dfa_rhs, out);
                return out;
            }

            dfa_type
            operator-(const dfa_type& x) {
                dfa_type out, dfa_lhs, dfa_rhs, dfa;
                minimize(*this, dfa_lhs);
                minimize(x, dfa_rhs);
                complement_fa(dfa_rhs, dfa);
                intersect_fa(dfa_lhs, dfa, out);
                return out;
            }

            dfa_type
            operator-(const nfa_type& x) {
                dfa_type out, dfa_lhs, dfa_rhs, dfa;
                minimize(*this, dfa_lhs);
                minimize(x, dfa_rhs);
                complement_fa(dfa_rhs, dfa);
                intersect_fa(dfa_lhs, dfa, out);
                return out;
            }

            dfa_type
            operator+(const dfa_type& x) {
                dfa_type out, dfa_lhs, dfa_rhs;
                minimize(*this, dfa_lhs);
                minimize(x, dfa_rhs);
                concat_fa(dfa_lhs, dfa_rhs, out);
                return out;
            }

            dfa_type
            operator+(const nfa_type& x) {
                dfa_type out, dfa_lhs, dfa_rhs;
                minimize(*this, dfa_lhs);
                minimize(x, dfa_rhs);
                concat_fa(dfa_lhs, dfa_rhs, out);
                return out;
            }

            dfa_type
            operator!() {
                dfa_type dfa, out;
                minimize(*this, dfa);
                complement_fa(dfa, out);
                return out;
            }

            bool
            operator==(const dfa_type& x) {
                dfa_type dfa_lhs, dfa_rhs;
                minimize(*this, dfa_lhs);
                minimize(x, dfa_rhs);
                return equal_fa(dfa_lhs, dfa_rhs);
            }

            bool
            operator==(const nfa_type& x) {
                dfa_type dfa_lhs, dfa_rhs;
                minimize(*this, dfa_lhs);
                minimize(x, dfa_rhs);
                return equal_fa(dfa_lhs, dfa_rhs);
            }

        private:
        };
}

#endif /* atl_finite_transducer_hpp */
