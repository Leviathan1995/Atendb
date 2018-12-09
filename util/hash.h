// Copyright (c) 2018 The atendb Authors. All rights reserved.

#ifndef ATENDB_UTIL_HASH_H_
#define ATENDB_UTIL_HASH_H_

namespace atendb {

unsigned int Hash(const char* str) {
  unsigned int seed = 131;
  unsigned int hash = 0;

  while (*str) {
    hash = hash * seed + (*str++);
  }
  return (hash & 0x7FFFFFFF);
}

} // namespace atendb

#endif // ATENDB_UTIL_HASH_H_
