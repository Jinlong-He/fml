//
//  finite_automaton.hpp
//  ATL 
//
//  Created by Jinlong He on 2019/10/27.
//  Copyright © 2019年 Ruting-Team. All rights reserved.
//

#ifndef atl_detail_finite_automaton_hpp 
#define atl_detail_finite_automaton_hpp

#include <boost/unordered_set.hpp>
#include <boost/unordered_map.hpp>
#include <util/util.hpp>
#include <atl/detail/automaton.hpp>
#include <atl/detail/no_type.hpp>

using boost::unordered_map;
using boost::unordered_set;

namespace atl::detail {
    template <class Symbol, 
              long epsilon_,
              class SymbolProperty,
              class StateProperty, 
              class AutomatonProperty> class nondeterministic_finite_automaton_gen;

    template <class Symbol, 
              long epsilon_,
              class SymbolProperty,
              class StateProperty, 
              class AutomatonProperty> class deterministic_finite_automaton_gen;


    template <class Symbol, 
              long epsilon_,
              class SymbolProperty,
              class StateProperty, 
              class AutomatonProperty>
    class finite_automaton_gen 
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

        typedef deterministic_finite_automaton_gen<Symbol, 
                                                   epsilon_,
                                                   SymbolProperty,
                                                   StateProperty,
                                                   AutomatonProperty> dfa_type;

        typedef nondeterministic_finite_automaton_gen<Symbol, 
                                                      epsilon_,
                                                      SymbolProperty,
                                                      StateProperty,
                                                      AutomatonProperty> nfa_type;

        typedef typename Base::Transition Transition;
        typedef typename Base::State State;

        typedef unordered_set<State> StateSet;
        typedef unordered_set<Symbol> SymbolSet;

        typedef unordered_map<State, State> State2Map;

    public:
        finite_automaton_gen(const SymbolSet alphabet = SymbolSet())
            : Base(),
              initial_state_(-1),
              alphabet_(alphabet) {}

        finite_automaton_gen(const std::initializer_list<Symbol> alphabet)
            : Base(),
              initial_state_(-1),
              alphabet_(alphabet) {}

        finite_automaton_gen(const finite_automaton_gen& x)
            : Base(x),
              initial_state_(x.initial_state_),
              final_state_set_(x.final_state_set_),
              state_set_(x.state_set_),
              alphabet_(x.alphabet_) {}

        ~finite_automaton_gen() {}

        finite_automaton_gen& 
        operator=(const finite_automaton_gen& x) {
            if (&x != this) {
                Base::operator=(x);
                initial_state_ = x.initial_state_;
                final_state_set_ = x.final_state_set_;
                state_set_ = x.state_set_;
                alphabet_ = x.alphabet_;
            }
            return *this;
        }

        virtual void clear() {
            Base::clear();
            initial_state_ = -1;
            final_state_set_.clear();
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

        void
        set_state_set(const StateSet& state_set) {
            state_set_ = state_set;
        }

        const StateSet&
        final_state_set() const {
            return final_state_set_;
        }

        void
        set_final_state_set(const StateSet& final_state_set) {
            final_state_set_ = final_state_set;
        }

        void
        clear_finale_state_set() {
            final_state_set_.clear();
        }

        const SymbolSet&
        alphabet() const {
            return alphabet_;
        }

        void
        set_alphabet(const SymbolSet& alphabet) {
            alphabet_ = alphabet;
        }

        void 
        set_alphabet(const Symbol& c) {
            alphabet_.insert(c);
        }

        void 
        set_state(State state) {
            state_set_.insert(state);
        }

        void 
        set_final_state(State state) {
            final_state_set_.insert(state);
        }

        void 
        remove_final_state(State state) {
            final_state_set_.erase(state);
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

    private:
        State initial_state_;
        StateSet final_state_set_;
        StateSet state_set_;
        SymbolSet alphabet_;
    };
};

namespace atl {
    #define FA_PARAMS typename FA_SYMBOL, long FA_EPSILON, typename FA_SYMBOL_PROP, typename FA_STATE_PROP, typename FA_AUT_PROP
    #define FA detail::finite_automaton_gen<FA_SYMBOL, FA_EPSILON, FA_SYMBOL_PROP, FA_STATE_PROP,FA_AUT_PROP>

    template <FA_PARAMS>
    inline typename FA::SymbolSet const&
    alphabet(const FA& fa) {
        return fa.alphabet();
    }

    template <FA_PARAMS>
    inline void
    set_alphabet(FA& fa,
                 typename FA::SymbolSet const& set) {
        fa.set_alphabet(set);
    }

    template <FA_PARAMS>
    inline typename FA::State
    initial_state(const FA& fa) {
        return fa.initial_state();
    }

    template <FA_PARAMS>
    inline void
    set_initial_state(FA& fa, 
                      typename FA::State state) {
        fa.set_initial_state(state);
    }

