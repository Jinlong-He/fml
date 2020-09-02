//
//  int_expr.hpp
//  ll 
//
//  Created by Jinlong He on 2019/11/1.
//  Copyright © 2019年 Ruting-Team. All rights reserved.
//

#ifndef ll_int_expr_hpp 
#define ll_int_expr_hpp

#include <ll/item/int/int_variable.hpp>

namespace ll {
    class int_expr 
        : public int_item {
        typedef std::unique_ptr<int_item> IntItemPtr;
    public:
        int_expr()
            : int_item(),
              lhs_(nullptr),
              rhs_(nullptr) {}

        int_expr(const string& id)
            : item(id),
              int_item(),
              lhs_(nullptr),
              rhs_(nullptr) {}

        int_expr(const int_item& lhs, const int_item& rhs, const string& op)
            : item("(" + lhs.identifier() + op + rhs.identifier() + ")"),
              int_item(),
              lhs_(new int_item(lhs)),
              rhs_(new int_item(rhs)),
              operator_(op) {}

        int_expr(const int_expr& v)
            : item(v),
              int_item(),
              lhs_(new int_item(v.lhs())),
              rhs_(new int_item(v.rhs())),
              operator_(v.operator_) {}

        int_expr& operator=(const int_expr& v) {
            if (this != &v) {
                item::operator=(v);
                int_item::operator=(v);
                lhs_ = IntItemPtr(new int_item(v.lhs()));
                rhs_ = IntItemPtr(new int_item(v.rhs()));
                operator_ = v.operator_;
            }
            return *this;
        }

        ~int_expr() {}

        const int_item& lhs() const {
            return *lhs_;
        }

        const int_item& rhs() const {
            return *rhs_;
        }
    private:
        IntItemPtr lhs_;
        IntItemPtr rhs_;
        string operator_;
    };

    inline int_item operator+(const int_item& lhs, const int_item& rhs) {
        return int_expr(lhs, rhs, "+");
    }

    inline int_item operator+(const int_item& lhs, int rhs) {
        return int_expr(lhs, int_value(rhs), "+");
    }

    inline int_item operator+(int lhs, const int_item& rhs) {
        return int_expr(int_value(lhs), rhs, "+");
    }

    inline int_item operator-(const int_item& lhs, const int_item& rhs) {
        return int_expr(lhs, rhs, "-");
    }

    inline int_item operator-(const int_item& lhs, int rhs) {
        return int_expr(lhs, int_value(rhs), "-");
    }

    inline int_item operator-(int lhs, const int_item& rhs) {
        return int_expr(int_value(lhs), rhs, "-");
    }

    inline int_item operator*(const int_item& lhs, const int_item& rhs) {
        return int_expr(lhs, rhs, "*");
    }

    inline int_item operator*(const int_item& lhs, int rhs) {
        return int_expr(lhs, int_value(rhs), "*");
    }

    inline int_item operator*(int lhs, const int_item& rhs) {
        return int_expr(int_value(lhs), rhs, "*");
    }

    inline int_item operator/(const int_item& lhs, const int_item& rhs) {
        return int_expr(lhs, rhs, "/");
    }

    inline int_item operator/(const int_item& lhs, int rhs) {
        return int_expr(lhs, int_value(rhs), "/");
    }

    inline int_item operator/(int lhs, const int_item& rhs) {
        return int_expr(int_value(lhs), rhs, "/");
    }
}

#endif /* ll_int_expr_hpp */
