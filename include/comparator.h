// Copyright (c) 2018 The atendb Authors. All rights reserved.

#ifndef ATENDB_INCLUDE_COMPARATOR_H_
#define ATENDB_INCLUDE_COMPARATOR_H_

#include <string.h>
#include "include/index.h"

namespace atendb {

class Comparator {
 public:
  virtual ~Comparator() {}
  virtual int compare(const std::string& a, const std::string& b) const = 0;
};

class BytewiseComparator : public Comparator {
 public:
  int compare(const std::string& a, const std::string& b) const {
    return a.compare(b);
  }
};

class Compare {
 public:
  Compare(Comparator* comparator)
    : comparator_(comparator) {}

  int operator()(const Index& a, const Index& b) const {
    return comparator_->compare(a.Key(), b.Key());
  }

  int operator()(const std::string& a, const std::string& b) const {
    return strcmp(a.c_str(), b.c_str());
  }

 private:
  Comparator* comparator_;
};

} // namespace atendb

#endif // ATENDB_INCLUDE_COMPARATOR_H_
