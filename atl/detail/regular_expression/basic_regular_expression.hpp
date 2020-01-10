//
//  basic_regular_expression.hpp
//  atl 
//
//  Created by Jinlong He on 2020/01/03.
//  Copyright © 2019年 Ruting-Team. All rights reserved.
//

#ifndef atl_detail_basic_regular_expression_hpp 
#define atl_detail_basic_regular_expression_hpp

#include <vector>
#include <stack>
#include <boost/unordered_map.hpp>
using boost::unordered_map;

namespace atl {
    namespace detail {
        template <typename Symbol>
        struct RegularSymbol {
            Symbol symbol;
            char opt;
            char priority;

            RegularSymbol() 
                : symbol(),
                  opt(0),
                  priority(0) {}

            RegularSymbol(const Symbol& c)
                : symbol(c),
                  opt(0),
                  priority(0) {}

            RegularSymbol(const Symbol& c, char o)
                : symbol(c),
                  opt(o) {
                      if (o == '|') {
                          priority = 1;
                      } else if (o == '.') {
                          priority = 2;
                      } else {
                          priority = 3;
                      }
                  }
        };

        template <typename Symbol>
        class basic_regular_expression_gen {
            typedef std::vector<RegularSymbol<Symbol> > RegularSymbols;
        public:
            basic_regular_expression_gen() {}

            basic_regular_expression_gen(const std::basic_string<Symbol>& re) {
                init_operator_map();
                init(re);
            }

            basic_regular_expression_gen(const std::basic_string<Symbol>& re, 
                                         const unordered_map<Symbol, char>& operator_map) 
                : operator_map_(operator_map) {
                init(re);
            }

            const RegularSymbols& original_expression() const {
                return original_expression_;
            }

            const RegularSymbols& postfix_expression() const {
                return postfix_expression_;
            }

        private:
            void init_operator_map() {
                operator_map_[Symbol('*')] = '*';
                operator_map_[Symbol('+')] = '+';
                operator_map_[Symbol('?')] = '?';
                operator_map_[Symbol('|')] = '|';
                operator_map_[Symbol('.')] = '.';
                operator_map_[Symbol('(')] = '(';
                operator_map_[Symbol(')')] = ')';
                operator_map_[Symbol('\\')] = '\\';
            }

            void init(const std::basic_string<Symbol>& re) {
                bool flag = false;
                for (size_t i = 0; i < re.size(); i++) {
                    if (operator_map_.count(re[i]) > 0) {
                        if (flag & ((operator_map_[re[i]] == '(') | 
                                    (operator_map_[re[i]] == '\\' ))) {
                            original_expression_.push_back(RegularSymbol<Symbol>(Symbol('.'),
                                                           operator_map_[Symbol('.')]));
                        }
                        flag = true;
                        if ((operator_map_[re[i]] == '|') | 
                            (operator_map_[re[i]] == '(')) {
                            flag = false;
                        }
                        if (operator_map_[re[i]] == '\\') {
                            original_expression_.push_back(RegularSymbol<Symbol>(re[++i]));
                        } else {
                            original_expression_.push_back(RegularSymbol<Symbol>(Symbol(re[i]),
                                                           operator_map_[Symbol(re[i])]));
                        }
                    } else {
                        if (flag) {
                            original_expression_.push_back(RegularSymbol<Symbol>(Symbol('.'),
                                                           operator_map_[Symbol('.')]));
                        }
                        original_expression_.push_back(RegularSymbol<Symbol>(re[i]));
                        flag = true;
                    }
                }
                make_postfix_expression();
            }

            void make_postfix_expression() {
                std::stack<RegularSymbol<Symbol> > operator_stack;
                for (size_t i = 0; i < original_expression_.size(); i++) {
                    if (original_expression_[i].opt == 0) {
                        postfix_expression_.push_back(original_expression_[i]);
                    } else {
                        if (original_expression_[i].opt == '(') {
                            operator_stack.push(original_expression_[i]);
                        } else if (original_expression_[i].opt == ')') {
                            while ((!operator_stack.empty()) & 
                                   (operator_stack.top().opt != '(')) {
                                postfix_expression_.push_back(operator_stack.top());
                                operator_stack.pop();
                            }
                            operator_stack.pop();
                        } else {
                            while ((!operator_stack.empty()) &
                                   ((operator_stack.top().opt != '(') & 
                                   (original_expression_[i].priority <= 
                                   operator_stack.top().priority))) {
                                postfix_expression_.push_back(operator_stack.top());
                                operator_stack.pop();
                            }
                            operator_stack.push(original_expression_[i]);
                        }
                    }
                }
                while (!operator_stack.empty()) {
                    postfix_expression_.push_back(operator_stack.top());
                    operator_stack.pop();
                }
            }

        private:
            unordered_map<Symbol, char> operator_map_;
            RegularSymbols original_expression_;
            RegularSymbols postfix_expression_;
        };
    }
}

#endif /* atl_detail_basic_regular_expression_hpp */
