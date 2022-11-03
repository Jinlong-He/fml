//
//  nondeterministic_finite_automaton.hpp
//  atl 
//
//  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
//  SPDX-License-Identifier: MIT
//  Copyright (c) 2020 Jinlong He.
//

#ifndef atl_detail_nondeterministic_finite_automaton_hpp 
#define atl_detail_nondeterministic_finite_automaton_hpp

#include "atl/detail/letter2letter_transducer/deterministic_letter2letter_transducer.hpp"
#include <unordered_map>
#include <atl/detail/finite_automaton/finite_automaton.hpp>

using std::unordered_map;

namespace atl::detail {
    template <class Symbol, 
              class SymbolProperty,
              class StateProperty, 
              class AutomatonProperty>
    class nondeterministic_finite_automaton_gen
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
        typedef typename Base::state_property_type state_property_type;
        typedef typename Base::symbol_property_type symbol_property_type;
        typedef typename Base::automaton_property_type automaton_property_type;
        typedef typename Base::transition_property_type transition_property_type;

        typedef typename Base::State State;
        typedef typename Base::StateSet StateSet;
        typedef typename Base::SymbolSet SymbolSet;
        typedef typename Base::Transition Transition;
        typedef typename Base::InTransitionIter InTransitionIter;
        typedef typename Base::OutTransitionIter OutTransitionIter;

        typedef unordered_map<StateSet, State> StateSetMap;

        typedef typename std::conditional<std::is_same<SymbolProperty, no_type>::value,
                              std::map<Symbol, StateSet>,
                              std::map<Symbol, 
                              std::map<SymbolProperty, StateSet> > > ::type
                Symbol2StateSetMap;

        typedef unordered_map<State, Symbol2StateSetMap> TransitionMap;

    public:
        nondeterministic_finite_automaton_gen()
            : Base() {}

        nondeterministic_finite_automaton_gen(const SymbolSet& alphabet, 
                                              const Symbol& epsilon)
            : Base(alphabet, epsilon) {}

        nondeterministic_finite_automaton_gen(const std::initializer_list<Symbol>& alphabet,
                                              const Symbol& epsilon)
            : Base(alphabet, epsilon) {}

        nondeterministic_finite_automaton_gen(const nondeterministic_finite_automaton_gen& x)
            : Base(x),
              transition_map_(x.transition_map_) {}

        ~nondeterministic_finite_automaton_gen() {}

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

        virtual void 
        clear_out_transitions(State s) {
            transition_map_.erase(s);
            Base::clear_out_transitions(s);
        }
                        
        using Base::add_transition;
        virtual pair<Transition, bool>
        add_transition(State s, State t, const transition_property_type& c) {
            if constexpr (std::is_same<SymbolProperty, no_type>::value) {
                if (c == epsilon(*this)) this -> set_flag(4, 1);
                auto& targets = transition_map_[s][c];
                if (targets.insert(t).second) {
                    if (targets.size() > 1) set_undeterministic_flag(*this);
                    return Base::add_transition(s, t, c);
                }
            } else {
                if (c.default_property == epsilon(*this)) this -> set_flag(4, 1);
                auto& targets = transition_map_[s][c.default_property][c.extended_property];
                if (targets.insert(t).second) {
                    if (targets.size() > 1) set_undeterministic_flag(*this);
                    return Base::add_transition(s, t, c);
                }
            }
            return std::make_pair(Transition(), false);
        }

        virtual pair<Transition, bool>
        add_transition(State s, State t,
                       const Symbol& c,
                       const SymbolProperty& p) {
            if (c == epsilon(*this)) this -> set_flag(4, 1);
            if constexpr (!std::is_same<SymbolProperty, no_type>::value) {
                auto& targets = transition_map_[s][c][p];
                if (targets.insert(t).second) {
                    if (targets.size() > 1) set_undeterministic_flag(*this);
                    return Base::add_transition(s, t, c, p);
                }
            }
            return std::make_pair(Transition(), false);
        }

