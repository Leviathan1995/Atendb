// Copyright (c) 2018 The atendb Authors. All rights reserved.

#ifndef ATENDB_INCLUDE_DB_H_
#define ATENDB_INCLUDE_DB_H_

#include "include/options.h"
#include "include/env.h"
#include "include/comparator.h"

namespace atendb {

class DB {
 public:
  DB() { }
  virtual ~DB() {}

  static bool Open(const std::string& dbname, const Options& options, DB** dbptr);

  virtual bool Put(const std::string& key, const std::string& value) = 0;

  virtual bool Get(const std::string& key, std::string* value) = 0;

  virtual bool Delete(const std::string& key) = 0;

 private:
  // No copying allowed
  void operator=(const DB&);
  DB(const DB&);
};

} // namespace atendb

#endif // ATENDB_INCLUDE_DB_H_
