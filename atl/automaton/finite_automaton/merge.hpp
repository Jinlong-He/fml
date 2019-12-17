//
//  merge.hpp
//  ATL 
//
//  Created by Jinlong He on 2019/11/1.
//  Copyright © 2019年 Ruting-Team. All rights reserved.
//

#ifndef atl_finite_automaton_merge_hpp 
#define atl_finite_automaton_merge_hpp 

namespace atl {
    template <class StateProp>
    struct intersect_merge {
        StateProp operator() (const StateProp& lhs, const StateProp& rhs) const {
            return (lhs & rhs);
        }

        template<class InputIterator>
        StateProp operator() (InputIterator first, InputIterator last) const {
            StateProp res = *first;
            while (++first != last) {
                res = res & (*first);
            }
            return res;
        }
    };

    template <class StateProp>
    struct union_merge {
        StateProp operator() (const StateProp& lhs, const StateProp& rhs) const {
            return (lhs | rhs);
        }

        template<class InputIterator>
        StateProp operator() (InputIterator first, InputIterator last) const {
            StateProp res = *first;
            while (++first != last) {
                res = res | (*first);
            }
            return res;
        }
    };
}

#endif /* atl_finite_automaton_merge_hpp */
