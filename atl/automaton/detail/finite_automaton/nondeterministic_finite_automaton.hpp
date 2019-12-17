//
//  nondeterministic_finite_automaton.hpp
//  ATL 
//
//  Created by Jinlong He on 2019/10/27.
//  Copyright © 2019年 Ruting-Team. All rights reserved.
//

#ifndef atl_detail_nondeterministic_finite_automaton_hpp 
#define atl_detail_nondeterministic_finite_automaton_hpp

#include <boost/unordered_map.hpp>
#include "finite_automaton.hpp"

using boost::unordered_map;

namespace boost {
    template <class K, class C, class A>
    std::size_t hash_value(const unordered_set<K, C, A>& v) {
        return hash_range(v.begin(), v.end());
    }
}

namespace atl {
    namespace detail {
        template <class Symbol, 
                  long epsilon_,
                  class SymbolProperty,
                  class StateProperty, 
                  class AutomatonProperty>
        class nondeterministic_finite_automaton_gen
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
            typedef unordered_map<StateSet, State> StateSetMap;

            typedef typename std::conditional<std::is_same<SymbolProperty, no_type>::value,
                                  unordered_map<Symbol, StateSet>,
                                  unordered_map<Symbol, 
                                  unordered_map<SymbolProperty, StateSet> > > ::type
                    Symbol2StateSetMap;

            typedef unordered_map<State, Symbol2StateSetMap> TransitionMap;

        public:
            nondeterministic_finite_automaton_gen()
                : Base() {}

            nondeterministic_finite_automaton_gen(const nondeterministic_finite_automaton_gen& x)
                : Base(x),
                  transition_map_(x.transition_map_) {}

            nondeterministic_finite_automaton_gen& 
            operator=(const nondeterministic_finite_automaton_gen& x) {
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
            add_transition(State s, State t, const Symbol& c) {
                if constexpr (std::is_same<SymbolProperty, no_type>::value) {
                    auto& targets = transition_map_[s][c];
                    if (targets.insert(t).second) {
                        if (targets.size() > 1) set_undeterministic_flag(*this);
                        return Base::add_transition(s, t, c);
                    }
                } else {
                    auto& targets = transition_map_[s][c][symbol_property_type()];
                    if (targets.insert(t).second) {
                        if (targets.size() > 1) set_undeterministic_flag(*this);
                        return Base::add_transition(s, t, c, symbol_property_type());
                    }
                }
                return std::make_pair(Transition(), false);
            }

            virtual pair<Transition, bool>
            add_transition(State s, State t,
                           const Symbol& c,
                           const SymbolProperty& p) {
                if constexpr (!std::is_same<SymbolProperty, no_type>::value) {
                    auto& targets = transition_map_[s][c][p];
                    if (targets.insert(t).second) {
                        if (targets.size() > 1) set_undeterministic_flag(*this);
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
                            set.insert(map_iter -> second.begin(), map_iter -> second.end());
                        } else {
                            for (auto& map_pair : map_iter -> second) {
                                set.insert(map_pair.second.begin(), map_pair.second.end());
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

    //template <typename NFA>
    //inline pair<typename NFA::Transition, bool>
    //add_epsilon_transition(NFA& a,
    //                       typename NFA::State s,
    //                       typename NFA::State t) {
    //    set_modified_flag(a);
    //    set_epsilon_flag(a);
    //    return a.add_transition(s, t, a.epsilon());
    //}

    template <typename NFA>
    inline void
    get_targets_in_map(const NFA& nfa, 
                       typename NFA::State s, 
                       typename NFA::symbol_type const& c, 
                       typename NFA::StateSet& set) {
        nfa.get_targets_in_map(s, c, set);
    }

    template <typename NFA>
    inline void
    get_targets_in_map(const NFA& nfa, 
                       typename NFA::StateSet const& set, 
                       typename NFA::symbol_type const& c, 
                       typename NFA::StateSet& targets) {
        for (auto s : set) {
            nfa.get_targets_in_map(s, c, targets);
        }
    }
}

#endif /* atl_detail_nondeterministic_finite_automaton_hpp */
