//
//  buchi_automaton.hpp
//  atl 
//
//  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
//  SPDX-License-Identifier: MIT
//  Copyright (c) 2020 Jinlong He.
//

#ifndef atl_detail_buchi_automaton_hpp 
#define atl_detail_buchi_automaton_hpp

#include <unordered_map>
#include <atl/detail/omega_automaton/omega_automaton.hpp>

using std::unordered_map;

namespace atl::detail {
    template <class Symbol, 
              long epsilon_,
              class SymbolProperty,
              class StateProperty, 
              class AutomatonProperty>
    class buchi_automaton_gen
        : public omega_automaton_gen<Symbol, epsilon_,
                                      SymbolProperty,
                                      StateProperty,
                                      AutomatonProperty> {
    public:
        typedef omega_automaton_gen<Symbol, epsilon_,
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
        buchi_automaton_gen()
            : Base() {}

        buchi_automaton_gen(const SymbolSet& alphabet)
            : Base(alphabet) {}

        buchi_automaton_gen(const std::initializer_list<Symbol>& alphabet)
            : Base(alphabet) {}

        buchi_automaton_gen(const buchi_automaton_gen& x)
            : Base(x),
              transition_map_(x.transition_map_) {}

        ~buchi_automaton_gen() {}

        buchi_automaton_gen& 
        operator=(const buchi_automaton_gen& x) {
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

        const StateSet&
        accept_state_set() const {
            return accept_state_set_;
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
        StateSet accept_state_set_;
    };
};

namespace atl {
    #define BA_PARAMS typename BA_SYMBOL, long BA_EPSILON, typename BA_SYMBOL_PROP, typename BA_STATE_PROP, typename BA_AUT_PROP
    #define BA detail::buchi_automaton_gen<BA_SYMBOL, BA_EPSILON, BA_SYMBOL_PROP, BA_STATE_PROP,BA_AUT_PROP>

    template <BA_PARAMS>
    inline typename BA::TransitionMap const&
    transition_map(const BA& ba) {
        return ba.transition_map();
    }

    template <BA_PARAMS>
    inline typename BA::StateSet const&
    accept_state_set(const BA& ba) {
        return ba.accept_state_set();
    }
};

#endif /* atl_detail_buchi_automaton_hpp */
