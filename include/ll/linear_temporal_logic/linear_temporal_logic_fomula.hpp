//
//  linear_temporal_logic_fomular.hpp
//  ll 
//
//  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
//  SPDX-License-Identifier: MIT
//  Copyright (c) 2020 Jinlong He.
//

#ifndef linear_temporal_logic_fomular_hpp 
#define linear_temporal_logic_fomular_hpp

#include <ll/propositional_logic/propositional_fomula.hpp>

namespace ll {
    class linear_temporal_logic_fomula : public propositional_fomula {
        typedef std::unique_ptr<linear_temporal_logic_fomula> LTLFomulaPtr;
    public:
        linear_temporal_logic_fomula()
            : propositional_fomula(),
              lhs_(nullptr),
              rhs_(nullptr),
              temporal_operator_("") {}

        linear_temporal_logic_fomula(const string& content)
            : propositional_fomula(content),
              lhs_(nullptr),
              rhs_(nullptr),
              temporal_operator_("") {}

        linear_temporal_logic_fomula(const atomic_proposition& ap)
            : propositional_fomula(ap),
              lhs_(nullptr),
              rhs_(nullptr),
              temporal_operator_("") {}

        linear_temporal_logic_fomula(const propositional_fomula& p)
            : propositional_fomula(p),
              lhs_(nullptr),
              rhs_(nullptr),
              temporal_operator_("") {}

        linear_temporal_logic_fomula(const atomic_proposition& p,
                                     const string& op,
                                     bool is_logical = true)
            : propositional_fomula(is_logical ? propositional_fomula::Builder(p, op) : 
                                                propositional_fomula::Builder(
                                                    op + "[" + p.to_string() + "]")),
              lhs_(is_logical ? nullptr : new linear_temporal_logic_fomula(p)),
              rhs_(nullptr),
              temporal_operator_(op) {}

        linear_temporal_logic_fomula(const atomic_proposition& lhs,
                                     const atomic_proposition& rhs,
                                     const string& op,
                                     bool is_logical = true)
            : propositional_fomula(is_logical ? propositional_fomula::Builder(lhs, rhs, op) :
                                                propositional_fomula::Builder(
                                                    "[" + lhs.to_string() + op + rhs.to_string() + "]")),
              lhs_(is_logical ? nullptr : new linear_temporal_logic_fomula(lhs)),
              rhs_(is_logical ? nullptr : new linear_temporal_logic_fomula(rhs)),
              temporal_operator_(op) {}

        linear_temporal_logic_fomula(const linear_temporal_logic_fomula& p) 
            : propositional_fomula(p),
              lhs_((p.rhs_ ? new linear_temporal_logic_fomula(p.lhs()): nullptr)),
              rhs_((p.rhs_ ? new linear_temporal_logic_fomula(p.rhs()): nullptr)),
              temporal_operator_(p.temporal_operator_) {}

        linear_temporal_logic_fomula& operator=(const linear_temporal_logic_fomula& p) {
            if (this != &p) {
                propositional_fomula::operator=(p);
                lhs_ = ((p.rhs_ ? LTLFomulaPtr(new linear_temporal_logic_fomula(p.lhs())): nullptr));
                rhs_ = ((p.rhs_ ? LTLFomulaPtr(new linear_temporal_logic_fomula(p.rhs())): nullptr));
                temporal_operator_ = p.temporal_operator_;
            }
            return *this;
        }

        const linear_temporal_logic_fomula& lhs() const {
            return *lhs_;
        }

        const linear_temporal_logic_fomula& rhs() const {
            return *rhs_;
        }

        const string& temporal_operator() const {
            return temporal_operator_;
        }

    private:
        LTLFomulaPtr lhs_;
        LTLFomulaPtr rhs_;
        string temporal_operator_;
    };

    inline linear_temporal_logic_fomula make_G(const linear_temporal_logic_fomula& p) {
        return linear_temporal_logic_fomula(p, "G", 0);
    }

    inline linear_temporal_logic_fomula make_F(const linear_temporal_logic_fomula& p) {
        return linear_temporal_logic_fomula(p, "F", 0);
    }

    inline linear_temporal_logic_fomula make_X(const linear_temporal_logic_fomula& p) {
        return linear_temporal_logic_fomula(p, "X", 0);
    }

    inline linear_temporal_logic_fomula make_U(const linear_temporal_logic_fomula& lhs,
                                               const linear_temporal_logic_fomula& rhs) {
        return linear_temporal_logic_fomula(lhs, rhs, "U", 0);
    }

    inline linear_temporal_logic_fomula make_W(const linear_temporal_logic_fomula& lhs,
                                               const linear_temporal_logic_fomula& rhs) {
        return linear_temporal_logic_fomula(lhs, rhs, "W", 0);
    }

    inline linear_temporal_logic_fomula make_R(const linear_temporal_logic_fomula& lhs,
                                               const linear_temporal_logic_fomula& rhs) {
        return linear_temporal_logic_fomula(lhs, rhs, "R", 0);
    }

    inline linear_temporal_logic_fomula operator&(const linear_temporal_logic_fomula& lhs, 
                                                  const linear_temporal_logic_fomula& rhs) {
        return linear_temporal_logic_fomula(lhs, rhs, "&");
    }

    inline linear_temporal_logic_fomula make_and(const linear_temporal_logic_fomula& lhs,
                                                 const linear_temporal_logic_fomula& rhs) {
        return linear_temporal_logic_fomula(lhs, rhs, "&");
    }

    inline linear_temporal_logic_fomula operator|(const linear_temporal_logic_fomula& lhs, 
                                                  const linear_temporal_logic_fomula& rhs) {
        return linear_temporal_logic_fomula(lhs, rhs, "|");
    }

    inline linear_temporal_logic_fomula make_or(const linear_temporal_logic_fomula& lhs,
                                                const linear_temporal_logic_fomula& rhs) {
        return linear_temporal_logic_fomula(lhs, rhs, "|");
    }

    inline linear_temporal_logic_fomula operator!(const linear_temporal_logic_fomula& f) {
        return linear_temporal_logic_fomula(f, "!");
    }

    inline linear_temporal_logic_fomula make_not(const linear_temporal_logic_fomula& f) {
        return linear_temporal_logic_fomula(f, "!");
    }

    inline linear_temporal_logic_fomula make_implies(const linear_temporal_logic_fomula& lhs,
                                                     const linear_temporal_logic_fomula& rhs) {
        return linear_temporal_logic_fomula(lhs, rhs, "->");
    }
}

#endif /* linear_temporal_logic_fomular_hpp */
