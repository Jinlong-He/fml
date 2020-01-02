//
//  automaton.hpp
//  ATL 
//
//  Created by Jinlong He on 2019/10/27.
//  Copyright © 2019年 Ruting-Team. All rights reserved.
//

#ifndef atl_detail_automaton_hpp 
#define atl_detail_automaton_hpp

#include <boost/scoped_ptr.hpp>
#include "boost/graph/adjacency_list.hpp"

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
                : graph_(new BoostGraph()) {
                //todo
            }

            automaton_gen& 
            operator=(const automaton_gen& x) {
                if (&x != this) {
                    BoostGraphPtr ptr(new BoostGraph(*x.graph_));
                    graph_.swap(ptr);
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

            virtual pair<Transition, bool>
            add_transition(State s, State t,
                           const TransitionProperty& p) {
                return boost::add_edge(s, t, p, *graph_);
            }

            virtual pair<Transition, bool>
            add_transition(State s, State t) {
                return boost::add_edge(s, t, *graph_);
            }
                            
        private:
            typedef scoped_ptr<BoostGraph> BoostGraphPtr;
            BoostGraphPtr graph_;
            Flag flag_;
        };
    }

    template <typename Automaton>
    inline typename Automaton::automaton_property_type const&
    get_property(const Automaton& a) {
        return a.get_property();
    }

    template <typename Automaton>
    inline typename Automaton::state_property_type const&
    get_property(const Automaton& a,
                 typename Automaton::State s) {
        return a.get_property(s);
    }

    template <typename Automaton>
    inline typename Automaton::transition_property_type const&
    get_property(const Automaton& a,
                 typename Automaton::Transition t) {
        return a.get_property(t);
    }

    template <typename Automaton>
    inline void
    set_property(Automaton& a,
                 typename Automaton::automaton_property_type const& p) {
        return a.set_property(p);
    }

    template <typename Automaton>
    inline void
    set_property(Automaton& a,
                 typename Automaton::State s,
                 typename Automaton::state_property_type const& p) {
        return a.set_property(s, p);
    }

    template <typename Automaton>
    inline void
    set_property(Automaton& a,
                 typename Automaton::Transition t,
                 typename Automaton::transition_property_type const& p) {
        return a.set_property(t, p);
    }

    template <typename Automaton>
    inline pair<typename Automaton::StateIter, typename Automaton::StateIter>
    states(const Automaton& a) {
        return a.states();
    }

    template <typename Automaton>
    inline typename Automaton::State
    source(const Automaton& a,
           typename Automaton::Transition t) {
        return a.source(t);
    }

    template <typename Automaton>
    inline typename Automaton::State
    target(const Automaton& a,
           typename Automaton::Transition t) {
        return a.target(t);
    }

    template <typename Automaton>
    inline pair<typename Automaton::TransitionIter, typename Automaton::TransitionIter>
    transitions(const Automaton& a) {
        return a.transitions();
    }

    template <typename Automaton>
    inline pair<typename Automaton::InTransitionIter, typename Automaton::InTransitionIter>
    in_transitions(const Automaton& a,
                   typename Automaton::State s) {
        return a.in_transitions(s);
    }

    template <typename Automaton>
    inline pair<typename Automaton::OutTransitionIter, typename Automaton::OutTransitionIter>
    out_transitions(const Automaton& a,
                    typename Automaton::State s) {
        return a.out_transitions(s);
    }

    template <typename Automaton>
    inline typename Automaton::State
    add_state(Automaton& a,
              typename Automaton::state_property_type const& p) {
        set_modified_flag(a);
        return a.add_state(p);
    }

    template <typename Automaton>
    inline typename Automaton::State
    add_state(Automaton& a) {
        set_modified_flag(a);
        return a.add_state();
    }

    template <typename Automaton>
    inline pair<typename Automaton::Transition, bool>
    add_transition(Automaton& a,
                   typename Automaton::State s,
                   typename Automaton::State t,
                   typename Automaton::transition_property_type const& p) {
        set_modified_flag(a);
        return a.add_transition(s, t, p);
    }

    template <typename Automaton>
    inline void
    clear(Automaton& a) {
        a.clear();
    }

    template <typename Automaton>
    inline void 
    set_modified_flag(Automaton& a, 
                      bool b = true) {
        a.set_flag(0, b);
    }

    template <typename Automaton>
    inline bool
    is_modified(const Automaton& a) {
        return a.flag(0);
    }
}

#endif /* atl_detail_automaton_hpp */
