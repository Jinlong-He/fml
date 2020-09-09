//
//  merge.hpp
//  atl 
//
//  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
//  SPDX-License-Identifier: MIT
//  Copyright (c) 2020 Jinlong He.
//

#ifndef atl_detail_finite_automaton_merge_hpp 
#define atl_detail_finite_automaton_merge_hpp 

namespace atl::detail {
    template <class Prop>
    struct intersect_merge_gen {
        Prop operator() (const Prop& lhs, const Prop& rhs) const {
            return (lhs & rhs);
        }

        template<class InputIterator>
        Prop operator() (InputIterator first, InputIterator last) const {
            Prop res = *first;
            while (++first != last) {
                res = res & (*first);
            }
            return res;
        }
    };

    template <class Prop>
    struct union_merge_gen {
        Prop operator() (const Prop& lhs, const Prop& rhs) const {
            return (lhs | rhs);
        }

        template<class InputIterator>
        Prop operator() (InputIterator first, InputIterator last) const {
            Prop res = *first;
            while (++first != last) {
                res = res | (*first);
            }
            return res;
        }
    };

    struct no_type_merge_gen {
    };
};

namespace atl {
    template <class Prop>
    inline detail::intersect_merge_gen<Prop>
    intersect_merge() {
        return detail::intersect_merge_gen<Prop>();
    }

    template <class Prop>
    inline detail::union_merge_gen<Prop>
    union_merge() {
        return detail::union_merge_gen<Prop>();
    }

    inline detail::no_type_merge_gen
    no_type_merge() {
        return detail::no_type_merge_gen();
    }
};

#endif /* atl_detail_finite_automaton_merge_hpp */
