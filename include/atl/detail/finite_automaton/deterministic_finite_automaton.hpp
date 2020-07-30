//
//  deterministic_finite_automaton.hpp
//  atl 
//
//  Created by Jinlong He on 2019/10/27.
//  Copyright © 2019年 Ruting-Team. All rights reserved.
//

#ifndef atl_detail_deterministic_finite_automaton_hpp 
#define atl_detail_deterministic_finite_automaton_hpp

#include <map>
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
            typedef unordered_set<State> StateSet;
            typedef pair<State, State> StatePair;
            typedef unordered_map<StatePair, State> StatePairMap;

            typedef typename std::conditional<std::is_same<SymbolProperty, no_type>::value,
                                  unordered_map<Symbol, StatePair>,
                                  unordered_map<Symbol, 
                                  unordered_map<SymbolProperty, StatePair> > >::type 
                Symbol2StatePairMap;

            typedef typename std::conditional<std::is_same<SymbolProperty, no_type>::value,
                                  std::map<Symbol, State>,
                                  std::map<Symbol, std::map<SymbolProperty, State> > >::type
                Symbol2StateMap;

            typedef unordered_map<State, Symbol2StateMap> TransitionMap;

        public:
            deterministic_finite_automaton_gen()
                : Base() {}

            deterministic_finite_automaton_gen(const deterministic_finite_automaton_gen& x)
                : Base(x),
                  transition_map_(x.transition_map_) {}

            ~deterministic_finite_automaton_gen() {}

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

            void
            get_targets_in_map(State s, const Symbol& c, StateSet& set) const {
                auto transition_map_iter = transition_map_.find(s);
                if (transition_map_iter != transition_map_.end()) {
                    const auto& map = transition_map_iter -> second;
                    auto map_iter = map.find(c);
                    if (map_iter != map.end()) {
                        if constexpr (std::is_same<SymbolProperty, no_type>::value) {
                            set.insert(*map_iter);
                        } else {
                            for (auto& map_pair : map_iter -> second) {
                                set.insert(map_pair.second);
                            }
                        }
                    }
                }
            }

            void
            get_targets_in_map(State s, const Symbol& c, SymbolProperty p, StateSet& set) const {
                if constexpr (std::is_same<SymbolProperty, no_type>::value) {
                    auto transition_map_iter = transition_map_.find(s);
                    if (transition_map_iter != transition_map_.end()) {
                        const auto& map = transition_map_iter -> second;
                        auto map_iter = map.find(c);
                        if (map_iter != map.end()) {
                            auto iter = map_iter -> second.fing(p);
                            if (iter != map_iter -> second.end()) {
                                set.insert(iter -> second.begin(), iter -> second.end());
                            }
                        }
                    }
                }
            }
        private:
            TransitionMap transition_map_;
        };
    }
    #define DFA_PARAMS typename DFA_SYMBOL, long DFA_EPSILON, typename DFA_SYMBOL_PROP, typename DFA_STATE_PROP, typename DFA_AUT_PROP
    #define DFA detail::deterministic_finite_automaton_gen<DFA_SYMBOL, DFA_EPSILON, DFA_SYMBOL_PROP, DFA_STATE_PROP,DFA_AUT_PROP>

    template <DFA_PARAMS>
    inline typename DFA::TransitionMap const&
    transition_map(const DFA& dfa) {
        return dfa.transition_map();
    }
}

#endif /* atl_detail_deterministic_finite_automaton_hpp */
