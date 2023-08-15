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

#include <fml/atl/detail/automaton.hpp>
namespace atl::detail {
    template <class Prop>
    struct intersect_merge_gen {
        Prop operator() (const Prop& lhs, const Prop& rhs) const {
            if constexpr (std::is_same<Prop, boost::no_property>::value) {
                return boost::no_property();
            } else {
                return (lhs & rhs);
            }
        }

        template<class InputIterator>
        Prop operator() (InputIterator first, InputIterator last) const {
            if constexpr (std::is_same<Prop, boost::no_property>::value) {
                return boost::no_property();
            } else {
                Prop res = *first;
                while (++first != last) {
                    res = res & (*first);
                }
                return res;
            }
        }
    };

    template <class Prop>
    struct union_merge_gen {
        Prop operator() (const Prop& lhs, const Prop& rhs) const {
            if constexpr (std::is_same<Prop, boost::no_property>::value) {
                return boost::no_property();
            } else {
                return (lhs | rhs);
            }
        }

        template<class InputIterator>
        Prop operator() (InputIterator first, InputIterator last) const {
            if constexpr (std::is_same<Prop, boost::no_property>::value) {
                return boost::no_property();
            } else {
                Prop res = *first;
                while (++first != last) {
                    res = res | (*first);
                }
                return res;
            }
        }
    };

    template <class LhsProp, class RhsProp, class ResProp>
    struct merge_gen {
        ResProp operator() (const LhsProp& lhs, const RhsProp& rhs) const {
            if constexpr (std::is_same<LhsProp, boost::no_property>::value &&
                          std::is_same<RhsProp, boost::no_property>::value) {
                return boost::no_property();
            } else {
                return (lhs & rhs);
            }
        }
    };

    template <class LhsProp, class RhsProp>
    struct equal_gen {
        bool operator() (const LhsProp& lhs, const RhsProp& rhs) const {
            if constexpr (std::is_same<LhsProp, boost::no_property>::value &&
                          std::is_same<RhsProp, boost::no_property>::value) {
                return true;
            } else {
                return (lhs == rhs);
            }
        }
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

    template <class LhsProp, class RhsProp = LhsProp>
    inline detail::equal_gen<LhsProp, RhsProp>
    equal() {
        return detail::equal_gen<LhsProp, RhsProp>();
    }

    template <class LhsProp, class RhsProp, class ResProp>
    inline detail::merge_gen<LhsProp, RhsProp, ResProp>
    merge() {
        return detail::merge_gen<LhsProp, RhsProp, ResProp>();
    }
};

#endif /* atl_detail_finite_automaton_merge_hpp */
