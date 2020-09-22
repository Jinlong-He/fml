//
//  automaton.hpp
//  ATL 
//
//  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
//  SPDX-License-Identifier: MIT
//  Copyright (c) 2020 Jinlong He.
//

#ifndef atl_detail_automaton_hpp 
#define atl_detail_automaton_hpp

#include <boost/scoped_ptr.hpp>
#include <boost/unordered_set.hpp>
#include <boost/graph/adjacency_list.hpp>

using boost::adjacency_list;
using boost::vecS;
using boost::listS;
using boost::bidirectionalS;
using boost::graph_traits;
using boost::scoped_ptr;
using boost::graph_bundle_t;
using boost::tie;
using std::pair;
using std::string;

namespace atl {
    typedef std::size_t Flag;
    typedef std::size_t ID;

    namespace detail {
        template <typename DefaultProperty, typename ExtendedProperty>
        struct Property {
            DefaultProperty default_property;
            ExtendedProperty extended_property;

            Property() {}

            Property(const DefaultProperty& dp, const ExtendedProperty& ep)
                : default_property(dp),
                  extended_property(ep) {}

            Property(long l) 
                : default_property(l),
                  extended_property() {}

            bool operator== (const Property& p) const {
                return default_property == p.default_property && 
                       extended_property == p.extended_property;
            }

            friend std::ostream& operator<< (std::ostream& os, const Property& x) {
                os << x.default_property << " {" << x.extended_property << "}";
                return os;
            }
        };

        template <class TransitionProperty, 
                  class StateProperty, 
                  class AutomatonProperty>
        class automaton_gen {
        public:
            typedef StateProperty state_property_type;
            typedef AutomatonProperty automaton_property_type;
            typedef TransitionProperty transition_property_type;

            typedef adjacency_list<listS, vecS, bidirectionalS, 
                                   StateProperty, TransitionProperty, AutomatonProperty, 
                                   listS> BoostGraph;

            typedef typename graph_traits<BoostGraph>::edge_descriptor Transition;
            typedef typename graph_traits<BoostGraph>::edge_iterator TransitionIter;
            typedef typename graph_traits<BoostGraph>::in_edge_iterator InTransitionIter;
            typedef typename graph_traits<BoostGraph>::out_edge_iterator OutTransitionIter;
            typedef typename graph_traits<BoostGraph>::vertex_descriptor State;
            typedef typename graph_traits<BoostGraph>::vertex_iterator StateIter;

        public:
            automaton_gen(const AutomatonProperty& p = AutomatonProperty())
                : graph_(new BoostGraph(p)),
                  flag_(0) {}

            automaton_gen(const automaton_gen& x)
                : graph_(new BoostGraph(*x.graph_)),
                  flag_(x.flag_) {
            }

            virtual ~automaton_gen() {}

            automaton_gen& 
            operator=(const automaton_gen& x) {
                if (&x != this) {
                    BoostGraphPtr ptr(new BoostGraph(*x.graph_));
                    graph_.swap(ptr);
                    flag_ = x.flag_;
                }
                return *this;
            }

            virtual void clear() {
                if (flag_ == 0) return;
                graph_ -> clear();
                flag_ = 0;
            }

            Flag 
            flag() const {
                return flag_;
            }

            bool 
            flag(ID i) const {
                return (flag_ & (1 << i)) == (1 << i);
            }

            void 
            set_flag(Flag flag) {
                flag_ = flag;
            }

            void 
            set_flag(ID i, bool b) {
                flag_ = b ? (flag_ | (1 << i)) : (flag_ & ~(1 << i));
            }

            const AutomatonProperty&
            get_property() const {
                return (*graph_)[graph_bundle_t()];
            }

            const TransitionProperty&
            get_property(Transition t) const {
                return (*graph_)[t];
            }

            const StateProperty&
            get_property(State s) const {
                return (*graph_)[s];
            }
            
            void
            set_property(const AutomatonProperty& p) {
                (*graph_)[graph_bundle_t()] = p;
            }

            void
            set_property(Transition t, 
                         const TransitionProperty& p) {
                (*graph_)[t] = p;
            }

            void
            set_property(State s, 
                         const StateProperty& p) {
                (*graph_)[s] = p;
            }

            pair<StateIter, StateIter>
            states() const {
                return boost::vertices(*graph_);
            }

            State
            source(Transition t) const {
                return boost::source(t, *graph_);
            }

            State
            target(Transition t) const {
                return boost::target(t, *graph_);
            }

            pair<TransitionIter, TransitionIter>
            transitions() const {
                return boost::edges(*graph_);
            }

            pair<InTransitionIter, InTransitionIter>
            in_transitions(State s) const {
                return boost::in_edges(s, *graph_);
            }

            pair<OutTransitionIter, OutTransitionIter>
            out_transitions(State s) const {
                return boost::out_edges(s, *graph_);
            }

            virtual State 
            add_state(const StateProperty& p) {
                return boost::add_vertex(p, *graph_);
            }

            virtual State 
            add_state() {
                return boost::add_vertex(*graph_);
            }

            virtual void 
            clear_state(State s) {
                return boost::clear_vertex(s, *graph_);
            }
                            
            virtual void 
            clear_out_transitions(State s) {
                return boost::clear_out_edges(s, *graph_);
            }
                            
            virtual void 
            clear_in_transitions(State s) {
                return boost::clear_in_edges(s, *graph_);
            }
                            
            virtual void 
            remove_state(State s) {
                return boost::remove_vertex(s, *graph_);
            }

            virtual pair<Transition, bool>
            add_transition(State s, State t,
                           const TransitionProperty& p) {
                return boost::add_edge(s, t, p, *graph_);
            }

