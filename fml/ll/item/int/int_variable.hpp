//
//  int_variable.hpp
//  ll 
//
//  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
//  SPDX-License-Identifier: MIT
//  Copyright (c) 2020 Jinlong He.
//

#ifndef ll_int_variable_hpp 
#define ll_int_variable_hpp

#include <list>
#include <fml/ll/item/variable.hpp>
#include <fml/ll/item/int/int_value.hpp>
#include <fml/ll/item/int/int_item.hpp>
namespace ll {
    class int_variable 
        : public variable,
          public int_item {
        typedef std::unique_ptr<int_value> IntValuePtr;
        typedef std::list<IntValuePtr> int_values;
    public:
        int_variable()
            : item("", "integer"),
              variable(),
              int_item(),
              min_(nullptr),
              max_(nullptr) {}

        int_variable(const string& id)
            : item(id, "integer"),
              variable(),
              int_item(),
              min_(nullptr),
              max_(nullptr) {}

        int_variable(const string& id, 
                     const int_value& min, const int_value& max)
            : item(id, min.identifier() + ".." + max.identifier()),
              variable(),
              int_item(),
              min_(new int_value(min)),
              max_(new int_value(max)) {}

        int_variable(const string& id, 
                     int min, int max)
            : item(id, std::to_string(min) + ".." + std::to_string(max)),
              variable(),
              int_item(),
              min_(new int_value(min)),
              max_(new int_value(max)) {}

        int_variable(const string& id, std::initializer_list<int_value> list)
            : item(id),
              variable(),
              int_item(),
              min_(nullptr),
              max_(nullptr) {
                  string res = "{";
                  for (auto& v : list) {
                      range_values_.push_back(IntValuePtr(new int_value(v)));
                      res += v.identifier() + ",";
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
            : item(v),
              variable(v),
              int_item(v),
              min_(new int_value(v.min())),
              max_(new int_value(v.max())) {
                  copy_values(v.range_values());
              }

        int_variable& operator=(const int_variable& v) {
            if (this != &v) {
                item::operator=(v);
                variable::operator=(v);
                int_item::operator=(v);
                min_ = IntValuePtr(new int_value(v.min()));
                max_ = IntValuePtr(new int_value(v.max()));
                copy_values(v.range_values());
            }
            return *this;
        }

        ~int_variable() {}

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
