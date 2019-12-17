//
//  int_variable.hpp
//  ll 
//
//  Created by Jinlong He on 2019/11/1.
//  Copyright © 2019年 Ruting-Team. All rights reserved.
//

#ifndef ll_detail_int_variable_hpp 
#define ll_detail_int_variable_hpp

#include "../variable.hpp"
#include "int_item.hpp"
#include "int_value.hpp"

namespace ll {
    namespace detail {
        class int_variable_gen 
            : public variable_gen,
              public int_item_gen {
            typedef std::unique_ptr<int_value_gen> IntValuePtr;
            typedef std::list<IntValuePtr> int_values;
        public:
            int_variable_gen()
                : variable_gen(),
                  int_item_gen(),
                  min_(nullptr),
                  max_(nullptr) {
                      this -> set_type("integer");
                  }

            int_variable_gen(const string& id)
                : variable_gen(),
                  int_item_gen(id),
                  min_(nullptr),
                  max_(nullptr) {
                      this -> set_type("integer");
                  }

            int_variable_gen(const string& id, 
                             const int_value_gen& min, const int_value_gen& max)
                : variable_gen(),
                  int_item_gen(id),
                  min_(new int_value_gen(min)),
                  max_(new int_value_gen(max)) {
                    this -> set_type(min_ -> to_string() + ".." + max_ -> to_string());
                  }

            int_variable_gen(const string& id, std::initializer_list<int_value_gen> list)
                : variable_gen(),
                  int_item_gen(id),
                  min_(nullptr),
                  max_(nullptr) {
                      string res = "{";
                      for (auto& v : list) {
                          range_values_.push_back(IntValuePtr(new int_value_gen(v)));
                          res += v.to_string() + ",";
                      }
                      res[res.length() - 1] = '}';
                      this -> set_type(res);
                  }

            //template <typename InputIterator>
            //int_variable_gen(const string& id, InputIterator first, InputIterator last)
            //    : variable_gen(),
            //      int_item_gen(id),
            //      min_(nullptr),
            //      max_(nullptr) {
            //          string res = "{";
            //          while (first != last) {
            //              range_values_.push_back(IntValuePtr(new int_value_gen(*first)));
            //              res += first.to_string() + ",";
            //              first++;
            //          }
            //          res[res.length() - 1] = '}';
            //          this -> set_type(res);
            //      }

            int_variable_gen(const int_variable_gen& v)
                : variable_gen(v),
                  int_item_gen(v),
                  min_(new int_value_gen(v.min())),
                  max_(new int_value_gen(v.max())) {
                      copy_values(v.range_values());
                  }

            int_variable_gen& operator=(const int_variable_gen& v) {
                if (this != &v) {
                    variable_gen::operator=(v);
                    int_item_gen::operator=(v);
                    min_ = IntValuePtr(new int_value_gen(v.min()));
                    max_ = IntValuePtr(new int_value_gen(v.max()));
                    copy_values(v.range_values());
                }
                return *this;
            }

            const int_values& range_values() const {
                return range_values_;
            }

            const int_value_gen& min() const {
                return *min_;
            }

            const int_value_gen& max() const {
                return *max_;
            }

        private:
            int_values range_values_;
            IntValuePtr min_;
            IntValuePtr max_;

            void copy_values(const int_values& values) {
                for (auto& v : values) {
                    range_values_.push_back(IntValuePtr(new int_value_gen(*v)));
                }
            }
        };
    }
}

#endif /* ll_detail_int_variable_hpp */
