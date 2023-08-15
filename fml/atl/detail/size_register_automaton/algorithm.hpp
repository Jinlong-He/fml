
#ifndef atl_detail_size_register_automaton_algorithm_hpp
#define atl_detail_size_register_automaton_algorithm_hpp

#include <queue>

#include <fml/atl/finite_automaton/nondeterministic_finite_automaton.hpp>

#include "size_register_automaton.hpp"

namespace atl::detail {

  #define SRA size_register_automaton_gen
  
  typedef SRA::State State;
  typedef SRA::Transition Transition;
  typedef SRA::TransitionProperty TransitionProperty;
  
  typedef Guard::Bound Bound;
  typedef std::vector<Bound> Bounds;

  bool
  is_self_triggered(
    const Guard& g1, const ModesTr& mt1,
    const Guard& g2, const ModesTr& mt2) {
    if(g1 != g2) return true;
    const int n = mt1.first.size();
    assert(n == mt2.first.size());
    for(int i = 0; i < n; i++)
      if(mt1.first[i] != mt2.first[i]) return true;
    assert(n == mt1.second.size());
    assert(n == mt2.second.size());
    for(int i = 0; i < n; i++)
      if(mt1.first[i] == RegisterMode::COUNT
          && mt1.second[i] != mt2.second[i])
        return true;
    return false;
  }

  Bounds
  intersect_bounds(const Bounds& bounds, Bound b) {
    Bounds n_bounds;
    for(auto bp : bounds) {
      Bound nb = std::make_pair(
        std::max(b.first, bp.first), -1);
      if(bp.second != -1 || b.second != -1) {
        if(b.second == -1) {
          nb.second = bp.second;
        } else if(bp.second == -1) {
          nb.second = b.second;
        } else {
          nb.second = std::min(b.second, bp.second);
        }
      }
      if(nb.first > nb.second) continue;
      n_bounds.push_back(nb);
    }
    return n_bounds;
  }

  // Check deterministc
  bool
  is_deterministic(const SRA& sra) {
    auto state_iter = sra.states();
    for(auto it = state_iter.first; it != state_iter.second; it++) {
      auto out_edge_iter = sra.out_transitions(*it);
      for(auto oet1 = out_edge_iter.first; oet1 != out_edge_iter.second; oet1++)
        for(auto oet2 = out_edge_iter.first; oet2 != out_edge_iter.second; oet2++)
          if(oet1 != oet2) {
            const Guard& g1 = sra.get_property(*oet1);
            const Guard& g2 = sra.get_property(*oet2);
            Modes m1 = sra.get_property(oet1->m_source);
            Modes m1p = sra.get_property(oet1->m_target);
            Modes m2 = sra.get_property(oet2->m_source);
            Modes m2p = sra.get_property(oet2->m_target);
            if(!is_self_triggered(
              g1, std::make_pair(m1, m1p),
              g2, std::make_pair(m2, m2p)))
              return false;
          }
    }
    return true;
  }

  bool
  check_count_to_idle(const Modes& m1, const Modes& m2) {
    assert(m1.size() == m2.size());
    for(int i = 0; i < m1.size(); i++)
      if(m1[i] == RegisterMode::IDLE &&
          m2[i] == RegisterMode::COUNT)
        return false;
    return true;
  }

