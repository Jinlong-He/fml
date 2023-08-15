
#ifndef atl_detail_property_hpp
#define atl_detail_property_hpp

#include <iostream>
#include <map>
#include <utility>

#include <fml/ll/propositional_logic.hpp>

using namespace ll;

namespace atl::detail {

  struct Guard {
    
    typedef std::pair<int, int> Bound;

    propositional_fomula guard;

    Guard() {
      int_variable cur("cur");
      guard = (cur <= -1) & (cur >= -1);
    }

    Guard(const propositional_fomula& formula) {
      guard = formula; minimize();
    }

    Guard
    operator&(const Guard& rhs) const {
      Guard n_guard(guard & rhs.guard);
      n_guard.minimize();
      return n_guard;
    }

    Guard
    operator&(const propositional_fomula& rhs) {
      if(!check_formula(rhs)) assert(false);
      Guard n_guard(guard & rhs);
      n_guard.minimize();
      return n_guard;
    }
    
    // phi /\ psi is consisitent
    bool
    operator==(const Guard& rhs) const {
      return is_compatible_with(rhs);
    }
    
    bool
    operator!=(const Guard& rhs) const {
      return !is_compatible_with(rhs);
    }

    // used for unordered_set
    bool
    operator<(const Guard& rhs) const {
      return guard.to_string() < rhs.guard.to_string();
    }

    bool
    is_compatible_with(const Guard& rhs) const {
      Bound b1 = get_bounds();
      if(b1.first == -1) return false;
      Bound b2 = rhs.get_bounds();
      int top = std::max(b1.first, b1.second);
      top = std::max(top, std::max(b2.first, b2.second)) + 1;
      if(b1.second == -1) b1.second = top;
      if(b2.second == -1) b2.second = top;
      if(b1.second < b2.first || b2.second < b1.first)
        return false;
      return true;
    }
    
    void
    minimize() {
      Bound bound = parse_bounds(guard);
      int_variable cur("cur");
      propositional_fomula n_guard = cur >= bound.first;
      if(bound.second != -1)
        n_guard = n_guard & (cur <= bound.second);
      guard = n_guard;
    }

    Bound
    get_bounds() const {
      return parse_bounds(guard);
    }

    friend std::ostream&
    operator<<(std::ostream& os, const Guard& x) {
      os << x.guard;
      return os;
    }

  private:

    bool
    check_formula(const propositional_fomula& f) {
      if(f.logical_operator() == "&")
        return check_formula(f.lhs()) &&
                check_formula(f.rhs());
      const string& pred = f.predicate();
      if(pred != "<=" && pred != ">=")
        return false;
      const atomic_proposition& ap = f;
      if(ap.lhs().identifier() != "cur")
          return false;
      if(ap.rhs().type() != "integer")
          return false;
      return true;
    }

    Bound
    parse_bounds(
      const propositional_fomula& g) const {
      int l = 0, r = -1;
      const atomic_proposition* ap = &g;
      if(g.logical_operator() == "&") {
        Bound b1 = parse_bounds(g.lhs());
        Bound b2 = parse_bounds(g.rhs());
        l = std::max(b1.first, b2.first);
        if(b1.second == -1) r = b2.second;
        else if(b2.second == -1) r = b1.second;
        else r = std::min(b1.second, b2.second);
        if(l > r) l = r = -1;
      } else if(g.predicate() == "<=" || g.predicate() == ">=") {
        assert(ap->rhs().type() == "integer");
        const int b = std::stoi(ap->rhs().identifier());
        if(g.predicate() == "<=")
          if(r == -1) r = b;
          else r = (b == -1 ? r : std::min(r, b));
        else
          l = std::max(l, b);
      } else assert(false);
      return std::make_pair(l, r);
    }

  };

  enum RegisterMode { IDLE, COUNT };
  typedef std::vector<RegisterMode> Modes;
  typedef std::pair<Modes, Modes> ModesTr;

  Modes
  operator+(const Modes& lhs, const Modes& rhs) {
    Modes out = lhs;
    for(auto reg_mode : rhs)
      out.push_back(reg_mode);
    return out;
  }

  Modes
  operator&(const Modes& lhs, const Modes& rhs) {
    return lhs + rhs;
  }

  std::ostream&
  operator<<(std::ostream& os, const Modes& x) {
    os << "(";
    for(int i = 0; i < x.size(); i++)
      os << (x[i] == RegisterMode::IDLE ? "IDLE" : "COUNT")
         << (i < (x.size() - 1) ? ", " : "");
    os << ")";
    return os;
  }

  ModesTr
  operator+(const ModesTr& lhs, const ModesTr& rhs) {
    return std::make_pair(lhs.first + rhs.first, lhs.second + rhs.second);
  }

  ModesTr
  operator&(const ModesTr& lhs, const ModesTr& rhs) {
    return lhs + rhs;
  }

  bool
  operator==(
    const std::pair<Modes, Modes>& lhs,
    const std::pair<Modes, Modes>& rhs) {
    return lhs.first.size() == rhs.first.size();
  }

  std::ostream&
  operator<<(std::ostream& os, const ModesTr& x) {
    os << x.first << " --> " << x.second;
    return os;
  }

  Modes
  binary_to_modes(int bin, int n) {
    Modes modes;
    for(int i = 0; i < n; i++) {
      modes.push_back(((bin >> i) & 1) ?
        RegisterMode::COUNT : RegisterMode::IDLE);
    }
    return modes;
  }

}

namespace std {
  template<> struct hash<atl::detail::Guard> {
    std::size_t operator()(atl::detail::Guard const& x) const {
      return hash<string>{}(x.guard.to_string());
    }
  };
}

#endif /* atl_detail_property_hpp */