            virtual pair<Transition, bool>
            add_transition(State s, State t) {
                return boost::add_edge(s, t, *graph_);
            }

            virtual void
            remove_transition(Transition t) {
                return boost::remove_edge(t, *graph_);
            }

            virtual void
            remove_transition(State s, State t) {
                return boost::remove_edge(s, t, *graph_);
            }
        private:
            typedef scoped_ptr<BoostGraph> BoostGraphPtr;
            BoostGraphPtr graph_;
            Flag flag_;
        };
    }

    #define AUTOMATON_PARAMS typename TP, typename SP, typename AP
    #define AUTOMATON detail::automaton_gen<TP, SP, AP>

    template <AUTOMATON_PARAMS>
    inline const AP&
    get_property(const AUTOMATON& a) {
        return a.get_property();
    }

    template <AUTOMATON_PARAMS>
    inline const SP&
    get_property(const AUTOMATON& a,
                 typename AUTOMATON::State s) {
        return a.get_property(s);
    }

    template <AUTOMATON_PARAMS>
    inline const TP&
    get_property(const AUTOMATON& a,
                 typename AUTOMATON::Transition t) {
        return a.get_property(t);
    }

    template <AUTOMATON_PARAMS>
    inline void
    set_property(AUTOMATON& a,
                 const AP& p) {
        return a.set_property(p);
    }

    template <AUTOMATON_PARAMS>
    inline void
    set_property(AUTOMATON& a,
                 typename AUTOMATON::State s,
                 const SP& p) {
        return a.set_property(s, p);
    }

    template <AUTOMATON_PARAMS>
    inline void
    set_property(AUTOMATON& a,
                 typename AUTOMATON::Transition t,
                 const TP& p) {
        return a.set_property(t, p);
    }

    template <AUTOMATON_PARAMS>
    inline pair<typename AUTOMATON::StateIter, 
                typename AUTOMATON::StateIter>
    states(const AUTOMATON& a) {
        return a.states();
    }

    template <AUTOMATON_PARAMS>
    inline typename AUTOMATON::State
    source(const AUTOMATON& a,
           typename AUTOMATON::Transition t) {
        return a.source(t);
    }

    template <AUTOMATON_PARAMS>
    inline typename AUTOMATON::State
    target(const AUTOMATON& a,
           typename AUTOMATON::Transition t) {
        return a.target(t);
    }

    template <AUTOMATON_PARAMS>
    inline pair<typename AUTOMATON::TransitionIter, 
                typename AUTOMATON::TransitionIter>
    transitions(const AUTOMATON& a) {
        return a.transitions();
    }

    template <AUTOMATON_PARAMS>
    inline pair<typename AUTOMATON::InTransitionIter, 
                typename AUTOMATON::InTransitionIter>
    in_transitions(const AUTOMATON& a,
                   typename AUTOMATON::State s) {
        return a.in_transitions(s);
    }

    template <AUTOMATON_PARAMS>
    inline pair<typename AUTOMATON::OutTransitionIter, 
                typename AUTOMATON::OutTransitionIter>
    out_transitions(const AUTOMATON& a,
                    typename AUTOMATON::State s) {
        return a.out_transitions(s);
    }

    template <AUTOMATON_PARAMS>
    inline void 
    set_modified_flag(AUTOMATON& a, 
                      bool b = true) {
        a.set_flag(0, b);
    }

    template <AUTOMATON_PARAMS>
    inline bool
    is_modified(const AUTOMATON& a) {
        return a.flag(0);
    }

    template <AUTOMATON_PARAMS>
    inline typename AUTOMATON::State
    add_state(AUTOMATON& a,
              const SP& p) {
        set_modified_flag(a);
        return a.add_state(p);
    }

    template <AUTOMATON_PARAMS>
    inline typename AUTOMATON::State
    add_state(AUTOMATON& a) {
        set_modified_flag(a);
        return a.add_state();
    }

    template <AUTOMATON_PARAMS>
    inline void
    clear_state(AUTOMATON& a,
                typename AUTOMATON::State s) {
        set_modified_flag(a);
        return a.clear_state(s);
    }

    template <AUTOMATON_PARAMS>
    inline void
    clear_out_transitions(AUTOMATON& a,
                         typename AUTOMATON::State s) {
        set_modified_flag(a);
        return a.clear_out_transitions(s);
    }

    template <AUTOMATON_PARAMS>
    inline void
    clear_in_transitions(AUTOMATON& a,
                         typename AUTOMATON::State s) {
        set_modified_flag(a);
        return a.clear_in_transitions(s);
    }

    template <AUTOMATON_PARAMS>
    inline void
    remove_state(AUTOMATON& a,
                 typename AUTOMATON::State s) {
        set_modified_flag(a);
        return a.remove_state(s);
    }

    template <AUTOMATON_PARAMS>
    inline pair<typename AUTOMATON::Transition, bool>
    add_transition(AUTOMATON& a,
                   typename AUTOMATON::State s,
                   typename AUTOMATON::State t,
                   const TP& p) {
        set_modified_flag(a);
        return a.add_transition(s, t, p);
    }

    template <AUTOMATON_PARAMS>
    inline void
    remove_transition(AUTOMATON& a,
                      typename AUTOMATON::Transition t) {
        return a.remove_transition(t);
    }

    template <AUTOMATON_PARAMS>
    inline void
    remove_transition(AUTOMATON& a,
                      typename AUTOMATON::State s,
                      typename AUTOMATON::State t) {
        return a.remove_transition(s, t);
    }

    template <AUTOMATON_PARAMS>
    inline void
    clear(AUTOMATON& a) {
        a.clear();
    }
}

#endif /* atl_detail_automaton_hpp */