  SRA
  get_full_sra(const SRA& sra) {
    SRA fsra = sra;
    
    std::unordered_map<int, State> qm_map;
    int n_reg = sra.num_registers();
    int n_qm = (1 << n_reg);
    for(int i = 0; i < n_qm; i++) {
      Modes m = binary_to_modes(i, n_reg);
      qm_map[i] = fsra.add_state(m);
    }
    
    int_variable cur("cur");
    propositional_fomula top = cur >= 0;
    for(int i = 0; i < n_qm; i++) {
      Modes m1 = binary_to_modes(i, n_reg);
      for(int j = 0; j < n_qm; j++) {
        Modes m2 = binary_to_modes(j, n_reg);
        bool ok_mode_tr = true;
        for(int k = 0; k < n_reg; k++)
          if(m1[k] == RegisterMode::IDLE &&
            m2[k] == RegisterMode::COUNT)
            ok_mode_tr = false;
        if(!ok_mode_tr) continue;
        fsra.add_transition(qm_map[i], qm_map[j], top);
      }
    }

    // compute the negation of each transition
    auto state_iter_pair = sra.states();
    for(auto s_it = state_iter_pair.first;
      s_it != state_iter_pair.second; s_it++) {
      Modes ms = sra.get_property(*s_it);
      for(int i = 0; i < n_qm; i++) {
        Modes m1p = binary_to_modes(i, n_reg);
        if(!check_count_to_idle(ms, m1p)) continue;
        const Guard& g1 = top;
        const ModesTr& mt1 = std::make_pair(ms, m1p);
        auto out_edge_iter = sra.out_transitions(*s_it);
        std::vector<Bound> bounds;
        bounds.push_back(std::make_pair(0, -1));
        for(auto oet = out_edge_iter.first;
          oet != out_edge_iter.second && !bounds.empty(); oet++) {
          const Guard& g2 = sra.get_property(*oet);
          Modes m2p = sra.get_property(oet->m_target);
          if(is_self_triggered(
            g1, mt1, g2, std::make_pair(ms, m2p)
          )) continue;
          Bound b = g2.get_bounds();
          Bounds nb1, nb2;
          if(b.first > 0)
            nb1 = intersect_bounds(bounds, std::make_pair(0, b.first - 1));
          if(b.second != -1)
            nb2 = intersect_bounds(bounds, std::make_pair(b.second + 1, -1));
          bounds.clear();
          for(auto nb : nb1) bounds.push_back(nb);
          for(auto nb : nb2) bounds.push_back(nb);
        }
        for(auto b : bounds) {
          Guard guard(cur >= b.first);
          if(b.second != -1)
            guard = guard & (cur <= b.second);
          fsra.add_transition(*s_it, qm_map[i], guard);
        }
      }
    }
    return fsra;
  }

  // Complement
  typedef size_register_automaton_gen::StateSet StateSet;
  SRA
  complement(const SRA& sra) {
    assert(is_deterministic(sra));
    SRA fsra = get_full_sra(sra);
    StateSet n_final_state_set;
    for(auto s : fsra.state_set())
      if(!fsra.is_final_state(s))
        n_final_state_set.insert(s);
    fsra.set_final_state_set(n_final_state_set);
    return fsra;
  }

  // Product
  SRA
  product(const SRA& lhs, const SRA& rhs) {
    SRA out;
    auto state2map = detail::product(lhs, rhs, out);

    out.set_register(lhs.num_registers() + rhs.num_registers());
    for(auto sp : state2map) {
      if(lhs.is_initial_state(sp.first.first) &&
          rhs.is_initial_state(sp.first.second))
        out.set_initial_state(sp.second);
      if(lhs.is_final_state(sp.first.first) &&
          rhs.is_final_state(sp.first.second))
        out.set_final_state(sp.second);
    }
    return out;
  }

  // Concatenate
  typedef size_register_automaton_gen::StateSet StateSet;

  SRA
  concatenate(const SRA& lhs, const SRA& rhs) {
    SRA out(lhs.num_registers() + rhs.num_registers());

    Modes lhs_idles, rhs_idles;
    for(int i = 0; i < lhs.num_registers(); i++)
      lhs_idles.push_back(RegisterMode::IDLE);
    for(int i = 0; i < rhs.num_registers(); i++)
      rhs_idles.push_back(RegisterMode::IDLE);

    std::unordered_map<State, State> lhs_state2state;
    std::unordered_map<State, State> rhs_state2state;
    for(auto state : lhs.state_set()) {
      Modes m = lhs.get_property(state);
      lhs_state2state[state] = out.add_state(m + rhs_idles);
    }
    for(auto state : rhs.state_set()) {
      if(rhs.is_initial_state(state)) continue;
      Modes m = rhs.get_property(state);
      rhs_state2state[state] = out.add_state(lhs_idles + m);
    }

    out.set_initial_state(lhs_state2state[lhs.initial_state()]);

    for(auto final_state : rhs.final_state_set()) {
      if(!rhs.is_initial_state(final_state)) {
        State n_final_state = rhs_state2state[final_state];
        out.set_final_state(n_final_state);
      } else for(auto lhs_final_state : lhs.final_state_set()) {
        State n_final_state = lhs_state2state[lhs_final_state];
        out.set_final_state(n_final_state);
      }
    }
    
    auto lhs_transition_iter = lhs.transitions();
    for(auto t_it = lhs_transition_iter.first;
      t_it != lhs_transition_iter.second; t_it++) {
        State ns = lhs_state2state[lhs.source(*t_it)];
        State nt = lhs_state2state[lhs.target(*t_it)];
        out.add_transition(ns, nt, lhs.get_property(*t_it)); 
    }

    auto rhs_transition_iter = rhs.transitions();
    for(auto t_it = rhs_transition_iter.first;
      t_it != rhs_transition_iter.second; t_it++) {
        State s = rhs.source(*t_it);
        State t = rhs.target(*t_it);
        StateSet ns_set, nt_set;
        if(!rhs.is_initial_state(s)) ns_set.insert(rhs_state2state[s]);
        else for(auto lhs_final_state : lhs.final_state_set())
          ns_set.insert(lhs_state2state[lhs_final_state]);
        if(!rhs.is_initial_state(t)) nt_set.insert(rhs_state2state[t]);
        else for(auto lhs_final_state : lhs.final_state_set())
          nt_set.insert(lhs_state2state[lhs_final_state]);
        for(auto ns : ns_set) for(auto nt : nt_set)
          out.add_transition(ns, nt, rhs.get_property(*t_it));
    }

    return out;
  }

