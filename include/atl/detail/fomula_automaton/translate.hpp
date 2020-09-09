//
//  translate.hpp
//  atl 
//
//  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
//  SPDX-License-Identifier: MIT
//  Copyright (c) 2020 Jinlong He.
//

#ifndef atl_detail_fomula_automaton_translate_hpp 
#define atl_detail_fomula_automaton_translate_hpp 

#include <iostream>
#include <atl/detail/fomula_automaton/fomula_automaton.hpp>
using std::endl;

namespace atl::detail {
    struct translate_nuxmv_impl {
        template <FOA_PARAMS>
        static void 
        apply(const FOA& foa,
              std::ostream& os) {
            os << "IVAR" << endl;
            for (auto state : input_state_set(foa)) {
                const auto& v = atl::get_property(foa, state);
                os << v.identifier() + " : " + v.type() + ";" << endl;
            }
            os << "VAR" << endl;
            for (auto state : control_state_set(foa)) {
                const auto& v = atl::get_property(foa, state);
                os << v.identifier() + " : " + v.type() + ";" << endl;
            }
            for (const auto& ap : init_list(foa)) {
                os << "INIT" << endl;
                os << ap.to_string() << endl;
            }
            os << "ASSIGN" << endl;
            for (auto source : control_state_set(foa)) {
                const auto& source_var = atl::get_property(foa, source);
                os << "next(" + source_var.identifier() + ") := case" << endl;
                typename FOA::OutTransitionIter it, end;
                for (tie(it, end) = out_transitions(foa, source); it != end; it++) {
                    auto target = atl::target(foa, *it);
                    const auto& target_item = atl::get_property(foa, target);
                    os << atl::get_property(foa, *it).to_string() + " : " + 
                          target_item.identifier() + ";" << endl;
                }
                os << "esac;" << endl;
            }
        }
    };
};

namespace atl {
    template <FOA_PARAMS>
    inline void 
    translate_nuxmv(const FOA& foa,
           std::ostream& os) {
        return detail::translate_nuxmv_impl::apply(foa, os);
    }
}

#endif /* atl_detail_fomula_automaton_translate_hpp */
