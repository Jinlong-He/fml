//
//  int_item.hpp
//  ll 
//
//  Created by Jinlong He on 2019/12/6.
//  Copyright © 2019年 Ruting-Team. All rights reserved.
//

#ifndef ll_detail_int_item_hpp 
#define ll_detail_int_item_hpp

#include "../item.hpp"

namespace ll {
    namespace detail {
        class int_expr_gen;
        class int_item_gen : public item_gen {
        public:
            int_item_gen()
                : item_gen() {}

            int_item_gen(const string& identifier)
                : item_gen(identifier) {}

            int_item_gen(const string& identifier, const string& type)
                : item_gen(identifier, type) {}

            int_item_gen(const int_item_gen& i)
                : item_gen(i) {}

            int_item_gen& operator=(const int_item_gen& i) {
                if (this != &i) {
                    item_gen::operator=(i);
                }
                return *this;
            }
        };
    }
}

#endif /* ll_detail_int_item_hpp */