    template <FA_PARAMS>
    inline typename FA::StateSet const&
    state_set(const FA& fa) {
        return fa.state_set();
    }

    template <FA_PARAMS>
    inline void  
    set_state_set(FA& fa, 
                  typename FA::StateSet const& set) {
        fa.set_state_set(set);
    }

    template <FA_PARAMS>
    inline void  
    set_state(FA& fa, 
              typename FA::State state) {
        fa.set_state(state);
    }

    template <FA_PARAMS>
    inline typename FA::StateSet const&
    final_state_set(const FA& fa) {
        return fa.final_state_set();
    }

    template <FA_PARAMS>
    inline void  
    set_final_state_set(FA& fa, 
                        typename FA::StateSet const& set) {
        fa.set_final_state_set(set);
    }

    template <FA_PARAMS>
    inline void  
    set_final_state(FA& fa, 
                    typename FA::State state) {
        fa.set_final_state(state);
    }
    
    template <FA_PARAMS>
    inline void  
    remove_final_state(FA& fa, 
                       typename FA::State state) {
        fa.remove_final_state(state);
    }

    template <FA_PARAMS>
    inline void  
    clear_finale_state_set(FA& fa) {
        fa.clear_finale_state_set();
    }

    template <FA_PARAMS>
    inline typename FA::symbol_type
    epsilon(const FA& fa) {
        return fa.epsilon();
    }

    template <FA_PARAMS>
    inline typename FA::transition_property_type
    epsilon_transition(const FA& fa) {
        return fa.epsilon_transition();
    }

    template <FA_PARAMS>
    inline typename FA::State
    add_initial_state(FA& fa,
                      typename FA::state_property_type const& p) {
        typename FA::State s = add_state(fa, p);
        fa.set_initial_state(s);
        return s;
    }

    template <FA_PARAMS>
    inline typename FA::State
    add_initial_state(FA& fa) {
        typename FA::State s = add_state(fa);
        fa.set_initial_state(s);
        return s;
    }

    template <FA_PARAMS>
    inline typename FA::State
    add_final_state(FA& fa,
                    typename FA::state_property_type const& p) {
        typename FA::State s = add_state(fa, p);
        fa.set_final_state(s);
        return s;
    }

    template <FA_PARAMS>
    inline typename FA::State
    add_final_state(FA& fa) {
        typename FA::State s = add_state(fa);
        fa.set_final_state(s);
        return s;
    }

    template <FA_PARAMS>
    inline pair<typename FA::Transition, bool>
    add_transition(FA& fa,
                   typename FA::State s,
                   typename FA::State t,
                   typename FA::symbol_type const& c,
                   typename FA::symbol_property_type const& p) {
        return fa.add_transition(s, t, c, p);
    }
    
    template <FA_PARAMS>
    inline bool
    is_initial_state(const FA& fa,
                     typename FA::State s) {
        return (s == initial_state(fa));
    }

    template <FA_PARAMS>
    inline bool
    is_final_state(const FA& fa,
                   typename FA::State s) {
        return fa.final_state_set().count(s);
    }

    template <FA_PARAMS>
    inline bool
    has_final_state(const FA& fa,
                   typename FA::StateSet const& set) {
        typename FA::StateSet res;
        util::set_intersection(fa.final_state_set(), set, res);
        return res.size();
    }

    template <FA_PARAMS>
    inline void 
    set_forward_reachable_flag(FA& a, bool b = true) {
        a.set_flag(1, b);
        a.set_flag(0, 0);
    }

    template <FA_PARAMS>
    inline bool
    is_forward_reachable(const FA& a) {
        return (a.flag(1) & !a.flag(0));
    }

    template <FA_PARAMS>
    inline void 
    set_minimal_flag(FA& a, bool b = true) {
        a.set_flag(2, b);
        a.set_flag(1, 1);
        a.set_flag(0, 0);
    }

    template <FA_PARAMS>
    inline bool
    is_minimal(const FA& a) {
        return (a.flag(2) & !a.flag(0));
    }

    template <FA_PARAMS>
    inline void 
    set_undeterministic_flag(FA& a, bool b = true) {
        a.set_flag(3, b);
    }

    template <FA_PARAMS>
    inline bool
    is_undeterministic(const FA& a) {
        return (a.flag(3) | a.flag(4));
    }

    template <FA_PARAMS>
    inline void 
    set_epsilon_flag(FA& a, bool b = true) {
        a.set_flag(4, b);
    }

    template <FA_PARAMS>
    inline bool
    has_epsilon_transition(const FA& a) {
        return a.flag(4);
    }

    template <FA_PARAMS>
    inline bool
    is_empty(const FA& a) {
        if (is_forward_reachable(a)) {
            if (a.final_state_set().size() == 0) return true;
            return false;
        } else {
            typename FA::StateSet states;
            reachable_closure(a, states);
            if (!has_final_state(a, states)) return true;
            return false;
        }
    }
};

#endif /* atl_detail_finite_automaton_hpp */
