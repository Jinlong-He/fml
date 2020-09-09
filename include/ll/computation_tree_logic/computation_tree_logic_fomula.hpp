//
//  computation_tree_logic_fomular.hpp
//  ll 
//
//  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
//  SPDX-License-Identifier: MIT
//  Copyright (c) 2020 Jinlong He.
//

#ifndef computation_tree_logic_fomular_hpp 
#define computation_tree_logic_fomular_hpp

#include <ll/propositional_logic/propositional_fomula.hpp>

namespace ll {
    class computation_tree_logic_fomula : public propositional_fomula {
        typedef std::unique_ptr<computation_tree_logic_fomula> CTLFomulaPtr;
    public:
        computation_tree_logic_fomula()
            : propositional_fomula(),
              lhs_(nullptr),
              rhs_(nullptr),
              quantifier_(""),
              temporal_operator_("") {}

        computation_tree_logic_fomula(const string& content)
            : propositional_fomula(content),
              lhs_(nullptr),
              rhs_(nullptr),
              quantifier_(""),
              temporal_operator_("") {}

        computation_tree_logic_fomula(const propositional_fomula& p)
            : propositional_fomula(p),
              lhs_(nullptr),
              rhs_(nullptr),
              quantifier_(""),
              temporal_operator_("") {}

        computation_tree_logic_fomula(const atomic_proposition& p)
            : propositional_fomula(p),
              lhs_(nullptr),
              rhs_(nullptr),
              quantifier_(""),
              temporal_operator_("") {}


        computation_tree_logic_fomula(const atomic_proposition& p,
                                      const string& logical_operator)
            : propositional_fomula(p, logical_operator),
              lhs_(nullptr),
              rhs_(nullptr),
              quantifier_(""),
              temporal_operator_("") {}

        computation_tree_logic_fomula(const atomic_proposition& lhs,
                                      const atomic_proposition& rhs,
                                      const string& logical_operator)
            : propositional_fomula(lhs, rhs, logical_operator),
              lhs_(nullptr),
              rhs_(nullptr),
              quantifier_(""),
              temporal_operator_("") {}

        computation_tree_logic_fomula(const atomic_proposition& p,
                                      const string& quantifier,
                                      const string& temporal_operator)
            : propositional_fomula(quantifier + temporal_operator + "[" + p.to_string() + "]"),
              lhs_(new computation_tree_logic_fomula(p)),
              rhs_(nullptr),
              quantifier_(quantifier),
              temporal_operator_(temporal_operator) {}

        computation_tree_logic_fomula(const atomic_proposition& lhs,
                                      const atomic_proposition& rhs,
                                      const string& quantifier,
                                      const string& temporal_operator)
            : propositional_fomula(quantifier + "[" + lhs.to_string() + temporal_operator + 
                                                      rhs.to_string() + "]"),
              lhs_(new computation_tree_logic_fomula(lhs)),
              rhs_(new computation_tree_logic_fomula(rhs)),
              quantifier_(quantifier),
              temporal_operator_(temporal_operator) {}

        computation_tree_logic_fomula(const computation_tree_logic_fomula& p) 
            : propositional_fomula(p),
              lhs_((p.rhs_ ? new computation_tree_logic_fomula(p.lhs()): nullptr)),
              rhs_((p.rhs_ ? new computation_tree_logic_fomula(p.rhs()): nullptr)),
              quantifier_(p.quantifier_),
              temporal_operator_(p.temporal_operator_) {}

        computation_tree_logic_fomula& operator=(const computation_tree_logic_fomula& p) {
            if (this != &p) {
                propositional_fomula::operator=(p);
                lhs_ = ((p.rhs_ ? CTLFomulaPtr(new computation_tree_logic_fomula(p.lhs())): nullptr));
                rhs_ = ((p.rhs_ ? CTLFomulaPtr(new computation_tree_logic_fomula(p.rhs())): nullptr));
                quantifier_ = p.quantifier_;
                temporal_operator_ = p.temporal_operator_;
            }
            return *this;
        }

        const computation_tree_logic_fomula& lhs() const {
            return *lhs_;
        }

        const computation_tree_logic_fomula& rhs() const {
            return *rhs_;
        }

        const string& quantifier() const {
            return quantifier_;
        }

        const string& temporal_operator() const {
            return temporal_operator_;
        }

    private:
        CTLFomulaPtr lhs_;
        CTLFomulaPtr rhs_;
        string quantifier_;
        string temporal_operator_;
    };

    inline computation_tree_logic_fomula make_AX(const computation_tree_logic_fomula& p) {
        return computation_tree_logic_fomula(p, "A", "X");
    }

    inline computation_tree_logic_fomula make_EX(const computation_tree_logic_fomula& p) {
        return computation_tree_logic_fomula(p, "E", "X");
    }

    inline computation_tree_logic_fomula make_AF(const computation_tree_logic_fomula& p) {
        return computation_tree_logic_fomula(p, "A", "F");
    }

    inline computation_tree_logic_fomula make_EF(const computation_tree_logic_fomula& p) {
        return computation_tree_logic_fomula(p, "E", "F");
    }

    inline computation_tree_logic_fomula make_AG(const computation_tree_logic_fomula& p) {
        return computation_tree_logic_fomula(p, "A", "G");
    }

    inline computation_tree_logic_fomula make_EG(const computation_tree_logic_fomula& p) {
        return computation_tree_logic_fomula(p, "E", "G");
    }

    inline computation_tree_logic_fomula make_AU(const computation_tree_logic_fomula& lhs,
                                                 const computation_tree_logic_fomula& rhs) {
        return computation_tree_logic_fomula(lhs, rhs, "A", "U");
    }

    inline computation_tree_logic_fomula make_EU(const computation_tree_logic_fomula& lhs,
                                                 const computation_tree_logic_fomula& rhs) {
        return computation_tree_logic_fomula(lhs, rhs, "E", "U");
    }

    inline computation_tree_logic_fomula operator&(const computation_tree_logic_fomula& lhs, 
                                                   const computation_tree_logic_fomula& rhs) {
        return computation_tree_logic_fomula(lhs, rhs, "&");
    }

    inline computation_tree_logic_fomula make_and(const computation_tree_logic_fomula& lhs,
                                                  const computation_tree_logic_fomula& rhs) {
        return computation_tree_logic_fomula(lhs, rhs, "&");
    }

    inline computation_tree_logic_fomula operator|(const computation_tree_logic_fomula& lhs, 
                                                   const computation_tree_logic_fomula& rhs) {
        return computation_tree_logic_fomula(lhs, rhs, "|");
    }

    inline computation_tree_logic_fomula make_or(const computation_tree_logic_fomula& lhs,
                                                 const computation_tree_logic_fomula& rhs) {
        return computation_tree_logic_fomula(lhs, rhs, "|");
    }

    inline computation_tree_logic_fomula operator!(const computation_tree_logic_fomula& f) {
        return computation_tree_logic_fomula(f, "!");
    }

    inline computation_tree_logic_fomula make_not(const computation_tree_logic_fomula& f) {
        return computation_tree_logic_fomula(f, "!");
    }

    inline computation_tree_logic_fomula make_implication(const computation_tree_logic_fomula& lhs,
                                                          const computation_tree_logic_fomula& rhs) {
        return computation_tree_logic_fomula(lhs, rhs, "->");
    }

    inline computation_tree_logic_fomula make_equivalence(const computation_tree_logic_fomula& lhs,
                                                          const computation_tree_logic_fomula& rhs) {
        return computation_tree_logic_fomula(lhs, rhs, "<->");
    }
}

#endif /* computation_tree_logic_fomular_hpp */