        virtual void
        remove_transition(Transition t) {
            auto source = this -> source(t);
            auto target = this -> target(t);
            auto& transition_property = atl::get_property(*this, t);
            auto& map = transition_map_.at(source);
            if constexpr (std::is_same<SymbolProperty, no_type>::value) {
                auto& states = map.at(transition_property);
                states.erase(target);
                if (states.size() == 0) {
                    map.erase(transition_property);
                }
            } else {
                auto& symbol = transition_property.default_property;
                auto& prop = transition_property.extended_property;
                auto& prop_map = map.at(symbol);
                auto& states = prop_map.at(prop);
                states.erase(target);
                if (states.size() == 0) {
                    prop_map.erase(prop);
                }
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
                for (auto& [symbol, states] : map) {
                    if (states.count(t) > 0) {
                        states.erase(t);
                        if (states.size() == 0) {
                            del_symbols.emplace_back(symbol);
                        }
                    }
                }
            } else {
                for (auto& [symbol, prop_map] : map) {
                    del_props.clear();
                    for (auto& [prop, states] : prop_map) {
                        if (states.count(t) > 0) {
                            states.erase(t);
                            if (states.size() == 0) {
                                del_props.emplace_back(prop);
                            }
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
    #define NFA_PARAMS typename NFA_SYMBOL, typename NFA_SYMBOL_PROP, typename NFA_STATE_PROP, typename NFA_AUT_PROP
    #define NFA detail::nondeterministic_finite_automaton_gen<NFA_SYMBOL, NFA_SYMBOL_PROP, NFA_STATE_PROP,NFA_AUT_PROP>

    template <NFA_PARAMS>
    inline typename NFA::TransitionMap const&
    transition_map(const NFA& nfa) {
        return nfa.transition_map();
    }

    template <NFA_PARAMS>
    inline pair<typename NFA::Transition, bool>
    add_epsilon_transition(NFA& a,
                           typename NFA::State s,
                           typename NFA::State t) {
        set_modified_flag(a);
        set_epsilon_flag(a);
        return a.add_transition(s, t, a.epsilon_transition());
    }

    template <NFA_PARAMS>
    inline void
    get_targets_in_map(const NFA& nfa, 
                       typename NFA::State s, 
                       const NFA_SYMBOL& c, 
                       typename NFA::StateSet& set) {
        const auto& transition_map_ = transition_map(nfa);
        auto transition_map_iter = transition_map_.find(s);
        if (transition_map_iter != transition_map_.end()) {
            const auto& map = transition_map_iter -> second;
            auto map_iter = map.find(c);
            if (map_iter != map.end()) {
                if constexpr (std::is_same<typename NFA::symbol_property_type, 
                                           no_type>::value) {
                    set.insert(map_iter -> second.begin(), map_iter -> second.end());
                } else {
                    for (auto& map_pair : map_iter -> second) {
                        set.insert(map_pair.second.begin(), map_pair.second.end());
                    }
                }
            }
        }
    }

    template <NFA_PARAMS>
    inline void
    get_targets_in_map(const NFA& nfa, 
                       typename NFA::StateSet const& set, 
                       const NFA_SYMBOL& c, 
                       typename NFA::StateSet& targets) {
        for (auto s : set) {
            get_targets_in_map(nfa, s, c, targets);
        }
    }

    template <NFA_PARAMS>
    inline void
    get_targets_in_map(const NFA& nfa,
                       typename NFA::State s, 
                       const NFA_SYMBOL& c, 
                       typename NFA::symbol_property_type const& p,
                       typename NFA::StateSet& set) {
        if constexpr (!std::is_same<typename NFA::symbol_property_type, 
                                    no_type>::value) {
            const auto& transition_map_ = transition_map(nfa);
            auto transition_map_iter = transition_map_.find(s);
            if (transition_map_iter != transition_map_.end()) {
                const auto& map = transition_map_iter -> second;
                auto map_iter = map.find(c);
                if (map_iter != map.end()) {
                    auto iter = map_iter -> second.find(p);
                    if (iter != map_iter -> second.end()) {
                        set.insert(iter -> second.begin(), iter -> second.end());
                    }
                }
            }
        }
    }
};

#endif /* atl_detail_nondeterministic_finite_automaton_hpp */
