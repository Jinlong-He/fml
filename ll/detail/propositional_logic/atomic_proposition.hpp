//
//  atomic_proposition.hpp
//  ll 
//
//  Created by Jinlong He on 2019/11/1.
//  Copyright © 2019年 Ruting-Team. All rights reserved.
//

#ifndef ll_detail_atomic_proposition_hpp 
#define ll_detail_atomic_proposition_hpp

#include "propositional_fomula.hpp"
#include "../item.hpp"

namespace ll {
    namespace detail {
        class atomic_proposition_gen : public propositional_fomula_gen {
            typedef std::unique_ptr<item_gen> ItemPtr;
        public:
            atomic_proposition_gen()
                : propositional_fomula_gen(),
                  lhs_(nullptr),
                  rhs_(nullptr),
                  predicate_("=") {}

            atomic_proposition_gen(const string& id)
                : propositional_fomula_gen(id),
                  lhs_(nullptr),
                  rhs_(nullptr),
                  predicate_("") {}

            atomic_proposition_gen(const item_gen& lhs, const item_gen& rhs, 
                                   const string& predicate) 
                : propositional_fomula_gen("(" + lhs.to_string() + predicate + 
                                                 rhs.to_string() + ")"),
                  lhs_(new item_gen(lhs)),
                  rhs_(new item_gen(rhs)),
                  predicate_(predicate) {}

            atomic_proposition_gen(const atomic_proposition_gen& ap)
                : propositional_fomula_gen(ap),
                  lhs_(new item_gen(ap.lhs())),
                  rhs_(new item_gen(ap.rhs())),
                  predicate_(ap.predicate_) {}

            atomic_proposition_gen& operator=(const atomic_proposition_gen& ap) {
                if (this != &ap) {
                    propositional_fomula_gen::operator=(ap);
                    lhs_ = ItemPtr(new item_gen(ap.lhs()));
                    lhs_ = ItemPtr(new item_gen(ap.rhs()));
                    predicate_ = ap.predicate_;
                }
                return *this;
            }

            const string& predicate() const {
                return predicate_;
            }

            const item_gen& lhs() const {
                return *lhs_;
            }

            const item_gen& rhs() const {
                return *rhs_;
            }
        private:
            ItemPtr lhs_;
            ItemPtr rhs_;
            string predicate_;
        };
        
        inline atomic_proposition_gen operator==(const item_gen& lhs, const item_gen& rhs) {
            return atomic_proposition_gen(lhs, rhs, "=");
        }

        inline atomic_proposition_gen operator!=(const item_gen& lhs, const item_gen& rhs) {
            return atomic_proposition_gen(lhs, rhs, "!=");
        }

        inline atomic_proposition_gen operator>(const item_gen& lhs, const item_gen& rhs) {
            return atomic_proposition_gen(lhs, rhs, ">");
        }

        inline atomic_proposition_gen operator>=(const item_gen& lhs, const item_gen& rhs) {
            return atomic_proposition_gen(lhs, rhs, ">=");
        }

        inline atomic_proposition_gen operator<(const item_gen& lhs, const item_gen& rhs) {
            return atomic_proposition_gen(lhs, rhs, "<");
        }

        inline atomic_proposition_gen operator<=(const item_gen& lhs, const item_gen& rhs) {
            return atomic_proposition_gen(lhs, rhs, "<=");
        }
    }
}

#endif /* ll_detail_atomic_proposition_hpp */
