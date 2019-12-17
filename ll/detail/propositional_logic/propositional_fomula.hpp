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
    namespace detail {
        class propositional_fomula_gen : public fomula_gen {
        public:
            propositional_fomula_gen()
                : fomula_gen() {}

            propositional_fomula_gen(const string& id)
                : fomula_gen(id) {}

            propositional_fomula_gen(const propositional_fomula_gen& p) 
                : fomula_gen(p) {}

            propositional_fomula_gen& operator=(const propositional_fomula_gen& p) {
                if (this != &p) {
                    fomula_gen::operator=(p);
                }
                return *this;
            }
        private:
        };
    }
}

#endif /* propositional_fomular_hpp */
