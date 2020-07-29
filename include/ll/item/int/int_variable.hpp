//
//  int_variable.hpp
//  ll 
//
//  Created by Jinlong He on 2019/11/1.
//  Copyright © 2019年 Ruting-Team. All rights reserved.
//

#ifndef ll_int_variable_hpp 
#define ll_int_variable_hpp

#include "../variable.hpp"
#include "int_item.hpp"
#include "int_value.hpp"

namespace ll {
    class int_variable 
        : public variable,
          public int_item {
        typedef std::unique_ptr<int_value> IntValuePtr;
        typedef std::list<IntValuePtr> int_values;
    public:
        int_variable()
            : variable(),
              int_item(),
              min_(nullptr),
              max_(nullptr) {
                  this -> set_type("integer");
              }

        int_variable(const string& id)
            : variable(),
              int_item(id),
              min_(nullptr),
              max_(nullptr) {
                  this -> set_type("integer");
              }

        int_variable(const string& id, 
                         const int_value& min, const int_value& max)
            : variable(),
              int_item(id),
              min_(new int_value(min)),
              max_(new int_value(max)) {
                this -> set_type(min_ -> to_string() + ".." + max_ -> to_string());
              }

        int_variable(const string& id, std::initializer_list<int_value> list)
            : variable(),
              int_item(id),
              min_(nullptr),
              max_(nullptr) {
                  string res = "{";
                  for (auto& v : list) {
                      range_values_.push_back(IntValuePtr(new int_value(v)));
                      res += v.to_string() + ",";
                  }
                  res[res.length() - 1] = '}';
                  this -> set_type(res);
              }

        //template <typename InputIterator>
        //int_variable(const string& id, InputIterator first, InputIterator last)
        //    : variable(),
        //      int_item(id),
        //      min_(nullptr),
        //      max_(nullptr) {
        //          string res = "{";
        //          while (first != last) {
        //              range_values_.push_back(IntValuePtr(new int_value(*first)));
        //              res += first.to_string() + ",";
        //              first++;
        //          }
        //          res[res.length() - 1] = '}';
        //          this -> set_type(res);
        //      }

        int_variable(const int_variable& v)
            : variable(v),
              int_item(v),
              min_(new int_value(v.min())),
              max_(new int_value(v.max())) {
                  copy_values(v.range_values());
              }

        int_variable& operator=(const int_variable& v) {
            if (this != &v) {
                variable::operator=(v);
                int_item::operator=(v);
                min_ = IntValuePtr(new int_value(v.min()));
                max_ = IntValuePtr(new int_value(v.max()));
                copy_values(v.range_values());
            }
            return *this;
        }

        const int_values& range_values() const {
            return range_values_;
        }

        const int_value& min() const {
            return *min_;
        }

        const int_value& max() const {
            return *max_;
        }

    private:
        int_values range_values_;
        IntValuePtr min_;
        IntValuePtr max_;

        void copy_values(const int_values& values) {
            for (auto& v : values) {
                range_values_.push_back(IntValuePtr(new int_value(*v)));
            }
        }
    };
}

#endif /* ll_int_variable_hpp */
