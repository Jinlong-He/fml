//
//  util.hpp
//  util 
//
//  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
//  SPDX-License-Identifier: MIT
//  Copyright (c) 2020 Jinlong He.
//

#ifndef util_hpp 
#define util_hpp
#include <iostream> 
#include <unordered_set>
#include <vector>
using std::vector;
using std::string;

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

    static inline vector<string> 
    split(const string& str, const string& delim) {  
        vector<string> res;  
        if(str == "") return res;  
        char * strs = new char[str.length() + 1];
        strcpy(strs, str.c_str());   
        char * d = new char[delim.length() + 1];  
        strcpy(d, delim.c_str());  
        char *p = strtok(strs, d);  
        while(p) {  
            string s = p;
            res.push_back(s);
            p = strtok(NULL, d);  
        }  
        return res;  
    }
}

#endif /* util_hpp */
