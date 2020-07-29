//
//  enum_variable.hpp
//  ll 
//
//  Created by Jinlong He on 2019/11/1.
//  Copyright © 2019年 Ruting-Team. All rights reserved.
//

#ifndef ll_enum_variable_hpp 
#define ll_enum_variable_hpp

#include "../variable.hpp"
#include "enum_item.hpp"
#include "enum_value.hpp"

namespace ll {
    class enum_variable 
        : public variable,
          public enum_item {
        typedef std::unique_ptr<enum_value> EnumValuePtr;
        typedef std::list<EnumValuePtr> enum_values;
    public:
        enum_variable()
            : variable(),
              enum_item() {
                  this -> set_type("enum");
              }

        enum_variable(const string& id)
            : variable(),
              enum_item(id) {
                  this -> set_type("enum");
              }

        enum_variable(const string& id, std::initializer_list<enum_value> list)
            : variable(),
              enum_item(id) {
                  string res = "{";
                  for (auto& v : list) {
                      range_values_.push_back(EnumValuePtr(new enum_value(v)));
                      res += v.to_string() + ",";
                  }
                  res[res.length() - 1] = '}';
                  this -> set_type(res);
              }

        template <typename InputIterator>
        enum_variable(const string& id, InputIterator first, InputIterator last)
            : variable(),
              enum_item(id) {
                  string res = "{";
                  while (first != last) {
                      range_values_.push_back(EnumValuePtr(new enum_value(*first)));
                      res += (*first).to_string() + ",";
                      first++;
                  }
                  res[res.length() - 1] = '}';
                  this -> set_type(res);
              }
        enum_variable(const enum_variable& v)
            : variable(v),
              enum_item(v) {
                  copy_values(v.range_values());
              }

        enum_variable& operator=(const enum_variable& v) {
            if (this != &v) {
                variable::operator=(v);
                enum_item::operator=(v);
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
                range_values_.push_back(EnumValuePtr(new enum_value(*v)));
            }
        }
    };
}

#endif /* ll_enum_variable_hpp */
