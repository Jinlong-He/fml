//
//  enum_variable.hpp
//  ll 
//
//  Created by Jinlong He on 2019/11/1.
//  Copyright © 2019年 Ruting-Team. All rights reserved.
//

#ifndef ll_detail_enum_variable_hpp 
#define ll_detail_enum_variable_hpp

#include "../variable.hpp"
#include "enum_item.hpp"
#include "enum_value.hpp"

namespace ll {
    namespace detail {
        class enum_variable_gen 
            : public variable_gen,
              public enum_item_gen {
            typedef std::unique_ptr<enum_value_gen> EnumValuePtr;
            typedef std::list<EnumValuePtr> enum_values;
        public:
            enum_variable_gen()
                : variable_gen(),
                  enum_item_gen() {
                      this -> set_type("enum");
                  }

            enum_variable_gen(const string& id)
                : variable_gen(),
                  enum_item_gen(id) {
                      this -> set_type("enum");
                  }

            enum_variable_gen(const string& id, std::initializer_list<enum_value_gen> list)
                : variable_gen(),
                  enum_item_gen(id) {
                      string res = "{";
                      for (auto& v : list) {
                          range_values_.push_back(EnumValuePtr(new enum_value_gen(v)));
                          res += v.to_string() + ",";
                      }
                      res[res.length() - 1] = '}';
                      this -> set_type(res);
                  }

            template <typename InputIterator>
            enum_variable_gen(const string& id, InputIterator first, InputIterator last)
                : variable_gen(),
                  enum_item_gen(id) {
                      string res = "{";
                      while (first != last) {
                          range_values_.push_back(EnumValuePtr(new enum_value_gen(*first)));
                          res += (*first).to_string() + ",";
                          first++;
                      }
                      res[res.length() - 1] = '}';
                      this -> set_type(res);
                  }
            enum_variable_gen(const enum_variable_gen& v)
                : variable_gen(v),
                  enum_item_gen(v) {
                      copy_values(v.range_values());
                  }

            enum_variable_gen& operator=(const enum_variable_gen& v) {
                if (this != &v) {
                    variable_gen::operator=(v);
                    enum_item_gen::operator=(v);
                    copy_values(v.range_values());
                }
                return *this;
            }

            const enum_values& range_values() const {
                return range_values_;
            }

        private:
            enum_values range_values_;

            void copy_values(const enum_values& values) {
                for (auto& v : values) {
                    range_values_.push_back(EnumValuePtr(new enum_value_gen(*v)));
                }
            }
        };
    }
}

#endif /* ll_detail_enum_variable_hpp */
