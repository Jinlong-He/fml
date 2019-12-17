//
//  fomula.hpp
//  ll 
//
//  Created by Jinlong He on 2019/12/6.
//  Copyright © 2019年 Ruting-Team. All rights reserved.
//

#ifndef ll_detail_fomula_hpp 
#define ll_detail_fomula_hpp

#include <iostream>
#include <list>
using std::string;
namespace ll {
    namespace detail {
        class fomula_gen {
        public:
            fomula_gen()
                : identifier_("") {}

            fomula_gen(const string& identifier)
                : identifier_(identifier) {}

            fomula_gen(const fomula_gen& i)
                : identifier_(i.identifier_) {}

            fomula_gen& operator=(const fomula_gen& i) {
                if (this != &i) {
                    identifier_ = i.identifier_;
                }
                return *this;
            }

            const string& to_string() const {
                return identifier_;
            }
        private:
            string identifier_;
        };
    }
}

#endif /* ll_detail_fomula_gen_hpp */
