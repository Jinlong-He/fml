//
//  bool_value.hpp
//  ATL 
//
//  Created by Jinlong He on 2019/11/5.
//  Copyright © 2019年 Ruting-Team. All rights reserved.
//

#ifndef ll_detail_bool_value_hpp 
#define ll_detail_bool_value_hpp

#include "../value.hpp"
#include "bool_item.hpp"

namespace ll {
    namespace detail {
        class bool_value_gen 
            : public value_gen,
              public bool_item_gen {
        public:
            bool_value_gen()
                : value_gen(),
                  bool_item_gen() {}

            bool_value_gen(bool b)
                : value_gen(),
                  bool_item_gen(std::to_string(b), "boolean") {
                      if (b) set_identifier("TRUE");
                      if (!b) set_identifier("FALSE");
                  }

            //bool_value_gen(const string& id)
            //    : value_gen(),
            //      bool_item_gen(id, "boolean") {}

            bool_value_gen(const bool_value_gen& v)
                : value_gen(v),
                  bool_item_gen(v) {}

            bool_value_gen& operator=(const bool_value_gen& v) {
                if (this != &v) {
                    value_gen::operator=(v);
                    bool_item_gen::operator=(v);
                }
                return *this;
            }
        private:
        };
    }
}

#endif /* bool_value_hpp */
