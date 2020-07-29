//
//  relation_proposition.hpp
//  ll 
//
//  Created by Jinlong He on 2019/11/1.
//  Copyright © 2019年 Ruting-Team. All rights reserved.
//

#ifndef ll_relation_proposition_hpp 
#define ll_relation_proposition_hpp

#include "propositional_fomula.hpp"
#include "../item/item.hpp"

namespace ll {
    class relation_proposition : public propositional_fomula {
        typedef std::unique_ptr<propositional_fomula> PropositionalFomularPtr;
    public:
        relation_proposition()
            : propositional_fomula(),
              lhs_(nullptr),
              rhs_(nullptr),
              relation_("&") {}

        relation_proposition(const propositional_fomula& f)
            : propositional_fomula("!(" + f.to_string() + ")"),
              lhs_(new propositional_fomula(f)),
              rhs_(nullptr),
              relation_("!") {}

        relation_proposition(const propositional_fomula& lhs, 
                             const propositional_fomula& rhs, 
                             const string& relation) 
            : propositional_fomula("(" + lhs.to_string() + relation + 
                                             rhs.to_string() + ")"),
              lhs_(new propositional_fomula(lhs)),
              rhs_(new propositional_fomula(rhs)),
              relation_(relation) {}

        relation_proposition(const relation_proposition& ap)
            : propositional_fomula(ap),
              lhs_(new propositional_fomula(ap.lhs())),
              rhs_(new propositional_fomula(ap.rhs())),
              relation_(ap.relation_) {}

        relation_proposition& operator=(const relation_proposition& ap) {
            if (this != &ap) {
                propositional_fomula::operator=(ap);
                lhs_ = PropositionalFomularPtr(new propositional_fomula(ap.lhs()));
                rhs_ = PropositionalFomularPtr(new propositional_fomula(ap.rhs()));
                relation_ = ap.relation_;
            }
            return *this;
        }

        const string& relation() const {
            return relation_;
        }

        const propositional_fomula& lhs() const {
            return *lhs_;
        }

        const propositional_fomula& rhs() const {
            return *rhs_;
        }
    private:
        PropositionalFomularPtr lhs_;
        PropositionalFomularPtr rhs_;
        string relation_;
    };
    
    inline relation_proposition operator&(const propositional_fomula& lhs, 
                                          const propositional_fomula& rhs) {
        return relation_proposition(lhs, rhs, "&");
    }

    inline relation_proposition operator|(const propositional_fomula& lhs, 
                                          const propositional_fomula& rhs) {
        return relation_proposition(lhs, rhs, "|");
    }

    inline relation_proposition operator!(const propositional_fomula& f) {
        return relation_proposition(f);
    }
}

#endif /* ll_relation_proposition_hpp */
