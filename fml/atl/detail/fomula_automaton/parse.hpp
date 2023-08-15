//
//  parse.hpp
//  atl 
//
//  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
//  SPDX-License-Identifier: MIT
//  Copyright (c) 2020 Jinlong He.
//

#ifndef atl_detail_fomula_automaton_parse_hpp 
#define atl_detail_fomula_automaton_parse_hpp 

#include <iostream>
#include <fstream>
#include <boost/algorithm/string.hpp>
#include <fml/atl/detail/fomula_automaton/fomula_automaton.hpp>
using std::endl;

namespace atl::detail {
    struct parse_trace_nuxmv_impl {
        template <FOA_PARAMS>
        static void 
        apply(const FOA& foa,
              const string& trace_file,
              unordered_map<string, vector<string> >& trace_table) {
            std::ifstream in(trace_file);
            string line = "";
            vector<string> vars;
            while (getline(in, line)) {
                vars.clear();
                boost::split(vars, line, boost::is_any_of("	"));
                if (vars[0] == "Step") continue;
                auto& trace = trace_table[vars[0]];
                for (ID i = 1; i < vars.size(); i++) {
                    if (vars[i] != "-") trace.emplace_back(vars[i]);
                }
            }
            in.close();
        }
    };
};

namespace atl {
    template <FOA_PARAMS>
    inline void 
    parse_trace_nuxmv(const FOA& foa,
                      const string& trace_file,
                      unordered_map<string, vector<string> >& trace_table) {
        return detail::parse_trace_nuxmv_impl::apply(foa, trace_file, trace_table);
    }
}

#endif /* atl_detail_fomula_automaton_parse_hpp */
