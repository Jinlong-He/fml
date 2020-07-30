//
//  propositional_fomular.hpp
//  ATL 
//
//  Created by Jinlong He on 2019/11/5.
//  Copyright © 2019年 Ruting-Team. All rights reserved.
//

#ifndef propositional_fomular_hpp 
#define propositional_fomular_hpp

#include "../computation_tree_logic/computation_tree_logic_fomula.hpp"
#include "../linear_temporal_logic/linear_temporal_logic_fomula.hpp"

namespace ll {
    class propositional_fomula : public computation_tree_logic_fomula,
                                 public linear_temporal_logic_fomula {
        typedef std::unique_ptr<propositional_fomula> PropositionalFomularPtr;
    public:
        propositional_fomula()
            : fomula(),
              computation_tree_logic_fomula(),
              linear_temporal_logic_fomula(),
              rhs_(nullptr),
              logical_operator_("") {}

        propositional_fomula(const string& content)
            : fomula(content),
              computation_tree_logic_fomula(content),
              linear_temporal_logic_fomula(content),
              rhs_(nullptr),
              logical_operator_("") {}

        propositional_fomula(const propositional_fomula& p,
                             const string& logical_operator)
            : fomula("(" + logical_operator + p.to_string() + ")"),
              computation_tree_logic_fomula(),
              linear_temporal_logic_fomula(),
              lhs_(new propositional_fomula(p)),
              rhs_(nullptr),
              logical_operator_(logical_operator) {}

        propositional_fomula(const propositional_fomula& lhs,
                             const propositional_fomula& rhs,
                             const string& logical_operator)
            : fomula("(" + lhs.to_string() + logical_operator + rhs.to_string() + ")"),
              computation_tree_logic_fomula(),
              linear_temporal_logic_fomula(),
              lhs_(new propositional_fomula(lhs)),
              rhs_(new propositional_fomula(rhs)),
              logical_operator_(logical_operator) {}

        propositional_fomula(const propositional_fomula& p) 
            : fomula(p),
              computation_tree_logic_fomula(p),
              linear_temporal_logic_fomula(p),
              lhs_(p.rhs_ ? new propositional_fomula(p.lhs()) : nullptr),
              rhs_(p.rhs_ ? new propositional_fomula(p.rhs()) : nullptr),
              logical_operator_(p.logical_operator_) {}

        propositional_fomula& operator=(const propositional_fomula& p) {
            if (this != &p) {
                fomula::operator=(p);
                computation_tree_logic_fomula::operator=(p);
                linear_temporal_logic_fomula::operator=(p);
                lhs_ = (p.rhs_ ? PropositionalFomularPtr(new propositional_fomula(p.lhs())) : 
                                 nullptr);
                rhs_ = (p.rhs_ ? PropositionalFomularPtr(new propositional_fomula(p.rhs())) : 
                                 nullptr);
                logical_operator_ = p.logical_operator_;
            }
            return *this;
        }

        const propositional_fomula& lhs() const {
            return *lhs_;
        }

        const propositional_fomula& rhs() const {
            return *rhs_;
        }

        const string& logical_operator() const {
            return logical_operator_;
        }

    private:
        PropositionalFomularPtr lhs_;
        PropositionalFomularPtr rhs_;
        string logical_operator_;
    };

    inline propositional_fomula operator&(const propositional_fomula& lhs, 
                                          const propositional_fomula& rhs) {
        return propositional_fomula(lhs, rhs, "&");
    }

    inline propositional_fomula make_and(const propositional_fomula& lhs,
                                         const propositional_fomula& rhs) {
        return propositional_fomula(lhs, rhs, "&");
    }

    inline propositional_fomula operator|(const propositional_fomula& lhs, 
                                          const propositional_fomula& rhs) {
        return propositional_fomula(lhs, rhs, "|");
    }

    inline propositional_fomula make_or(const propositional_fomula& lhs,
                                        const propositional_fomula& rhs) {
        return propositional_fomula(lhs, rhs, "|");
    }

    inline propositional_fomula operator!(const propositional_fomula& f) {
        return propositional_fomula(f, "!");
    }

    inline propositional_fomula make_not(const propositional_fomula& f) {
        return propositional_fomula(f, "!");
    }

    inline propositional_fomula make_implication(const propositional_fomula& lhs,
                                             const propositional_fomula& rhs) {
        return propositional_fomula(lhs, rhs, "->");
    }

    inline propositional_fomula make_equivalence(const propositional_fomula& lhs,
                                             const propositional_fomula& rhs) {
        return propositional_fomula(lhs, rhs, "<->");
    }
}

#endif /* propositional_fomular_hpp */
