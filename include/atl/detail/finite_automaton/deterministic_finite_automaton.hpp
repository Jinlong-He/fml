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

#include "atl/detail/letter2letter_transducer/deterministic_letter2letter_transducer.hpp"
#include "atl/detail/omega_automaton/omega_automaton.hpp"
#include <map>
#include <unordered_map>
#include <atl/detail/finite_automaton/finite_automaton.hpp>

using std::unordered_map;

namespace atl::detail {
    template <class Symbol, 
              class SymbolProperty,
              class StateProperty, 
              class AutomatonProperty>
    class deterministic_finite_automaton_gen
        : public finite_automaton_gen<Symbol,
                                      SymbolProperty,
                                      StateProperty,
                                      AutomatonProperty> {
    public:
        typedef finite_automaton_gen<Symbol,
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

        deterministic_finite_automaton_gen(const SymbolSet& alphabet, const Symbol& epsilon)
            : Base(alphabet, epsilon) {}

        deterministic_finite_automaton_gen(const std::initializer_list<Symbol>& alphabet,
                                           const Symbol& epsilon)
            : Base(alphabet, epsilon) {}

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

        virtual void 
        clear_out_transitions(State s) {
            transition_map_.erase(s);
            Base::clear_out_transitions(s);
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
        remove_transition(Transition t) {
            auto source = this -> source(t);
            auto& transition_property = atl::get_property(*this, t);
            auto& map = transition_map_.at(source);
            if constexpr (std::is_same<SymbolProperty, no_type>::value) {
                map.erase(transition_property);
            } else {
                auto& symbol = transition_property.default_property;
                auto& prop = transition_property.extended_property;
                auto& prop_map = map.at(symbol);
                prop_map.erase(prop);
                if (prop_map.size() == 0) {
                    map.erase(symbol);
                }
            }
            if (map.size() == 0) {
                transition_map_.erase(source);
            }
            Base::remove_transition(t);
        }

        virtual void
        remove_transition(State s, State t) {
            if (transition_map_.count(s) == 0) return;
            auto& map = transition_map_.at(s);
            vector<Symbol> del_symbols;
            vector<SymbolProperty> del_props;
            if constexpr (std::is_same<SymbolProperty, no_type>::value) {
                for (auto& [symbol, state] : map) {
                    if (state == t) {
                        del_symbols.emplace_back(symbol);
                    }
                }
            } else {
                for (auto& [symbol, prop_map] : map) {
                    del_props.clear();
                    for (auto& [prop, state] : prop_map) {
                        if (state == t) {
                            del_props.emplace_back(prop);
                        }
                    }
                    for (auto& prop : del_props) {
                        prop_map.erase(prop);
                    }
                    if (prop_map.size() == 0) {
                        del_symbols.emplace_back(symbol);
                    }
                }
            }
            for (auto& symbol : del_symbols) {
                map.erase(symbol);
            }
            if (map.size() == 0) {
                transition_map_.erase(s);
            }
            Base::remove_transition(s, t);
        }
    private:
        TransitionMap transition_map_;
    };
};

namespace atl {
    #define DFA_PARAMS typename DFA_SYMBOL, typename DFA_SYMBOL_PROP, typename DFA_STATE_PROP, typename DFA_AUT_PROP
    #define DFA detail::deterministic_finite_automaton_gen<DFA_SYMBOL, DFA_SYMBOL_PROP, DFA_STATE_PROP,DFA_AUT_PROP>

    template <DFA_PARAMS>
    inline typename DFA::TransitionMap const&
    transition_map(const DFA& dfa) {
        return dfa.transition_map();
    }

    template <DFA_PARAMS>
    inline void
    get_targets_in_map(const DFA& dfa, 
                       typename DFA::State s, 
                       const DFA_SYMBOL& c, 
                       typename DFA::StateSet& set) {
        const auto& transition_map_ = transition_map(dfa);
        auto transition_map_iter = transition_map_.find(s);
        if (transition_map_iter != transition_map_.end()) {
            const auto& map = transition_map_iter -> second;
            auto map_iter = map.find(c);
            if (map_iter != map.end()) {
                if constexpr (std::is_same<typename DFA::symbol_property_type, 
                                           no_type>::value) {
                    set.insert(map_iter -> second);
                } else {
                    for (auto& map_pair : map_iter -> second) {
                        set.insert(map_pair.second);
                    }
                }
            }
        }
    }

    template <DFA_PARAMS>
    inline void
    get_targets_in_map(const DFA& dfa, 
                       typename DFA::State s, 
                       const DFA_SYMBOL& c, 
                       typename DFA::symbol_property_type const& p, 
                       typename DFA::StateSet& set) {
        if constexpr (std::is_same<typename DFA::symbol_property_type, 
                                   no_type>::value) {
            const auto& transition_map_ = transition_map(dfa);
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
}

#endif /* atl_detail_deterministic_finite_automaton_hpp */
