//
//  propositional_fomular.hpp
//  ll 
//
//  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
//  SPDX-License-Identifier: MIT
//  Copyright (c) 2020 Jinlong He.
//

#ifndef propositional_fomular_hpp 
#define propositional_fomular_hpp

#include <ll/atomic_proposition.hpp>

namespace ll {
    class propositional_fomula : public atomic_proposition {
        typedef std::unique_ptr<propositional_fomula> PropositionalFomularPtr;
    public:
        struct Builder {
        public:
            Builder(const string& content)
                : lhs_(nullptr),
                  rhs_(nullptr),
                  logical_operator_(""),
                  content_(content) {}

            Builder(const atomic_proposition& p,
                    const string& logical_operator)
                : lhs_(&p),
                  rhs_(nullptr),
                  logical_operator_(logical_operator),
                  content_("(" + logical_operator + p.to_string() + ")") {}

            Builder(const atomic_proposition& lhs,
                    const atomic_proposition& rhs,
                    const string& logical_operator)
                : lhs_(&lhs),
                  rhs_(&rhs),
                  logical_operator_(logical_operator),
                  content_("(" + lhs.to_string() + logical_operator + rhs.to_string() + ")") {}

            const atomic_proposition& lhs() const {
                return *lhs_;
            }

            const atomic_proposition& rhs() const {
                return *rhs_;
            }

            const string& logical_operator() const {
                return logical_operator_;
            }

            bool hasLhs() const {
                return lhs_;
            }

            bool hasRhs() const {
                return rhs_;
            }

            const string& content() const {
                return content_;
            }
        private:
            const atomic_proposition* lhs_;
            const atomic_proposition* rhs_;
            string logical_operator_;
            string content_;
        };

        propositional_fomula()
            : atomic_proposition(),
              rhs_(nullptr),
              logical_operator_("") {}

        propositional_fomula(const string& content)
            : atomic_proposition(content),
              rhs_(nullptr),
              logical_operator_("") {}

        propositional_fomula(const item& lhs, const item& rhs, 
                             const string& predicate) 
            : atomic_proposition(lhs, rhs, predicate),
              rhs_(nullptr),
              logical_operator_("") {}

        propositional_fomula(const Builder& builder)
            : atomic_proposition(builder.content()),
              lhs_(builder.hasLhs() ? new propositional_fomula(builder.lhs()) : nullptr),
              rhs_(builder.hasRhs() ? new propositional_fomula(builder.rhs()) : nullptr),
              logical_operator_(builder.logical_operator()) {}

        propositional_fomula(const atomic_proposition& ap)
            : atomic_proposition(ap),
              rhs_(nullptr),
              logical_operator_("") {}

        propositional_fomula(const atomic_proposition& p,
                             const string& logical_operator)
            : atomic_proposition("(" + logical_operator + p.to_string() + ")"),
              lhs_(new propositional_fomula(p)),
              rhs_(nullptr),
              logical_operator_(logical_operator) {}

        propositional_fomula(const atomic_proposition& lhs,
                             const atomic_proposition& rhs,
                             const string& logical_operator)
            : atomic_proposition("(" + lhs.to_string() + logical_operator + rhs.to_string() + ")"),
              lhs_(new propositional_fomula(lhs)),
              rhs_(new propositional_fomula(rhs)),
              logical_operator_(logical_operator) {}

        propositional_fomula(const propositional_fomula& p) 
            : atomic_proposition(p),
              lhs_(p.rhs_ ? new propositional_fomula(p.lhs()) : nullptr),
              rhs_(p.rhs_ ? new propositional_fomula(p.rhs()) : nullptr),
              logical_operator_(p.logical_operator_) {}

        propositional_fomula& operator=(const propositional_fomula& p) {
            if (this != &p) {
                atomic_proposition::operator=(p);
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
