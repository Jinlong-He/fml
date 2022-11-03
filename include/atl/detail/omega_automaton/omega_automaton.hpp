//
//  omega_automaton.hpp
//  ATL 
//
//  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
//  SPDX-License-Identifier: MIT
//  Copyright (c) 2020 Jinlong He.
//

#ifndef atl_detail_omega_automaton_hpp 
#define atl_detail_omega_automaton_hpp

#include <unordered_set>
#include <unordered_map>
#include <util/util.hpp>
#include <atl/detail/automaton.hpp>
#include <atl/detail/no_type.hpp>

using std::unordered_map, std::unordered_set;

namespace atl::detail {

    template <class Symbol, 
              Symbol epsilon_,
              class SymbolProperty,
              class StateProperty, 
              class AutomatonProperty>
    class omega_automaton_gen 
        : public automaton_gen<
                 typename std::conditional<std::is_same<SymbolProperty, no_type>::value,
                               Symbol, Property<Symbol, SymbolProperty> >::type,
                 typename std::conditional<std::is_same<StateProperty, no_type>::value,
                               boost::no_property, StateProperty>::type,
                 typename std::conditional<std::is_same<AutomatonProperty, no_type>::value,
                               boost::no_property, AutomatonProperty>::type> {
    public:
        typedef Property<Symbol, SymbolProperty> transition_property;
        typedef Symbol symbol_type;
        typedef SymbolProperty symbol_property_type;

        typedef typename std::conditional<std::is_same<SymbolProperty, no_type>::value,
                              Symbol, transition_property>::type 
            transition_property_type;
        typedef typename std::conditional<std::is_same<StateProperty, no_type>::value,
                               boost::no_property, StateProperty>::type
            state_property_type;
        typedef typename std::conditional<std::is_same<AutomatonProperty, no_type>::value,
                               boost::no_property, AutomatonProperty>::type
            automaton_property_type;
        typedef automaton_gen<transition_property_type, 
                              state_property_type, 
                              automaton_property_type> Base;

        typedef typename Base::State State;
        typedef typename Base::Transition Transition;
        typedef typename Base::InTransitionIter InTransitionIter;
        typedef typename Base::OutTransitionIter OutTransitionIter;

        typedef unordered_set<State> StateSet;
        typedef unordered_set<Symbol> SymbolSet;

        typedef unordered_map<State, State> State2Map;

    public:
        omega_automaton_gen(const SymbolSet alphabet = SymbolSet())
            : Base(),
              initial_state_(-1),
              alphabet_(alphabet) {}

        omega_automaton_gen(const std::initializer_list<Symbol> alphabet)
            : Base(),
              initial_state_(-1),
              alphabet_(alphabet) {}

        omega_automaton_gen(const omega_automaton_gen& x)
            : Base(x),
              initial_state_(x.initial_state_),
              state_set_(x.state_set_),
              alphabet_(x.alphabet_) {}

        ~omega_automaton_gen() {}

        omega_automaton_gen& 
        operator=(const omega_automaton_gen& x) {
            if (&x != this) {
                Base::operator=(x);
                initial_state_ = x.initial_state_;
                state_set_ = x.state_set_;
                alphabet_ = x.alphabet_;
            }
            return *this;
        }

        virtual void clear() {
            Base::clear();
            initial_state_ = -1;
            state_set_.clear();
            alphabet_.clear();
        }

        virtual State 
        add_state(const state_property_type& p) {
            State state = Base::add_state(p);
            state_set_.insert(state);
            return state;
        }

        virtual State 
        add_state() {
            State state = Base::add_state();
            state_set_.insert(state);
            return state;
        }

        virtual void 
        clear_state(State s) {
            clear_out_transitions(s);
            clear_in_transitions(s);
            Base::clear_state(s);
        }
                        
        virtual void 
        clear_out_transitions(State s) {
            Base::clear_out_transitions(s);
        }
                        
        virtual void 
        clear_in_transitions(State s) {
            InTransitionIter first, last;
            StateSet del_states;
            tie(first, last) = this -> in_transitions(s);
            for (; first != last; first++) {
                del_states.insert(this -> source(*first));
            }
            for (auto del_state : del_states) {
                remove_transition(del_state, s);
            }
        }

        virtual void 
        remove_state(State s) {
            state_set_.erase(s);
            Base::remove_state(s);
        }

        Symbol
        epsilon() const {
            return epsilon_;
        }

        transition_property_type
        epsilon_transition() const {
            return transition_property_type(epsilon_);
        }

        State
        initial_state() const {
            return initial_state_;
        }

        void 
        set_initial_state(State state) {
            initial_state_ = state;
        }

        const StateSet&
        state_set() const {
            return state_set_;
        }

        const SymbolSet&
        alphabet() const {
            return alphabet_;
        }

        virtual void
        set_alphabet(const SymbolSet& alphabet) {
            alphabet_ = alphabet;
        }

        virtual void 
        add_alphabet(const Symbol& c) {
            alphabet_.insert(c);
        }

        using Base::add_transition;
        virtual pair<Transition, bool>
        add_transition(State s, State t,
                       const transition_property_type& c) {
            if constexpr (std::is_same<SymbolProperty, no_type>::value) {
                if (c == epsilon()) this -> set_flag(4, 1);
            } else {
                if (c.default_property == epsilon()) this -> set_flag(4, 1);
            }
            return Base::add_transition(s, t, c);
        }

        virtual pair<Transition, bool>
        add_transition(State s, State t,
                       const Symbol& c,
                       const SymbolProperty& p) {
            if constexpr (std::is_same<SymbolProperty, no_type>::value) {
                if (c == epsilon()) this -> set_flag(4, 1);
                return Base::add_transition(s, t, c);
            } else {
                if (c == epsilon()) this -> set_flag(4, 1);
                return Base::add_transition(s, t, transition_property(c, p));
            }
        }

        virtual void
        remove_transition(Transition t) {
            Base::remove_transition(t);
        }

        virtual void
        remove_transition(State s, State t) {
            Base::remove_transition(s, t);
        }

    private:
        State initial_state_;
        StateSet state_set_;
        SymbolSet alphabet_;
    };
};