  // Checking emptiness
  enum CA { EQ, GE };
  typedef std::pair<CA, int> CARegister;
  typedef std::vector<CARegister> CARegisterSet;

  typedef std::pair<State, CARegisterSet> CAState;
  
  int max_bound(const SRA& sra) {
    int maxn = 0;
    auto transition_iter = sra.transitions();
    for(auto t_it = transition_iter.first;
          t_it != transition_iter.second; t_it++) {
      const Guard& g = sra.get_property(*t_it);
      const Bound b = g.get_bounds();
      maxn = std::max(maxn, std::max(b.first, b.second));
    }
    return maxn;
  }

  bool
  is_compatible(
    const CARegisterSet& cars,
    const Modes& m,
    const Modes& mp
  ) {
    assert(cars.size() == m.size());
    assert(m.size() == mp.size());
    for(int i = 0; i < m.size(); i++) {
      if(m[i] == RegisterMode::IDLE) {
        if(cars[i].first != CA::EQ || cars[i].second != 0)
          return false;
      } else if(mp[i] == RegisterMode::IDLE) {
        if(cars[i].second != 1)
          return false;
      } else if(cars[i].second < 2)
        return false;
    }
    return true;
  }

  bool
  has_idle_to_count(const Modes& m, const Modes& mp) {
    assert(m.size() == mp.size());
    for(int i = 0; i < m.size(); i++)
      if(m[i] == RegisterMode::IDLE &&
          mp[i] == RegisterMode::COUNT)
        return true;
    return false;
  }

  std::vector<CARegisterSet>
  compute_nu_set(
    const int n_reg,
    const Guard& guard,
    const CARegisterSet& mu,
    const Modes& m,
    const Modes& mp
  ) {
    assert(n_reg == m.size());
    assert(n_reg == mp.size());
    assert(n_reg == mu.size());
    std::vector<CARegisterSet> nu_set;
    nu_set.push_back(CARegisterSet());
    for(int i = 0; i < n_reg; i++) {
      if(m[i] == RegisterMode::IDLE && mp[i] == RegisterMode::IDLE) {
        for(auto& nu : nu_set) nu.push_back(std::make_pair(CA::EQ, 0));
      } else if(m[i] == RegisterMode::COUNT && mp[i] == RegisterMode::IDLE) {
        for(auto& nu : nu_set) nu.push_back(std::make_pair(CA::EQ, 0));
      } else if(m[i] == RegisterMode::COUNT && mp[i] == RegisterMode::COUNT) {
        if(mu[i].first == CA::EQ)
          for(auto& nu : nu_set)
            nu.push_back(std::make_pair(CA::EQ, mu[i].second - 1));
        else {
          int n = nu_set.size();
          for(int j = 0; j < n; j++) {
            CARegisterSet n_nu = nu_set[j];
            n_nu.push_back(std::make_pair(CA::EQ, mu[i].second - 1));
            nu_set.push_back(n_nu);
            nu_set[j].push_back(std::make_pair(CA::GE, mu[i].second));
          }
        }
      } else { // IDLE -> COUNT
        Bound b = guard.get_bounds();
        if(b.second != -1) {
          int n = nu_set.size();
          for(int k = 0; k < n; k++) {
            CARegisterSet n_nu = nu_set[k];
            for(int j = b.first; j <= b.second; j++) {
              if(j == b.first)
                nu_set[k].push_back(std::make_pair(CA::EQ, j));
              else {
                n_nu.push_back(std::make_pair(CA::EQ, j));
                nu_set.push_back(n_nu);
                n_nu.pop_back();
              }
            }
          }
        } else {
          if(b.first > 1)
            for(auto& nu : nu_set)
              nu.push_back(std::make_pair(CA::GE, b.first));
          else {
            int n = nu_set.size();
            for(int j = 0; j < n; j++) {
              CARegisterSet n_nu = nu_set[j];
              n_nu.push_back(std::make_pair(CA::EQ, 1));
              nu_set.push_back(n_nu);
              nu_set[j].push_back(std::make_pair(CA::GE, 2));
            }
          }
        }
      }
    }
    return nu_set;
  }

