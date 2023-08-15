
#ifndef atl_detail_size_register_automaton_hpp
#define atl_detail_size_register_automaton_hpp

#include <fml/atl/detail/algorithm.hpp>
#include <fml/atl/detail/automaton.hpp>

#include "property.hpp"

namespace atl::detail {

  class size_register_automaton_gen
    : public automaton_gen<Guard, Modes, boost::no_property> {
  public: 
    typedef automaton_gen<Guard, Modes, boost::no_property> Base;

    typedef Base::State State;
    typedef Base::Transition Transition;
    typedef Base::StateIter StateIter;
    typedef Base::TransitionIter TransitionIter;
    typedef Base::state_property_type StateProperty;
    typedef Base::transition_property_type TransitionProperty;
    typedef Base::automaton_property_type AutomatonProperty;

    typedef std::unordered_set<State> StateSet;
    typedef std::unordered_map<State, Modes> State2Modes;

  public:
    size_register_automaton_gen()
      : Base(), register_n_(0) {}

    size_register_automaton_gen(int n)
      : Base(), register_n_(n) {}

    size_register_automaton_gen(const size_register_automaton_gen& x)
      : Base(x),
        register_n_(x.register_n_),
        state_set_(x.state_set_),
        initial_state_(x.initial_state_),
        final_state_set_(x.final_state_set_) {}

    virtual ~size_register_automaton_gen() {}

    void
    set_register(int n) {
      register_n_ = n;
    }

    int
    num_registers() const {
      return register_n_;
    }

    State
    add_state(const StateProperty& tp) {
      State s = Base::add_state(tp);
      state_set_.insert(s);
      return s;
    }

    void
    set_initial_state(State s) {
      const Modes& m = this->get_property(s);
      for(auto mi : m) assert(mi == RegisterMode::IDLE);
      initial_state_ = s;
    }

    State
    initial_state() const {
      return initial_state_;
    }

    bool
    is_initial_state(State s) const {
      return s == initial_state_;
    }

    const StateSet&
    state_set() const {
      return state_set_;
    }

    void
    set_final_state(State s) {
      final_state_set_.insert(s);
    }

    void
    set_final_state_set(StateSet& ss) {
      final_state_set_ = ss;
    }

    bool
    is_final_state(State s) const {
      return final_state_set_.find(s) !=
        final_state_set_.end();
    }

    const StateSet&
    final_state_set() const {
      return final_state_set_;
    }

    void
    clear_final_state_set() {
      final_state_set_.clear();
    }
  
  private:
    int register_n_;
    StateSet state_set_;
    State initial_state_;
    StateSet final_state_set_;

  }; /* class size_register_automaton_gen*/

}

#endif /* atl_detail_size_register_automaton_hpp */