//
//  automaton_utility.hpp
//  atl 
//
//  Created by Jinlong He on 2019/10/27.
//  Copyright © 2019年 Ruting-Team. All rights reserved.
//

#ifndef atl_automaton_utility_hpp 
#define atl_automaton_utility_hpp

#include <iostream>
#include <fstream>
#include <atl/detail/automaton.hpp>
#include <atl/detail/no_type.hpp>
#include <atl/finite_automaton/nondeterministic_finite_automaton.hpp>
#include <atl/push_down_system/push_down_system.hpp>
using std::cout;
using std::endl;
using std::fstream;
namespace atl {
    template <typename Automaton>
    inline void
    print_state(const Automaton& a,
                typename Automaton::State state,
                std::ostream& os) {
        os << state;
        if constexpr (!std::is_same<typename Automaton::state_property_type, boost::no_property>::value) {
            os << " {" << atl::get_property(a, state) << "}";
        }
    }
    template <typename Automaton>
    inline void
    print_state(const Automaton& a,
                typename Automaton::State state) {
        return print_state(a, state, cout);
    }
    
    template <typename Automaton>
    inline void
    print_automaton(const Automaton& a, std::ostream& os) {
        typename Automaton::StateIter s_it, s_end;
        typename Automaton::OutTransitionIter t_it, t_end;
        for (tie(s_it, s_end) = states(a); s_it != s_end; s_it++) {
            tie(t_it, t_end) = out_transitions(a, *s_it);
            if (t_it == t_end) continue;
            os << "State: ";
            print_state(a, *s_it, os);
            os << endl;
            for ( ; t_it != t_end; t_it++) {
                os << atl::get_property(a, *t_it) << " ";
                print_state(a, atl::target(a, *t_it), os);
                os << endl;
            }
        }
    }

    template <typename Automaton>
    inline void
    print_automaton(const Automaton& a) {
        return print_automaton(a, cout);
    }

    template <FA_PARAMS>
    inline void
    print_fa(const FA& fa, std::ostream& os) {
        os << "FA: ";
        if constexpr (!std::is_same<typename FA::automaton_property_type, boost::no_property>::value) {
            os << atl::get_property(fa);
        }
        os << endl;
        os << "States: " << state_set(fa).size() << endl;
        os << "Alphabet: ";
        ID i = 0;
        for (auto& c : alphabet(fa)) {
            if (++i == alphabet(fa).size()) {
                os << c;
            } else {
                os << c << " ";
            }
        }
        os << endl;
        os << "Initial: " << initial_state(fa) << endl;
        os << "Final: ";
        i = 0;
        for (auto s : final_state_set(fa)) {
            if (++i == final_state_set(fa).size()) {
                os << s;
            } else {
                os << s << " ";
            }
        }
        os << endl;
        os << "--BODY--" <<endl;;
        print_automaton(fa, os);
        os << "--END--" << endl;
    }

    template <FA_PARAMS>
    inline void
    print_fa(const FA& fa) {
        return print_fa(fa, cout);
    }

    template <PDS_PARAMS>
    inline void
    print_pds(const PDS& pds, const string& name = "") {
        cout << "PDS: " << name << endl;
        cout << "States: " << state_set(pds).size() << endl;
        cout << "Alphabet: ";
        ID i = 0;
        for (auto& c : alphabet(pds)) {
            if (++i == alphabet(pds).size()) {
                cout << c;
            } else {
                cout << c << " ";
            }
        }
        cout << endl;
        cout << "--BODY--" <<endl;;
        print_automaton(pds);
        cout << "--END--" << endl;
    }

    template <FA_PARAMS>
    inline void
    read_fa(FA& fa, const string& file_name = "") {
        fstream fs;
        fs.open (file_name, fstream::in);
        if (fs.is_open()) {
            string str;
            int source_num = -1;
            while(getline(fs, str)) {
                if (str.find("FA:") != string::npos || 
                    str.find("--BODY--") != string::npos ||
                    str.find("--END--") != string::npos) {
                    continue;
                }
                else if (str.find("States:") != string::npos) {
                    int states_num = stoi(str.substr(8, str.length() - 8));
                    for (int i = 0; i < states_num; i++)
                        add_state(fa);
                } else if (str.find("Alphabet:") != string::npos) {
                    typename FA::SymbolSet alphabet;
                    for (string& c : util::split(str.substr(10, str.length() - 10), " ")) {
                        alphabet.insert(c[0]);
                    }
                    set_alphabet(fa, alphabet);
                } else if (str.find("Initial:") != string::npos) {
                    int initial_num = stoi(str.substr(9, str.length() - 9));
                    set_initial_state(fa, initial_num);
                } else if (str.find("Final:") != string::npos) {
                    for (string& num : util::split(str.substr(7, str.length() - 7), " ")) {
                        set_final_state(fa, stoi(num));
                    }
                } else if (str.find("State:") != string::npos) {
                    source_num = stoi(str.substr(7, str.length() - 7));
                } else {
                    auto res = util::split(str, " ");
                    add_transition(fa, source_num, stoi(res[1]), res[0][0]);
                }
            }
            fs.close();
        } else {
            cout << "error: file not found!" << endl;
            return;
        }
    }

    template <PDS_PARAMS>
    inline void
    read_pds(PDS& pds, const string& file_name = "") {
        fstream fs;
        fs.open (file_name, fstream::in);
        if (fs.is_open()) {
            string str;
            int source_num = -1;
            while(getline(fs, str)) {
                if (str.find("PDS:") != string::npos || 
                    str.find("--BODY--") != string::npos ||
                    str.find("--END--") != string::npos ||
                    str.find("#") == 0) {
                    continue;
                }
                else if (str.find("States:") != string::npos) {
                    int states_num = stoi(str.substr(8, str.length() - 8));
                    for (int i = 0; i < states_num; i++)
                        add_state(pds);
                } else if (str.find("Alphabet:") != string::npos) {
                    typename PDS::SymbolSet alphabet;
                    for (string& c : util::split(str.substr(10, str.length() - 10), " ")) {
                        alphabet.insert(c[0]);
                    }
                    set_alphabet(pds, alphabet);
                } else if (str.find("State:") != string::npos) {
                    source_num = stoi(str.substr(7, str.length() - 7));
                } else {
                    int pos = str.find(")");
                    vector<char> stack;
                    for (auto c : util::split(str.substr(3, pos - 3), " ")) {
                        stack.push_back(c[0]);
                    }
                    add_transition(pds, source_num, stoi(str.substr(pos + 2, str.length() - pos + 1)), str[0], stack);
                }
            }
            fs.close();
        } else {
            cout << "error: file not found!" << endl;
            return;
        }
    }
}

#endif /* atl_automaton_utility_hpp */
