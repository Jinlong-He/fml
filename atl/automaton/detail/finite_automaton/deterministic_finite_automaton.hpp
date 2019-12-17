//
//  deterministic_finite_automaton.hpp
//  ATL 
//
//  Created by Jinlong He on 2019/10/27.
//  Copyright © 2019年 Ruting-Team. All rights reserved.
//

#ifndef atl_detail_deterministic_finite_automaton_hpp 
#define atl_detail_deterministic_finite_automaton_hpp

#include <boost/unordered_map.hpp>
#include "finite_automaton.hpp"

using boost::unordered_map;

namespace atl {
    namespace detail {
        template <class Symbol, 
                  long epsilon_,
                  class SymbolProperty,
                  class StateProperty, 
                  class AutomatonProperty>
        class deterministic_finite_automaton_gen
            : public finite_automaton_gen<Symbol, epsilon_,
                                          SymbolProperty,
                                          StateProperty,
                                          AutomatonProperty> {
        public:
            typedef Symbol symbol_type;
            typedef SymbolProperty symbol_property_type;

            typedef finite_automaton_gen<Symbol, epsilon_,
                                         SymbolProperty,
                                         StateProperty,
                                         AutomatonProperty> Base;

            typedef typename Base::transition_property_type transition_property_type;
            typedef typename Base::state_property_type state_property_type;
            typedef typename Base::automaton_property_type automaton_property_type;
            typedef typename Base::Transition Transition;
            typedef typename Base::State State;
            typedef pair<State, State> StatePair;
            typedef unordered_map<StatePair, State> StatePairMap;

            typedef typename std::conditional<std::is_same<SymbolProperty, no_type>::value,
                                  unordered_map<Symbol, StatePair>,
                                  unordered_map<Symbol, 
                                  unordered_map<SymbolProperty, StatePair> > >::type 
                                      Symbol2StatePairMap;

            typedef typename std::conditional<std::is_same<SymbolProperty, no_type>::value,
                                  unordered_map<Symbol, State>,
                                  unordered_map<Symbol, 
                                  unordered_map<SymbolProperty, State> > >::type Symbol2StateMap;

            typedef unordered_map<Symbol, Symbol2StateMap> TransitionMap;

        public:
            deterministic_finite_automaton_gen()
                : Base() {}

            deterministic_finite_automaton_gen(const deterministic_finite_automaton_gen& x)
                : Base(x),
                  transition_map_(x.transition_map_) {}

            deterministic_finite_automaton_gen& 
            operator=(const deterministic_finite_automaton_gen& x) {
                if (&x != this) {
                    Base::operator=(x);
                    transition_map_ = x.transition_map_;
                }
                return *this;
            }

            virtual void clear() {
                Base::clear();
                transition_map_.clear();
            }

            const TransitionMap&
            transition_map() const {
                return transition_map_;
            }

            virtual pair<Transition, bool>
            add_transition(State s, State t, 
                           const transition_property_type& c) {
                if constexpr (std::is_same<SymbolProperty, no_type>::value) {
                    if (transition_map_[s].count(c) == 0) {
                        transition_map_[s][c] = t;
                    }
                } else {
                    if (transition_map_[s][c.symbol].count(c.symbol_property) == 0) {
                        transition_map_[s][c.symbol][c.symbol_property] = t;
                    } 
                }
                return Base::add_transition(s, t, c);
            }

            virtual pair<Transition, bool>
            add_transition(State s, State t,
                           const Symbol& c,
                           const SymbolProperty& p) {
                if constexpr (!std::is_same<SymbolProperty, no_type>::value) {
                    if (transition_map_[s][c].count(p) == 0) {
                        transition_map_[s][c][p] = t;
                        return Base::add_transition(s, t, c, p);
                    } 
                }
                return std::make_pair(Transition(), false);
            }

        private:
            TransitionMap transition_map_;
        };
    }
}

#endif /* atl_detail_deterministic_finite_automaton_hpp */
