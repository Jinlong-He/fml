//
//  bool_variable.hpp
//  ll 
//
//  Created by Jinlong He on 2019/11/1.
//  Copyright © 2019年 Ruting-Team. All rights reserved.
//

#ifndef ll_detail_bool_variable_hpp 
#define ll_detail_bool_variable_hpp

#include "../variable.hpp"
#include "bool_item.hpp"
#include "bool_value.hpp"

namespace ll {
    namespace detail {
        class bool_variable_gen 
            : public variable_gen,
              public bool_item_gen {
        public:
            bool_variable_gen()
                : variable_gen(),
                  bool_item_gen() {
                      this -> set_type("boolean");
                  }

            bool_variable_gen(const string& id)
                : variable_gen(),
                  bool_item_gen(id) {
                      this -> set_type("boolean");
                  }

            bool_variable_gen(const bool_variable_gen& v)
                : variable_gen(v),
                  bool_item_gen(v) {}

            bool_variable_gen& operator=(const bool_variable_gen& v) {
                if (this != &v) {
                    variable_gen::operator=(v);
                    bool_item_gen::operator=(v);
                }
                return *this;
            }

        private:
        };
    }
}

#endif /* ll_detail_bool_variable_hpp */
