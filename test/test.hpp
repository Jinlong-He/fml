//
//  test.hpp
//  fml 
//
//  Created by Jinlong He on 2020/07/29.
//  Copyright © 2019年 Ruting-Team. All rights reserved.
//

#ifndef test_hpp 
#define test_hpp 
#include <string>
#include <boost/unordered_map.hpp>
using std::string;

namespace test {
    struct Symbol {
    public:
        Symbol() : id_("") {}
        Symbol(const string& id) : id_(id) {}
        Symbol(char id) : id_(1, id) {
            if (id == 0) id_ = "";
        }
        const string& id() const {
            return id_;
        }
        bool operator== (const Symbol& x) const {
            return id_ == x.id_;
        }

        bool operator!= (const Symbol& x) const {
            return id_ != x.id_;
        }

        bool operator< (const Symbol& x) const {
            return id_ < x.id_;
        }

        Symbol operator& (const Symbol& x) const {
            if (x == Symbol(0)) return *this;
            if (*this == Symbol(0)) return x;
            return Symbol(id_ + "&" + x.id_);
        }

        Symbol operator| (const Symbol& x) const {
            if (x == Symbol(0)) return *this;
            if (*this == Symbol(0)) return x;
            return Symbol(id_ + "|" + x.id_);
        }

        friend std::ostream& operator<< (std::ostream& os, const Symbol& x) {
            os << x.id_;
            return os;
        }
    private:
        string id_;
    };
}

namespace boost {
    template<>
    struct hash<test::Symbol> {
        std::size_t operator() (const test::Symbol& c) const{
            return boost::hash<string>()(c.id());
        }
    };
}

namespace test {
    //test finite automaton

    bool test_minimize1();
    bool test_minimize2();
    bool test_minimize3();
    bool test_minimize4();
    bool test_minimize5();
    bool test_minimize6();
    bool test_minimize7();
    bool test_minimize8();

    bool test_basic_regular_expression1();
    bool test_basic_regular_expression2();
    bool test_basic_regular_expression3();
    bool test_basic_regular_expression4();
    bool test_basic_regular_expression5();

    bool test_intersect1();
    bool test_intersect2();
    bool test_intersect3();
    bool test_intersect4();
    bool test_intersect5();

    bool test_union1();
    bool test_union2();
    bool test_union3();
    bool test_union4();
    bool test_union5();

    bool test_concat1();
    bool test_concat2();
    bool test_concat3();
    bool test_concat4();

    bool test_composite1();

    bool test_poststar();
    bool test_prestar();

    bool test_fomula_automaton();

    bool test_propositional_logic();
    bool test_ctl();
    bool test_ltl();

    void test_equal();

    void test_basic_regular_expression();
    void test_transducer();
}

#endif /* test_hpp */
