//
//  deterministic_finite_automaton.hpp
//  atl 
//
//  Created by Jinlong He on 2019/10/27.
//  Copyright © 2019年 Ruting-Team. All rights reserved.
//

#ifndef atl_deterministic_finite_automaton_hpp 
#define atl_deterministic_finite_automaton_hpp

#include "../detail/finite_automaton/deterministic_finite_automaton.hpp"
#include "../detail/finite_automaton/operate.hpp"
#include "../detail/finite_automaton/cast.hpp"

namespace atl {
    template <class Symbol = char, 
              long epsilon_ = 0,
              class SymbolProperty = no_type,
              class StateProperty = no_type, 
              class AutomatonProperty = no_type>
    class deterministic_finite_automaton
        : public detail::deterministic_finite_automaton_gen<Symbol, epsilon_,
                                                            SymbolProperty,
                                                            StateProperty,
                                                            AutomatonProperty> {
        public:
            typedef Symbol symbol_type;
            typedef detail::deterministic_finite_automaton_gen<Symbol, epsilon_,
                                                               SymbolProperty,
                                                               StateProperty,
                                                               AutomatonProperty> Base;

            typedef detail::finite_automaton_gen<Symbol, epsilon_,
                                                 SymbolProperty,
                                                 StateProperty,
                                                 AutomatonProperty> fa_type;
            typedef Base dfa_type;
            typedef detail::nondeterministic_finite_automaton_gen<Symbol, epsilon_,
                                                                  SymbolProperty,
                                                                  StateProperty,
                                                                  AutomatonProperty> nfa_type;

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
            deterministic_finite_automaton()
                : Base() {}

            deterministic_finite_automaton(const SymbolSet& alphabet)
                : Base(alphabet) {}

            deterministic_finite_automaton(const std::initializer_list<Symbol>& alphabet)
                : Base(alphabet) {}

            deterministic_finite_automaton(const deterministic_finite_automaton& x)
                : Base(x) {}

            ~deterministic_finite_automaton() {}

            deterministic_finite_automaton& 
            operator=(const deterministic_finite_automaton& x) {
                if (&x != this) {
                    Base::operator=(x);
                }
                return *this;
            }

            deterministic_finite_automaton
            operator&(const deterministic_finite_automaton& x) {
                deterministic_finite_automaton out, dfa_lhs, dfa_rhs;
                minimize(*this, dfa_lhs);
                minimize(x, dfa_rhs);
                intersect_fa(dfa_lhs, dfa_rhs, out);
                return out;
            }

            deterministic_finite_automaton
            operator&(const nfa_type& x) {
                deterministic_finite_automaton out, dfa_lhs, dfa_rhs;
                minimize(*this, dfa_lhs);
                minimize(x, dfa_rhs);
                intersect_fa(dfa_lhs, dfa_rhs, out);
                return out;
            }
            
            deterministic_finite_automaton
            operator|(const deterministic_finite_automaton& x) {
                deterministic_finite_automaton out, dfa_lhs, dfa_rhs;
                minimize(*this, dfa_lhs);
                minimize(x, dfa_rhs);
                union_fa(dfa_lhs, dfa_rhs, out);
                return out;
            }

            deterministic_finite_automaton
            operator|(const nfa_type& x) {
                deterministic_finite_automaton out, dfa_lhs, dfa_rhs;
                minimize(*this, dfa_lhs);
                minimize(x, dfa_rhs);
                union_fa(dfa_lhs, dfa_rhs, out);
                return out;
            }

            deterministic_finite_automaton
            operator-(const deterministic_finite_automaton& x) {
                deterministic_finite_automaton out, dfa_lhs, dfa_rhs, dfa;
                minimize(*this, dfa_lhs);
                minimize(x, dfa_rhs);
                complement_fa(dfa_rhs, dfa);
                intersect_fa(dfa_lhs, dfa, out);
                return out;
            }

            deterministic_finite_automaton
            operator-(const nfa_type& x) {
                deterministic_finite_automaton out, dfa_lhs, dfa_rhs, dfa;
                minimize(*this, dfa_lhs);
                minimize(x, dfa_rhs);
                complement_fa(dfa_rhs, dfa);
                intersect_fa(dfa_lhs, dfa, out);
                return out;
            }
            
            deterministic_finite_automaton
            operator+(const deterministic_finite_automaton& x) {
                deterministic_finite_automaton out, dfa_lhs, dfa_rhs;
                minimize(*this, dfa_lhs);
                minimize(x, dfa_rhs);
                concat_fa(dfa_lhs, dfa_rhs, out);
                return out;
            }

            deterministic_finite_automaton
            operator+(const nfa_type& x) {
                deterministic_finite_automaton out, dfa_lhs, dfa_rhs;
                minimize(*this, dfa_lhs);
                minimize(x, dfa_rhs);
                concat_fa(dfa_lhs, dfa_rhs, out);
                return out;
            }

            deterministic_finite_automaton
            operator!() {
                deterministic_finite_automaton dfa, out;
                minimize(*this, dfa);
                complement_fa(dfa, out);
                return out;
            }

            bool
            operator==(const deterministic_finite_automaton& x) {
                deterministic_finite_automaton dfa_lhs, dfa_rhs;
                minimize(*this, dfa_lhs);
                minimize(x, dfa_rhs);
                return equal_fa(dfa_lhs, dfa_rhs);
            }

            bool
            operator==(const nfa_type& x) {
                deterministic_finite_automaton dfa_lhs, dfa_rhs;
                minimize(*this, dfa_lhs);
                minimize(x, dfa_rhs);
                return equal_fa(dfa_lhs, dfa_rhs);
            }
        private:
        };
}

#endif /* atl_deterministic_finite_automaton_hpp */