  SRA
  sra_to_insra(const SRA& sra) {
    propositional_fomula top = int_variable("cur") >= 0;

    const int n_reg = sra.num_registers();
    SRA insra(n_reg);
    int maxn = max_bound(sra);
    
    std::map<CAState, State> castate_map;
    CAState q0_p;
    q0_p.first = sra.initial_state();
    for(int i = 0; i < n_reg; i++)
      q0_p.second.push_back(std::make_pair(CA::EQ, 0));
    Modes idles;
    for(int i = 0; i < n_reg; i++)
      idles.push_back(RegisterMode::IDLE);
    castate_map[q0_p] = insra.add_state(idles);
    insra.set_initial_state(castate_map[q0_p]);

    std::queue<CAState> castates_queue;
    castates_queue.push(q0_p);
    while(!castates_queue.empty()) {
      CAState cas = castates_queue.front(); castates_queue.pop();
      State s = cas.first;
      const CARegisterSet& mu = cas.second;
      const State in_s = castate_map[cas];

      if(sra.is_final_state(s))
        insra.set_final_state(castate_map[cas]);
      
      auto out_edges_iter = sra.out_transitions(cas.first);
      for(auto oet = out_edges_iter.first;
            oet != out_edges_iter.second; oet++) {
        const Modes& m = sra.get_property(s);
        State t = sra.target(*oet);
        const Modes& mp = sra.get_property(t);
        if(!is_compatible(cas.second, m, mp)) continue;
        Guard in_g;
        if(has_idle_to_count(m, mp)) in_g = Guard(top);
        else in_g = sra.get_property(*oet);
        
        std::vector<CARegisterSet> nu_set
          = compute_nu_set(
            n_reg, sra.get_property(*oet), mu, m, mp);

        for(auto nu : nu_set) {
          CAState ca_t = std::make_pair(t, nu);
          if(castate_map.find(ca_t) == castate_map.end()) {
            castate_map[ca_t] = insra.add_state(mp);
            castates_queue.push(ca_t);
          }
          State in_t = castate_map[ca_t];
          insra.add_transition(in_s, in_t, in_g);
        }
      }
    }
    return insra;    
  }

  typedef nondeterministic_finite_automaton<Guard> nfa_sra;
  typedef int Register;

  struct EquivalenceClass {
    std::vector<Register> eq_rel;

    EquivalenceClass(int n) {
      eq_rel.resize(n);
      for(int i = 0; i < n; i++)
        eq_rel[i] = i;
    }

    EquivalenceClass(const EquivalenceClass& x)
      : eq_rel(x.eq_rel) {}

    bool
    operator==(const EquivalenceClass& rhs) const {
      EquivalenceClass eq1(*this), eq2(rhs);
      for(int i = 0; i < eq_rel.size(); i++)
        if(eq1.find_class(i) != eq2.find_class(i))
          return false;
      return true;
    }

    bool
    operator<(const EquivalenceClass& rhs) const {
      EquivalenceClass eq1(*this), eq2(rhs);
      for(int i = 0; i < eq_rel.size(); i++) {
        Register c1 = eq1.find_class(i);
        Register c2 = eq2.find_class(i);
        if(c1 < c2) return true;
        if(c1 > c2) return false;
      }
      return false;
    }

