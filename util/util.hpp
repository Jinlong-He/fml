//
//  util.hpp
//  util 
//
//  Created by Jinlong He on 2019/9/12.
//  Copyright © 2019年 Ruting-Team. All rights reserved.
//

#ifndef util_hpp 
#define util_hpp
#include <iostream> 
#include <unordered_set>

namespace util {
    template <class Set>
    static inline void set_union(const Set& lhs, const Set& rhs, Set& set) {
        set.insert(lhs.begin(), lhs.end());                            
        set.insert(rhs.begin(), rhs.end());                            
    }

    template <class InputIterator, class Set>
    static inline void set_union(InputIterator first, InputIterator last, Set& set) {
        do {
            set.insert((*first).begin(), (*first).end());
        } while (++first != last);
    }

    template <class Set, class Key = typename Set::value_type>
    static inline void set_intersection(const Set& lhs, const Set& rhs, Set& set) {
        if (lhs.size() <= rhs.size()) {                                 
            for (const Key& key : lhs) {
                if (rhs.count(key) > 0) {
                    set.insert(key);
                }
            }
        } else {
            set_intersection(rhs, lhs, set);
        }
    }

    template <class InputIterator, class Set, class Key = typename Set::value_type>
    static inline void set_intersection(InputIterator first, InputIterator last, Set& set) {
        if ((last - first) == 1) {
            set.insert((*first).begin(), (*first).end());
            return;
        }
        if ((last - first) == 2) {
            set_intersection(*first, *(++first), set);
            return;
        }
        Set lhs, rhs;
        InputIterator mid = (last - first)/2 + first;
        set_intersection(first, mid, lhs);
        set_intersection(mid, last, rhs);
        set_intersection(lhs, rhs, set);
    }
}

#endif /* util_hpp */
