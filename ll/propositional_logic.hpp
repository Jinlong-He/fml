//
//  propositional_logic.hpp
//  ll 
//
//  Created by Jinlong He on 2019/11/5.
//  Copyright © 2019年 Ruting-Team. All rights reserved.
//

#ifndef ll_propositional_logic_hpp 
#define ll_propositional_logic_hpp

#include "detail/propositional_logic/atomic_proposition.hpp"
#include "detail/propositional_logic/relation_proposition.hpp"
#include "item/item.hpp"

namespace ll {
    class atomic_proposition : public detail::atomic_proposition_gen {
    public:
        atomic_proposition()
            : atomic_proposition_gen() {}

        atomic_proposition(const string& id)
            : atomic_proposition_gen(id) {}

        atomic_proposition(const detail::item_gen& lhs, const detail::item_gen& rhs, 
                           const string& predicate) 
            : atomic_proposition_gen(lhs, rhs, predicate) {}

        atomic_proposition(const atomic_proposition& ap)
            : atomic_proposition_gen(ap) {}

        atomic_proposition& operator=(const atomic_proposition& ap) {
            if (this != &ap) {
                atomic_proposition_gen::operator=(ap);
            }
            return *this;
        }
    };

    class relation_proposition : public detail::relation_proposition_gen {
    public:
        relation_proposition()
            : relation_proposition_gen() {}

        relation_proposition(const detail::propositional_fomula_gen& f)
            : relation_proposition_gen(f) {}

        relation_proposition(const detail::propositional_fomula_gen& lhs, 
                             const detail::propositional_fomula_gen& rhs, 
                             const string& relation) 
            : relation_proposition_gen(lhs, rhs, relation) {}

        relation_proposition(const relation_proposition& ap)
            : relation_proposition_gen(ap) {}

        relation_proposition& operator=(const relation_proposition& ap) {
            if (this != &ap) {
                relation_proposition_gen::operator=(ap);
            }
            return *this;
        }
    };

    class propositional_fomula : public detail::propositional_fomula_gen {
    public:
        propositional_fomula()
            : propositional_fomula_gen() {}

        propositional_fomula(const string& id)
            : propositional_fomula_gen(id) {}

        propositional_fomula(const detail::propositional_fomula_gen& f)
            : propositional_fomula_gen(f) {}

        propositional_fomula(const propositional_fomula& ap)
            : propositional_fomula_gen(ap) {}

        propositional_fomula& operator=(const propositional_fomula& ap) {
            if (this != &ap) {
                propositional_fomula_gen::operator=(ap);
            }
            return *this;
        }
    };
}

#endif /* propositional_logic_value_hpp */
