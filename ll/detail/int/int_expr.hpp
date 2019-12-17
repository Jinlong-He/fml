//
//  int_expr.hpp
//  ll 
//
//  Created by Jinlong He on 2019/11/1.
//  Copyright © 2019年 Ruting-Team. All rights reserved.
//

#ifndef ll_detail_int_expr_hpp 
#define ll_detail_int_expr_hpp

#include "int_variable.hpp"

namespace ll {
    namespace detail {
        class int_expr_gen 
            : public int_item_gen {
            typedef std::unique_ptr<int_item_gen> IntItemPtr;
        public:
            int_expr_gen()
                : int_item_gen(),
                  lhs_(nullptr),
                  rhs_(nullptr) {}

            int_expr_gen(const string& id)
                : int_item_gen(id),
                  lhs_(nullptr),
                  rhs_(nullptr) {}

            int_expr_gen(const int_item_gen& lhs, const int_item_gen& rhs, const string& op)
                : int_item_gen("(" + lhs.to_string() + op + rhs.to_string() + ")"),
                  lhs_(new int_item_gen(lhs)),
                  rhs_(new int_item_gen(rhs)),
                  operator_(op) {}

            int_expr_gen(const int_expr_gen& v)
                : int_item_gen(v),
                  lhs_(new int_item_gen(v.lhs())),
                  rhs_(new int_item_gen(v.rhs())),
                  operator_(v.operator_) {}

            int_expr_gen& operator=(const int_expr_gen& v) {
                if (this != &v) {
                    int_item_gen::operator=(v);
                    lhs_ = IntItemPtr(new int_item_gen(v.lhs()));
                    rhs_ = IntItemPtr(new int_item_gen(v.rhs()));
                    operator_ = v.operator_;
                }
                return *this;
            }

            const int_item_gen& lhs() const {
                return *lhs_;
            }

            const int_item_gen& rhs() const {
                return *rhs_;
            }
        private:
            IntItemPtr lhs_;
            IntItemPtr rhs_;
            string operator_;
        };

        inline int_item_gen operator+(const int_item_gen& lhs, const int_item_gen& rhs) {
            return int_expr_gen(lhs, rhs, "+");
        }

        inline int_item_gen operator+(const int_item_gen& lhs, int rhs) {
            return int_expr_gen(lhs, int_value_gen(rhs), "+");
        }

        inline int_item_gen operator+(int lhs, const int_item_gen& rhs) {
            return int_expr_gen(int_value_gen(lhs), rhs, "+");
        }

        inline int_item_gen operator-(const int_item_gen& lhs, const int_item_gen& rhs) {
            return int_expr_gen(lhs, rhs, "-");
        }

        inline int_item_gen operator-(const int_item_gen& lhs, int rhs) {
            return int_expr_gen(lhs, int_value_gen(rhs), "-");
        }

        inline int_item_gen operator-(int lhs, const int_item_gen& rhs) {
            return int_expr_gen(int_value_gen(lhs), rhs, "-");
        }

        inline int_item_gen operator*(const int_item_gen& lhs, const int_item_gen& rhs) {
            return int_expr_gen(lhs, rhs, "*");
        }

        inline int_item_gen operator*(const int_item_gen& lhs, int rhs) {
            return int_expr_gen(lhs, int_value_gen(rhs), "*");
        }

        inline int_item_gen operator*(int lhs, const int_item_gen& rhs) {
            return int_expr_gen(int_value_gen(lhs), rhs, "*");
        }

        inline int_item_gen operator/(const int_item_gen& lhs, const int_item_gen& rhs) {
            return int_expr_gen(lhs, rhs, "/");
        }

        inline int_item_gen operator/(const int_item_gen& lhs, int rhs) {
            return int_expr_gen(lhs, int_value_gen(rhs), "/");
        }

        inline int_item_gen operator/(int lhs, const int_item_gen& rhs) {
            return int_expr_gen(int_value_gen(lhs), rhs, "/");
        }
    }
}

#endif /* ll_detail_int_expr_hpp */
