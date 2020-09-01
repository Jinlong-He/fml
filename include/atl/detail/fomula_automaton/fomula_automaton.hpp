//
//  fomula_automaton.hpp
//  atl 
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
#include "../../../ll/atomic_proposition.hpp"

using boost::unordered_map;
using boost::unordered_set;

namespace atl::detail {
    template <class Fomula>
    class fomula_automaton_gen 
        : public automaton_gen<Fomula, ll::item, boost::no_property> {
    public:
        typedef Fomula fomula_type;
        typedef Fomula transition_property_type;
        typedef ll::item Item;
        typedef ll::variable Variable;
        typedef Item state_property_type;

        typedef automaton_gen<transition_property_type,
                              state_property_type,
                              boost::no_property> Base;

        typedef typename Base::State State;
        typedef typename Base::Transition Transition;

        typedef typename Base::StateIter StateIter;
        typedef typename Base::TransitionIter TransitionIter;
        typedef typename Base::InTransitionIter InTransitionIter;
        typedef typename Base::OutTransitionIter OutTransitionIter;

        typedef unordered_set<State> StateSet;
        typedef unordered_map<State, State> State2Map;

        typedef unordered_set<Item const*> ItemSet;
        typedef unordered_set<Variable const*> VariableSet;
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

        const VariableSet&
        variable_set() const {
            return variable_set_;
        }

        void
        set_variable_set(const VariableSet& variable_set) {
            variable_set_ = variable_set;
        }

        const VariableSet&
        control_variable_set() const {
            return control_variable_set_;
        }

        void
        set_control_variable_set(const VariableSet& control_variable_set) {
            control_variable_set_ = control_variable_set;
        }

        const VariableSet&
        input_variable_set() const {
            return input_variable_set_;
        }

        void
        set_input_variable_set(const VariableSet& input_variable_set) {
            input_variable_set_ = input_variable_set;
        }

        const std::list<ll::atomic_proposition>&
        init_list() const {
            return init_list_;
        }

        void set_init_list(const std::list<ll::atomic_proposition>& list) {
            init_list_ = list;
        }

        const unordered_map<Item const*, State>&
        item_map() const {
            return item_map_();
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
        set_variable(const state_property_type& v) {
            variable_set_.insert(dynamic_cast<const Variable*>(&v));
        }

        void
        set_control_variable(const state_property_type& v) {
            control_variable_set_.insert(dynamic_cast<const Variable*>(&v));
        }

        void
        set_input_variable(const state_property_type& v) {
            input_variable_set_.insert(dynamic_cast<const Variable*>(&v));
        }

        void 
        add_init_list(const ll::atomic_proposition& ap) {
            init_list_.push_back(ap);
        }

        State
        add_state(const state_property_type& p) {
            if (p.type() == "integer") this -> set_flag(1, 1);
            auto state = Base::add_state(p);
            set_state(state);
            item_map_[&p] = state;
            return state;
        }

        using Base::add_transition;
        pair<Transition, bool>
        add_transition(const Variable& v, const Item& i,
                       const Fomula& p) {
            auto source = -1, target = -1;
            if (item_map_.count(&v) == 0) {
                source = add_state(v);
                set_control_state(source);
            } else {
                source = item_map_.at(&v);
            }
            if (item_map_.count(&i) == 0) {
                target = add_state(i);
            } else {
                target = item_map_.at(&i);
            }
            return add_transition(source, target, p);
        }

    private:
        StateSet state_set_;
        StateSet control_state_set_;
        StateSet input_state_set_;
        VariableSet variable_set_;
        VariableSet input_variable_set_;
        VariableSet control_variable_set_;
        unordered_map<Item const*, State> item_map_;
        std::list<ll::atomic_proposition> init_list_;
    };
}

namespace atl {
    #define FOA_PARAMS typename FOMULA
    #define FOA detail::fomula_automaton_gen<FOMULA>

    template <FOA_PARAMS>
    const typename FOA::StateSet&
    control_state_set(const FOA& foa) {
        return foa.control_state_set();
    }

    template <FOA_PARAMS>
    const typename FOA::StateSet&
    input_state_set(const FOA& foa) {
        return foa.input_state_set();
    }

    template <FOA_PARAMS>
    inline const std::list<ll::atomic_proposition>& 
    init_list(const FOA& foa) {
        return foa.init_list();
    }

    template <FOA_PARAMS>
    inline typename FOA::State
    add_control_state(FOA& foa,
                      typename FOA::state_property_type const& p,
                      const ll::atomic_proposition& ap) {
        auto s = foa.add_state(p);
        foa.set_control_state(s);
        foa.set_control_variable(p);
        foa.set_variable(p);
        foa.add_init_list(ap);
        return s;
    }

    template <FOA_PARAMS>
    inline typename FOA::State
    add_input_state(FOA& foa,
                    typename FOA::state_property_type const& p) {
        auto s = add_state(foa, p);
        foa.set_input_state(s);
        foa.set_input_variable(p);
        return s;
    }

    template <FOA_PARAMS>
    inline pair<typename FOA::Transition, bool>
    add_transition(FOA& foa,
                   const ll::variable& v,
                   const ll::item& i,
                   typename FOA::fomula_type const& f) {
        return foa.add_transition(v, i, f);
    }

    template <FOA_PARAMS>
    inline void 
    set_infinite_flag(FOA& foa, bool b = true) {
        foa.set_flag(1, b);
    }

    template <FOA_PARAMS>
    inline bool
    is_infinite(const FOA& foa) {
        return foa.flag(1);
    }
}

#endif /* atl_detail_fomula_automaton_hpp */
