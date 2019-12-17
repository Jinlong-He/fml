//
//  enum_value.hpp
//  ATL 
//
//  Created by Jinlong He on 2019/11/5.
//  Copyright © 2019年 Ruting-Team. All rights reserved.
//

#ifndef ll_detail_enum_value_hpp 
#define ll_detail_enum_value_hpp

#include "../value.hpp"
#include "enum_item.hpp"

namespace ll {
    namespace detail {
        class enum_value_gen 
            : public value_gen,
              public enum_item_gen {
        public:
            enum_value_gen()
                : value_gen(),
                  enum_item_gen() {}

            enum_value_gen(const string& id)
                : value_gen(),
                  enum_item_gen(id, "enum") {}

            enum_value_gen(const enum_value_gen& v)
                : value_gen(v),
                  enum_item_gen(v) {}

            enum_value_gen& operator=(const enum_value_gen& v) {
                if (this != &v) {
                    value_gen::operator=(v);
                    enum_item_gen::operator=(v);
                }
                return *this;
            }
        private:
        };
    }
}

#endif /* enum_value_hpp */
