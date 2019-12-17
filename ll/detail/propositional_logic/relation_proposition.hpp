//
//  relation_proposition.hpp
//  ll 
//
//  Created by Jinlong He on 2019/11/1.
//  Copyright © 2019年 Ruting-Team. All rights reserved.
//

#ifndef ll_detail_relation_proposition_hpp 
#define ll_detail_relation_proposition_hpp

#include "propositional_fomula.hpp"
#include "../item.hpp"

namespace ll {
    namespace detail {
        class relation_proposition_gen : public propositional_fomula_gen {
            typedef std::unique_ptr<propositional_fomula_gen> PropositionalFomularPtr;
        public:
            relation_proposition_gen()
                : propositional_fomula_gen(),
                  lhs_(nullptr),
                  rhs_(nullptr),
                  relation_("&") {}

            relation_proposition_gen(const propositional_fomula_gen& f)
                : propositional_fomula_gen("!(" + f.to_string() + ")"),
                  lhs_(new propositional_fomula_gen(f)),
                  rhs_(nullptr),
                  relation_("!") {}

            relation_proposition_gen(const propositional_fomula_gen& lhs, 
                                     const propositional_fomula_gen& rhs, 
                                     const string& relation) 
                : propositional_fomula_gen("(" + lhs.to_string() + relation + 
                                                 rhs.to_string() + ")"),
                  lhs_(new propositional_fomula_gen(lhs)),
                  rhs_(new propositional_fomula_gen(rhs)),
                  relation_(relation) {}

            relation_proposition_gen(const relation_proposition_gen& ap)
                : propositional_fomula_gen(ap),
                  lhs_(new propositional_fomula_gen(ap.lhs())),
                  rhs_(new propositional_fomula_gen(ap.rhs())),
                  relation_(ap.relation_) {}

            relation_proposition_gen& operator=(const relation_proposition_gen& ap) {
                if (this != &ap) {
                    propositional_fomula_gen::operator=(ap);
                    lhs_ = PropositionalFomularPtr(new propositional_fomula_gen(ap.lhs()));
                    rhs_ = PropositionalFomularPtr(new propositional_fomula_gen(ap.rhs()));
                    relation_ = ap.relation_;
                }
                return *this;
            }

            const string& relation() const {
                return relation_;
            }

            const propositional_fomula_gen& lhs() const {
                return *lhs_;
            }

            const propositional_fomula_gen& rhs() const {
                return *rhs_;
            }
        private:
            PropositionalFomularPtr lhs_;
            PropositionalFomularPtr rhs_;
            string relation_;
        };
        
        inline relation_proposition_gen operator&(const propositional_fomula_gen& lhs, 
                                                  const propositional_fomula_gen& rhs) {
            return relation_proposition_gen(lhs, rhs, "&");
        }

        inline relation_proposition_gen operator|(const propositional_fomula_gen& lhs, 
                                                  const propositional_fomula_gen& rhs) {
            return relation_proposition_gen(lhs, rhs, "|");
        }

        inline relation_proposition_gen operator!(const propositional_fomula_gen& f) {
            return relation_proposition_gen(f);
        }
    }
}

#endif /* ll_detail_relation_proposition_hpp */