    void
    merge(Register r1, Register r2) {
      Register c1 = find_class(r1);
      Register c2 = find_class(r2);
      if(c1 == c2) return;
      if(c1 < c2) eq_rel[c2] = c1;
      else eq_rel[c1] = c2;
    }

    void
    split(Register r) {
      Register c = find_class(r);
      for(int i = 0; i < eq_rel.size(); i++)
        if(find_class(i) == c)
          eq_rel[i] = i;
    }

    Register
    find_class(Register r) {
      return (r == eq_rel[r] ? r :
        eq_rel[r] = find_class(eq_rel[r]));
    }

  };

  bool
  is_compatible(const Modes& m, EquivalenceClass& eq) {
    int n = m.size();
    assert(eq.eq_rel.size() == n);
    for(int i = 0; i < n; i++)
      for(int j = i + 1; j < n; j++)
        if(eq.find_class(i) == eq.find_class(j))
          if(m[i] != m[j]) return false;
    return true;
  }

  typedef std::pair<State, EquivalenceClass> EQState;

  nfa_sra
  insra_to_nfa(const SRA& insra) {
    int n_reg = insra.num_registers();

    std::map<EQState, State> eqstate_nfastate_map;
    State insra_q0 = insra.initial_state();
    EquivalenceClass sim_id(n_reg);
    EQState eqs_ini = std::make_pair(insra_q0, sim_id);

    nfa_sra nfa;
    State q0 = nfa.add_state();
    nfa.set_initial_state(q0);
    eqstate_nfastate_map[eqs_ini] = q0;

    std::queue<EQState> eqstates_queue;
    eqstates_queue.push(eqs_ini);
    while(!eqstates_queue.empty()) {
      EQState eqs = eqstates_queue.front();
      eqstates_queue.pop();
      State s = eqs.first;
      EquivalenceClass& sim = eqs.second;
      const Modes& m = insra.get_property(s);

      assert(is_compatible(m, sim));

      if(insra.is_final_state(s))
        nfa.set_final_state(eqstate_nfastate_map[eqs]);

      auto out_edges_iter = insra.out_transitions(s);
      for(auto oet = out_edges_iter.first;
            oet != out_edges_iter.second; oet++) {
        State t = insra.target(*oet);
        Guard symbol = insra.get_property(*oet);
        const Modes& mp = insra.get_property(t);
        EquivalenceClass simp(sim);

        std::vector<Register> idle_to_count;
        for(int i = 0; i < n_reg; i++)
          if(m[i] == RegisterMode::IDLE && mp[i] == RegisterMode::COUNT)
            idle_to_count.push_back(i);
        for(int i = 1; i < idle_to_count.size(); i++)
          simp.merge(idle_to_count[0], idle_to_count[i]);
        
        for(int i = 0; i < n_reg; i++)
          if(m[i] == RegisterMode::COUNT && mp[i] == RegisterMode::IDLE)
            simp.split(i);
        
        if(!is_compatible(mp, simp)) continue;

        EQState eqt = std::make_pair(t, simp);
        if(eqstate_nfastate_map.find(eqt) == eqstate_nfastate_map.end()) {
          eqstate_nfastate_map[eqt] = nfa.add_state();
          eqstates_queue.push(eqt);
        }
        State nt = eqstate_nfastate_map[eqt];
        nfa.add_transition(s, nt, symbol);
      }
    }
    return nfa;
  }

  nfa_sra
  sra_to_nfa(const SRA& sra) {
    return insra_to_nfa(sra_to_insra(sra));
  }

  bool
  sra_nonempty(const SRA& sra) {
    nfa_sra nfa = sra_to_nfa(sra);
    
    std::unordered_map<State, bool> has_vis;
    std::queue<State> states_queue;
    states_queue.push(nfa.initial_state());
    while(!states_queue.empty()) {
      State s = states_queue.front(); states_queue.pop();
      if(has_vis[s]) continue;
      has_vis[s] = true;

      if(nfa.final_state_set().find(s)
          != nfa.final_state_set().end())
        return true;
      auto out_edges_iter = nfa.out_transitions(s);
      for(auto oet = out_edges_iter.first;
            oet != out_edges_iter.second; oet++) {
        State t = nfa.target(*oet);
        if(has_vis[t]) continue;
        states_queue.push(t);
      }
    }
    return false;
  }

}

#endif /* atl_detail_size_register_automaton_algorithm_hpp */