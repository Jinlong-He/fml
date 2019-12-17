//
//  int_value.hpp
//  ATL 
//
//  Created by Jinlong He on 2019/11/5.
//  Copyright © 2019年 Ruting-Team. All rights reserved.
//

#ifndef ll_detail_int_value_hpp 
#define ll_detail_int_value_hpp

#include "../value.hpp"
#include "int_item.hpp"

namespace ll {
    namespace detail {
        class int_value_gen 
            : public value_gen,
              public int_item_gen {
        public:
            int_value_gen()
                : value_gen(),
                  int_item_gen() {}

            int_value_gen(int v)
                : value_gen(),
                  int_item_gen(std::to_string(v), "integer") {}

            int_value_gen(const int_value_gen& v)
                : value_gen(v),
                  int_item_gen(v) {}

            int_value_gen& operator=(const int_value_gen& v) {
                if (this != &v) {
                    value_gen::operator=(v);
                    int_item_gen::operator=(v);
                }
                return *this;
            }
        private:
        };
    }
}

#endif /* int_value_hpp */
