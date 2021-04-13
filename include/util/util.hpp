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
using std::vector, std::string;
using std::cout;
using std::endl;

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

    template <class SetIn, class SetOut, class Pair = typename SetOut::value_type>
    static inline void set_product(const SetIn& set_in, SetOut& set_out) {
        for (const auto& i : set_in) {
            for (const auto& j : set_in) {
                set_out.insert(Pair(i,j));
            }
        }
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
            res.emplace_back(s);
            p = strtok(NULL, d);  
        }  
        return res;  
    }

    template <class T>
    static inline void
    shuffle(const vector<T>& datas1, const vector<T>& datas2, 
            const vector<vector<T> >& datas, vector<vector<T> >& res) {
        if (datas1.size() == 0 && datas2.size() == 0) {
            res = datas;
            return;
        }
        vector<vector<T> > new_res1, new_res2;
        if (datas1.size() > 0) {
            vector<vector<T> > new_datas;
            for (auto& data : datas) {
                vector<T> new_data = data;
                new_data.emplace_back(datas1[0]);
                new_datas.emplace_back(new_data);
            }
            if (datas.size() == 0) {
                new_datas.emplace_back(vector<T>({datas1[0]}));
            }
            vector<T> new_datas1(datas1.begin() + 1, datas1.end());
            shuffle(new_datas1, datas2, new_datas, new_res1);
        }
        if (datas2.size() > 0) {
            vector<vector<T> > new_datas;
            for (auto& data : datas) {
                vector<T> new_data = data;
                new_data.emplace_back(datas2[0]);
                new_datas.emplace_back(new_data);
            }
            if (datas.size() == 0) {
                new_datas.emplace_back(vector<T>({datas2[0]}));
            }
            vector<T> new_datas2(datas2.begin() + 1, datas2.end());
            shuffle(datas1, new_datas2, new_datas, new_res2);
        }
        res = new_res1;
        res.insert(res.end(), new_res2.begin(), new_res2.end());
    }

    template <class T>
    static inline void
    shuffle(const vector<T>& datas1, const vector<T>& datas2, vector<vector<T> >& res) {
        shuffle(datas1, datas2, vector<vector<T> >(), res);
    }
}

#endif /* util_hpp */