namespace atl {
    #define OA_PARAMS typename OA_SYMBOL, OA_SYMBOL OA_EPSILON, typename OA_SYMBOL_PROP, typename OA_STATE_PROP, typename OA_AUT_PROP
    #define OA detail::omega_automaton_gen<OA_SYMBOL, OA_EPSILON, OA_SYMBOL_PROP, OA_STATE_PROP,OA_AUT_PROP>

    template <OA_PARAMS>
    inline typename OA::SymbolSet const&
    alphabet(const OA& oa) {
        return oa.alphabet();
    }

    template <OA_PARAMS>
    inline void
    set_alphabet(OA& oa,
                 typename OA::SymbolSet const& set) {
        oa.set_alphabet(set);
    }

    template <OA_PARAMS>
    inline typename OA::State
    initial_state(const OA& oa) {
        return oa.initial_state();
    }

    template <OA_PARAMS>
    inline void
    set_initial_state(OA& oa, 
                      typename OA::State state) {
        oa.set_initial_state(state);
    }

    template <OA_PARAMS>
    inline typename OA::StateSet const&
    state_set(const OA& oa) {
        return oa.state_set();
    }

    template <OA_PARAMS>
    inline OA_SYMBOL
    epsilon(const OA& oa) {
        return oa.epsilon();
    }

    template <OA_PARAMS>
    inline typename OA::transition_property_type
    epsilon_transition(const OA& oa) {
        return oa.epsilon_transition();
    }

    template <OA_PARAMS>
    inline void
    add_alphabet(OA& oa,
                 const OA_SYMBOL& c) {
        oa.add_alphabet(c);
    }

    template <OA_PARAMS>
    inline typename OA::State
    add_initial_state(OA& oa,
                      typename OA::state_property_type const& p) {
        typename OA::State s = add_state(oa, p);
        oa.set_initial_state(s);
        return s;
    }

    template <OA_PARAMS>
    inline typename OA::State
    add_initial_state(OA& oa) {
        typename OA::State s = add_state(oa);
        oa.set_initial_state(s);
        return s;
    }

    template <OA_PARAMS>
    inline pair<typename OA::Transition, bool>
    add_transition(OA& oa,
                   typename OA::State s,
                   typename OA::State t,
                   const OA_SYMBOL& c,
                   typename OA::symbol_property_type const& p) {
        return oa.add_transition(s, t, c, p);
    }
    
    template <OA_PARAMS>
    inline bool
    is_initial_state(const OA& oa,
                     typename OA::State s) {
        return (s == initial_state(oa));
    }

    template <OA_PARAMS>
    inline void 
    set_forward_reachable_flag(OA& a, bool b = true) {
        a.set_flag(1, b);
        a.set_flag(0, 0);
    }

    template <OA_PARAMS>
    inline bool
    is_forward_reachable(const OA& a) {
        return (a.flag(1) & !a.flag(0));
    }

    template <OA_PARAMS>
    inline void 
    set_minimal_flag(OA& a, bool b = true) {
        a.set_flag(2, b);
        a.set_flag(1, 1);
        a.set_flag(0, 0);
    }

    template <OA_PARAMS>
    inline bool
    is_minimal(const OA& a) {
        return (a.flag(2) & !a.flag(0));
    }

    template <OA_PARAMS>
    inline void 
    set_undeterministic_flag(OA& a, bool b = true) {
        a.set_flag(3, b);
    }

    template <OA_PARAMS>
    inline bool
    is_undeterministic(const OA& a) {
        return (a.flag(3) | a.flag(4));
    }

    template <OA_PARAMS>
    inline void 
    set_epsilon_flag(OA& a, bool b = true) {
        a.set_flag(4, b);
    }

    template <OA_PARAMS>
    inline bool
    has_epsilon_transition(const OA& a) {
        return a.flag(4);
    }

};

#endif /* atl_detail_omega_automaton_hpp */
