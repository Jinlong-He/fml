//
//  atomic_proposition.hpp
//  ll 
//
//  Created by Jinlong He on 2019/11/1.
//  Copyright © 2019年 Ruting-Team. All rights reserved.
//

#ifndef ll_atomic_proposition_hpp 
#define ll_atomic_proposition_hpp

#include "propositional_fomula.hpp"
#include "../item/item.hpp"

namespace ll {
    class atomic_proposition : public propositional_fomula {
        typedef std::unique_ptr<item> ItemPtr;
    public:
        atomic_proposition()
            : propositional_fomula(),
              lhs_(nullptr),
              rhs_(nullptr),
              predicate_("=") {}

        atomic_proposition(const string& id)
            : propositional_fomula(id),
              lhs_(nullptr),
              rhs_(nullptr),
              predicate_("") {}

        atomic_proposition(const item& lhs, const item& rhs, 
                               const string& predicate) 
            : propositional_fomula("(" + lhs.to_string() + predicate + 
                                             rhs.to_string() + ")"),
              lhs_(new item(lhs)),
              rhs_(new item(rhs)),
              predicate_(predicate) {}

        atomic_proposition(const atomic_proposition& ap)
            : propositional_fomula(ap),
              lhs_(new item(ap.lhs())),
              rhs_(new item(ap.rhs())),
              predicate_(ap.predicate_) {}

        atomic_proposition& operator=(const atomic_proposition& ap) {
            if (this != &ap) {
                propositional_fomula::operator=(ap);
                lhs_ = ItemPtr(new item(ap.lhs()));
                lhs_ = ItemPtr(new item(ap.rhs()));
                predicate_ = ap.predicate_;
            }
            return *this;
        }

        const string& predicate() const {
            return predicate_;
        }

        const item& lhs() const {
            return *lhs_;
        }

        const item& rhs() const {
            return *rhs_;
        }
    private:
        ItemPtr lhs_;
        ItemPtr rhs_;
        string predicate_;
    };
    
    inline atomic_proposition operator==(const item& lhs, const item& rhs) {
        return atomic_proposition(lhs, rhs, "=");
    }

    inline atomic_proposition operator!=(const item& lhs, const item& rhs) {
        return atomic_proposition(lhs, rhs, "!=");
    }

    inline atomic_proposition operator>(const item& lhs, const item& rhs) {
        return atomic_proposition(lhs, rhs, ">");
    }

    inline atomic_proposition operator>=(const item& lhs, const item& rhs) {
        return atomic_proposition(lhs, rhs, ">=");
    }

    inline atomic_proposition operator<(const item& lhs, const item& rhs) {
        return atomic_proposition(lhs, rhs, "<");
    }

    inline atomic_proposition operator<=(const item& lhs, const item& rhs) {
        return atomic_proposition(lhs, rhs, "<=");
    }
}

#endif /* ll_atomic_proposition_hpp */
