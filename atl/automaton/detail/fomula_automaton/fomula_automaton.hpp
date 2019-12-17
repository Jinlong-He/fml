//
//  fomula_automaton.hpp
//  ATL 
//
//  Created by Jinlong He on 2019/10/27.
//  Copyright © 2019年 Ruting-Team. All rights reserved.
//

#ifndef atl_detail_fomula_automaton_hpp 
#define atl_detail_fomula_automaton_hpp

#include <boost/unordered_set.hpp>
#include <boost/unordered_map.hpp>
#include "../../../util/util.hpp"
#include "../automaton.hpp"
#include "../no_type.hpp"
#include "../../../../ll/propositional_logic.hpp"

using boost::unordered_map;
using boost::unordered_set;

namespace atl {
    namespace detail {
        class fomula_automaton_gen 
            : public automaton_gen<ll::detail::propositional_fomula_gen, ll::detail::item_gen,
                                   ll::detail::propositional_fomula_gen> {
        public:
            typedef typename ll::detail::propositional_fomula_gen transition_property_type;
            typedef typename ll::detail::propositional_fomula_gen automaton_property_type;
            typedef typename ll::detail::item_gen state_property_type;

            typedef automaton_gen<ll::detail::propositional_fomula_gen, ll::detail::item_gen,
                                  ll::detail::propositional_fomula_gen> Base;

            typedef typename Base::State State;
            typedef typename Base::Transition Transition;

            typedef typename Base::StateIter StateIter;
            typedef typename Base::TransitionIter TransitionIter;
            typedef typename Base::InTransitionIter InTransitionIter;
            typedef typename Base::OutTransitionIter OutTransitionIter;

            typedef unordered_set<State> StateSet;

            typedef unordered_map<State, State> State2Map;
        public:
            fomula_automaton_gen()
                : Base() {}

            fomula_automaton_gen(const fomula_automaton_gen& x)
                : Base(x),
                  state_set_(x.state_set_),
                  control_state_set_(x.control_state_set_),
                  input_state_set_(x.input_state_set_) {}

            fomula_automaton_gen& 
            operator=(const fomula_automaton_gen& x) {
                if (&x != this) {
                    Base::operator=(x);
                    state_set_ = x.state_set_;
                    control_state_set_ = x.control_state_set_;
                    input_state_set_ = x.input_state_set_;
                }
                return *this;
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
            control_state_set() const {
                return control_state_set_;
            }

            void 
            set_control_state_set(const StateSet& control_state_set) {
                control_state_set_ = control_state_set;
            }

            const StateSet&
            input_state_set() const {
                return input_state_set_;
            }

            void 
            set_input_state_set(const StateSet& input_state_set) {
                input_state_set_ = input_state_set;
            }

            const std::list<ll::detail::propositional_fomula_gen>&
            init_list() const {
                return init_list_;
            }

            void set_init_list(const std::list<ll::detail::propositional_fomula_gen>& list) {
                init_list_ = list;
            }

            void 
            set_state(State state) {
                state_set_.insert(state);
            }

            void 
            set_control_state(State state) {
                control_state_set_.insert(state);
            }

            void 
            set_input_state(State state) {
                input_state_set_.insert(state);
            }

            void 
            add_init_list(const ll::detail::propositional_fomula_gen& f) {
                init_list_.push_back(f);
            }
        private:
            StateSet state_set_;
            StateSet control_state_set_;
            StateSet input_state_set_;
            std::list<ll::detail::propositional_fomula_gen> init_list_;
        };
    }

    template <typename FA>
    const typename FA::StateSet&
    control_state_set(const FA& fa) {
        return fa.control_state_set();
    }

    template <typename FA>
    const typename FA::StateSet&
    input_state_set(const FA& fa) {
        return fa.input_state_set();
    }

    template <typename FA>
    inline const std::list<ll::detail::propositional_fomula_gen>& 
    init_list(const FA& fa) {
        return fa.init_list();
    }

    template <typename FA>
    inline typename FA::State
    add_control_state(FA& fa,
                      const ll::detail::item_gen& v,
                      const ll::detail::propositional_fomula_gen& f) {
        typename FA::State s = add_state(fa, v);
        if (v.type() == "integer") set_infinite_flag(fa);
        fa.set_control_state(s);
        fa.add_init_list(f);
        return s;
    }

    template <typename FA>
    inline typename FA::State
    add_input_state(FA& fa,
                    const ll::detail::item_gen& v) {
        typename FA::State s = add_state(fa, v);
        if (v.type() == "integer") set_infinite_flag(fa);
        fa.set_input_state(s);
        return s;
    }
    
    template <typename FA>
    inline void 
    set_infinite_flag(FA& a, bool b = true) {
        a.set_flag(1, b);
    }

    template <typename FA>
    inline bool
    is_infinite(const FA& a) {
        return a.flag(1);
    }
}

#endif /* atl_detail_fomula_automaton_hpp */
