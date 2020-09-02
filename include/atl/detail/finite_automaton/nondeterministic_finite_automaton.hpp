//
//  nondeterministic_finite_automaton.hpp
//  atl 
//
//  Created by Jinlong He on 2019/10/27.
//  Copyright © 2019年 Ruting-Team. All rights reserved.
//

#ifndef atl_detail_nondeterministic_finite_automaton_hpp 
#define atl_detail_nondeterministic_finite_automaton_hpp

#include <boost/unordered_map.hpp>
#include <atl/detail/finite_automaton/finite_automaton.hpp>

using boost::unordered_map;

namespace boost {
    template <class K, class C, class A>
    std::size_t hash_value(const unordered_set<K, C, A>& v) {
        return hash_range(v.begin(), v.end());
    }
}

namespace atl::detail {
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
        typedef finite_automaton_gen<Symbol, epsilon_,
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

        nondeterministic_finite_automaton_gen(const SymbolSet& alphabet)
            : Base(alphabet) {}

        nondeterministic_finite_automaton_gen(const std::initializer_list<Symbol>& alphabet)
            : Base(alphabet) {}

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

        using Base::add_transition;
        virtual pair<Transition, bool>
        add_transition(State s, State t, const transition_property_type& c) {
            if constexpr (std::is_same<SymbolProperty, no_type>::value) {
                auto& targets = transition_map_[s][c];
                if (targets.insert(t).second) {
                    if (targets.size() > 1) set_undeterministic_flag(*this);
                    return Base::add_transition(s, t, c);
                }
            } else {
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
        get_targets_in_map(State s, const Symbol& c, const SymbolProperty& p, StateSet& set) const {
            if constexpr (!std::is_same<SymbolProperty, no_type>::value) {
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

    private:
        TransitionMap transition_map_;
    };
};

namespace atl {
    #define NFA_PARAMS typename NFA_SYMBOL, long NFA_EPSILON, typename NFA_SYMBOL_PROP, typename NFA_STATE_PROP, typename NFA_AUT_PROP
    #define NFA detail::nondeterministic_finite_automaton_gen<NFA_SYMBOL, NFA_EPSILON, NFA_SYMBOL_PROP, NFA_STATE_PROP,NFA_AUT_PROP>

    //template <typename NFA>
    //inline pair<typename NFA::Transition, bool>
    //add_epsilon_transition(NFA& a,
    //                       typename NFA::State s,
    //                       typename NFA::State t) {
    //    set_modified_flag(a);
    //    set_epsilon_flag(a);
    //    return a.add_transition(s, t, a.epsilon());
    //}

    template <NFA_PARAMS>
    inline void
    get_targets_in_map(const NFA& nfa, 
                       typename NFA::State s, 
                       typename NFA::symbol_type const& c, 
                       typename NFA::StateSet& set) {
        nfa.get_targets_in_map(s, c, set);
    }

    template <NFA_PARAMS>
    inline void
    get_targets_in_map(const NFA& nfa, 
                       typename NFA::StateSet const& set, 
                       typename NFA::symbol_type const& c, 
                       typename NFA::StateSet& targets) {
        for (auto s : set) {
            nfa.get_targets_in_map(s, c, targets);
        }
    }

    template <NFA_PARAMS>
    inline void
    get_targets_in_map(const NFA& nfa,
                       typename NFA::State s, 
                       typename NFA::symbol_type const& c, 
                       typename NFA::symbol_property_type const& p,
                       typename NFA::StateSet& set) {
        nfa.get_targets_in_map(s, c, p ,set);
    }

    template <NFA_PARAMS>
    inline typename NFA::TransitionMap const&
    transition_map(const NFA& nfa) {
        return nfa.transition_map();
    }
};

#endif /* atl_detail_nondeterministic_finite_automaton_hpp */
