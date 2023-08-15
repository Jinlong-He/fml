
#ifndef atl_size_register_automaton_hpp
#define atl_size_register_automaton_hpp

#include <fml/atl/detail/size_register_automaton/algorithm.hpp>
#include <fml/atl/detail/size_register_automaton/size_register_automaton.hpp>

namespace atl {

  using namespace detail;

  class size_register_automaton
    : public size_register_automaton_gen {

  public:
    typedef size_register_automaton_gen Base;
    
    typedef size_register_automaton sra_type;

  public:
    size_register_automaton() : Base() {}

    size_register_automaton(int n) : Base(n) {}

    size_register_automaton(const sra_type& x) : Base(x) {}

    size_register_automaton(const Base& x) : Base(x) {}

    virtual ~size_register_automaton() {}

    sra_type
    operator&(const sra_type& rhs) {
      return sra_type(product(*this, rhs));
    }

    sra_type
    operator!() {
      return sra_type(complement(*this));
    }

    sra_type
    operator+(const sra_type& rhs) {
      return sra_type(concatenate(*this, rhs));
    }

    bool
    is_nonempty() const {
      return sra_nonempty(*this);
    }

    friend std::ostream&
    operator<<(std::ostream& os, const sra_type& x) {
      os << " Deterministic : " << (is_deterministic(x) ? "Yes" : "No") << std::endl;

      os << " States :\n";
      for(auto s : x.state_set())
        os << "    " << s << " - " << x.get_property(s) << "\n";
      os << std::endl;


      os << " Initial State : " << x.initial_state() << std::endl;

      os << " Final States : ";
      for(auto s : x.final_state_set()) os << s << " ";
      os << std::endl;

      os << " Transitions :\n";
      auto transition_iter = x.transitions();
      for(auto t_it = transition_iter.first; t_it != transition_iter.second; t_it++) {
        State s = x.source(*t_it); Modes ms = x.get_property(s);
        State t = x.target(*t_it); Modes mt = x.get_property(t);
        TransitionProperty prop = x.get_property(*t_it);
        os << "   " << s << " " << ms << " " << prop << " " << t << " " << mt << std::endl;
      }
      os << std::endl;
      return os;
    }

  private:
  }; /* class size_register_automaton */

}

#endif /* atl_size_register_automaton_hpp */