//
//  deterministic_finite_automaton.hpp
//  atl 
//
//  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
//  SPDX-License-Identifier: MIT
//  Copyright (c) 2020 Jinlong He.
//

#ifndef atl_detail_deterministic_finite_automaton_hpp 
#define atl_detail_deterministic_finite_automaton_hpp

#include <map>
#include <boost/unordered_map.hpp>
#include <atl/detail/finite_automaton/finite_automaton.hpp>

using boost::unordered_map;

namespace atl::detail {
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
        typedef finite_automaton_gen<Symbol, epsilon_,
                                     SymbolProperty,
                                     StateProperty,
                                     AutomatonProperty> Base;

        typedef typename Base::symbol_type symbol_type;
        typedef typename Base::symbol_property_type symbol_property_type;
        typedef typename Base::transition_property_type transition_property_type;
        typedef typename Base::state_property_type state_property_type;
        typedef typename Base::automaton_property_type automaton_property_type;

        typedef typename Base::State State;
        typedef typename Base::StateSet StateSet;
        typedef typename Base::SymbolSet SymbolSet;
        typedef typename Base::Transition Transition;
        typedef typename Base::InTransitionIter InTransitionIter;
        typedef typename Base::OutTransitionIter OutTransitionIter;

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

        deterministic_finite_automaton_gen(const SymbolSet& alphabet)
            : Base(alphabet) {}

        deterministic_finite_automaton_gen(const std::initializer_list<Symbol>& alphabet)
            : Base(alphabet) {}

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
                    return Base::add_transition(s, t, c);
                }
            } else {
                if (transition_map_[s][c.default_property].count(c.extended_property) == 0) {
                    transition_map_[s][c.default_property][c.extended_property] = t;
                    return Base::add_transition(s, t, c);
                } 
            }
            return std::make_pair(Transition(), false);
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

        virtual void 
        clear_out_transitions(State s) {
            transition_map_.erase(s);
            Base::clear_out_transitions(s);
        }
                        
        virtual void 
        clear_in_transitions(State s) {
            InTransitionIter first, last;
            StateSet del_states;
            vector<Symbol> del_symbols;
            vector<SymbolProperty> del_props;
            tie(first, last) = this -> in_transitions(s);
            for (; first != last; first++) {
                del_states.insert(this -> source(*first));
            }
            for (auto del_state : del_states) {
                auto& map = transition_map_.at(del_state);
                del_symbols.clear();
                if constexpr (std::is_same<SymbolProperty, no_type>::value) {
                    for (auto& [symbol, state] : map) {
                        if (state == s) {
                            del_symbols.push_back(symbol);
                        }
                    }
                } else {
                    for (auto& [symbol, prop_map] : map) {
                        del_props.clear();
                        for (auto& [prop, state] : prop_map) {
                            if (state == s) {
                                del_props.push_back(prop);
                            }
                        }
                        for (auto& prop : del_props) {
                            prop_map.erase(prop);
                        }
                        if (prop_map.size() == 0) {
                            del_symbols.push_back(symbol);
                        }
                    }
                }
                for (auto& symbol : del_symbols) {
                    map.erase(symbol);
                }
                if (map.size() == 0) {
                    transition_map_.erase(del_state);
                }
            }
            Base::clear_in_transitions(s);
        }

        void
        get_targets_in_map(State s, const Symbol& c, StateSet& set) const {
            auto transition_map_iter = transition_map_.find(s);
            if (transition_map_iter != transition_map_.end()) {
                const auto& map = transition_map_iter -> second;
                auto map_iter = map.find(c);
                if (map_iter != map.end()) {
                    if constexpr (std::is_same<SymbolProperty, no_type>::value) {
                        set.insert(map_iter -> second);
                    } else {
                        for (auto& map_pair : map_iter -> second) {
                            set.insert(map_pair.second);
                        }
                    }
                }
            }
        }

        void
        get_targets_in_map(State s, const Symbol& c, const SymbolProperty& p, StateSet& set) const {
            if constexpr (std::is_same<SymbolProperty, no_type>::value) {
                auto transition_map_iter = transition_map_.find(s);
                if (transition_map_iter != transition_map_.end()) {
                    const auto& map = transition_map_iter -> second;
                    auto map_iter = map.find(c);
                    if (map_iter != map.end()) {
                        auto iter = map_iter -> second.find(p);
                        if (iter != map_iter -> second.end()) {
                            set.insert(iter -> second);
                        }
                    }
                }
            }
        }
    private:
        TransitionMap transition_map_;
    };
};

namespace atl {
    #define DFA_PARAMS typename DFA_SYMBOL, long DFA_EPSILON, typename DFA_SYMBOL_PROP, typename DFA_STATE_PROP, typename DFA_AUT_PROP
    #define DFA detail::deterministic_finite_automaton_gen<DFA_SYMBOL, DFA_EPSILON, DFA_SYMBOL_PROP, DFA_STATE_PROP,DFA_AUT_PROP>

    template <DFA_PARAMS>
    inline typename DFA::TransitionMap const&
    transition_map(const DFA& dfa) {
        return dfa.transition_map();
    }

    template <DFA_PARAMS>
    inline void
    get_targets_in_map(const DFA& dfa, 
                       typename DFA::State s, 
                       typename DFA::symbol_type const& c, 
                       typename DFA::StateSet& set) {
        dfa.get_targets_in_map(s, c, set);
    }

    template <DFA_PARAMS>
    inline void
    get_targets_in_map(const DFA& dfa, 
                       typename DFA::State s, 
                       typename DFA::symbol_type const& c, 
                       typename DFA::symbol_property_type const& p, 
                       typename DFA::StateSet& set) {
        dfa.get_targets_in_map(s, c, p, set);
    }
}

#endif /* atl_detail_deterministic_finite_automaton_hpp */
