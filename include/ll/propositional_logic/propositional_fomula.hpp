//
//  propositional_fomular.hpp
//  ATL 
//
//  Created by Jinlong He on 2019/11/5.
//  Copyright © 2019年 Ruting-Team. All rights reserved.
//

#ifndef propositional_fomular_hpp 
#define propositional_fomular_hpp

#include "../fomula.hpp"

namespace ll {
    class propositional_fomula : public fomula {
    public:
        propositional_fomula()
            : fomula() {}

        propositional_fomula(const string& id)
            : fomula(id) {}

        propositional_fomula(const propositional_fomula& p) 
            : fomula(p) {}

        propositional_fomula& operator=(const propositional_fomula& p) {
            if (this != &p) {
                fomula::operator=(p);
            }
            return *this;
        }
    private:
    };
}

#endif /* propositional_fomular_hpp */
