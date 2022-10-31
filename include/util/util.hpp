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
#include <set>
#include <cstring>
#include <vector>
#include <boost/functional/hash.hpp>
using std::vector, std::string;
using std::cout;
using std::endl;

namespace std {
    template <class K, class C, class A>
    struct hash<unordered_set<K, C, A> > {
        size_t operator() (const unordered_set<K, C, A>& v) const {
            size_t res = 0;
            for (auto& i : v) {
                res ^= boost::hash_value(i);
            }
            return res;
        }
    };

    template <class K, class C, class A>
    struct hash<set<K, C, A> > {
        size_t operator() (const set<K, C, A>& v) const {
            size_t res = 0;
            for (auto& i : v) {
                res ^= boost::hash_value(i);
            }
            return res;
        }
    };

    template <class T>
    struct hash<vector<T> > {
        size_t operator() (const vector<T>& v) const {
            return boost::hash_range(v.begin(), v.end());
        }
    };

    template <class T1, class T2>
    struct hash<pair<T1, T2> > {
        size_t operator() (const pair<T1, T2>& p) const {
            return boost::hash_value(p);
        }
    };
}

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
    shuffle(const vector<T>& data1, const vector<T>& data2, 
            const vector<vector<T> >& datas, vector<vector<T> >& res) {
        if (data1.size() == 0 && data2.size() == 0) {
            res = datas;
            return;
        }
        vector<vector<T> > new_res1, new_res2;
        if (data1.size() > 0) {
            vector<vector<T> > new_datas;
            for (auto& data : datas) {
                vector<T> new_data = data;
                new_data.emplace_back(data1[0]);
                new_datas.emplace_back(new_data);
            }
            if (datas.size() == 0) {
                new_datas.emplace_back(vector<T>({data1[0]}));
            }
            vector<T> new_data1(data1.begin() + 1, data1.end());
            shuffle(new_data1, data2, new_datas, new_res1);
        }
        if (data2.size() > 0) {
            vector<vector<T> > new_datas;
            for (auto& data : datas) {
                vector<T> new_data = data;
                new_data.emplace_back(data2[0]);
                new_datas.emplace_back(new_data);
            }
            if (datas.size() == 0) {
                new_datas.emplace_back(vector<T>({data2[0]}));
            }
            vector<T> new_data2(data2.begin() + 1, data2.end());
            shuffle(data1, new_data2, new_datas, new_res2);
        }
        res = new_res1;
        res.insert(res.end(), new_res2.begin(), new_res2.end());
    }

    template <class T>
    static inline void
    shuffle(const vector<T>& data1, const vector<T>& data2, vector<vector<T> >& res) {
        shuffle(data1, data2, vector<vector<T> >(), res);
    }

    template<class T>
    static void combine(const vector<T>& datas, size_t n, size_t m, vector<size_t>& indexes, size_t M,  vector<vector<T> >& coms) {
        for (auto i = n; i >= m; i--) {
            indexes[m - 1] = i - 1;
            if (m > 1) {
                combine(datas, i - 1, m - 1, indexes, M, coms);
            } else {
                vector<T> com;
                for (int j = M - 1; j >= 0; j--) {
                    com.emplace_back(datas[indexes[j]]);
                }
                coms.emplace_back(com);
            }
        }
    }

    template<class T>
    static void combine(const vector<T>& datas, size_t m, vector<vector<T> >& coms) {
        vector<size_t> indexes(m);
        auto n = datas.size();
        combine(datas, n, m, indexes, m, coms);
    }

    template<class T>
    static void combine(const vector<T>& datas, size_t n, size_t m, vector<size_t>& indexes, size_t M,std:: unordered_set<std::set<T> >& coms) {
        for (auto i = n; i >= m; i--) {
            indexes[m - 1] = i - 1;
            if (m > 1) {
                combine(datas, i - 1, m - 1, indexes, M, coms);
            } else {
                std::set<T> com;
                for (int j = M - 1; j >= 0; j--) {
                    com.insert(datas[indexes[j]]);
                }
                coms.insert(com);
            }
        }
    }

    template<class T>
    static void combine(const vector<T>& datas, size_t m, std::unordered_set<std::set<T> >& coms) {
        vector<size_t> indexes(m);
        auto n = datas.size();
        combine(datas, n, m, indexes, m, coms);
    }

    template<class T>
    static void permut(vector<T>& datas, vector<vector<T> >& pers) {
        sort(datas.begin(), datas.end());
        do {
            pers.push_back(datas);
        }
        while(next_permutation(datas.begin(), datas.end()));
    }

    template<class T>
    static void product(const vector<vector<T> > &datas, vector<vector<T> > &res, int layer, vector<T> &tmp) {
        if (layer < datas.size() - 1){
            for (int i = 0; i < datas[layer].size(); i++){
                vector<double> sb;
                sb.clear();
                for (int i = 0; i < tmp.size(); i++){
                        sb.emplace_back(tmp[i]);
                }
                sb.emplace_back(datas[layer][i]);
                productImplement(datas, res, layer + 1, sb);
            }
        }
        else if (layer == datas.size() - 1){
            for (int j = 0; j < datas[layer].size(); j++){
                tmp.emplace_back(datas[layer][j]);
                res.emplace_back(tmp);
                tmp.pop_back();
            }
        }
    }
    template<class T>
    static void product(vector<vector<T> > datas, vector<vector<T> > &res) {
        vector<T> tmp;
        product(datas, res, 0, tmp);
    }

}

#endif /* util_hpp */


