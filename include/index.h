// Copyright (c) 2018 The atendb Authors. All rights reserved.

#ifndef ATENDB_INCLUDE_INDEX_H_
#define ATENDB_INCLUDE_INDEX_H_

#include <string>

namespace atendb {


class Index {
 public:
  Index() {}

  Index(std::string key) :
    key_(key),
    file_index_(0),
    file_offset_(0),
    key_size_(0),
    value_size_(0) {}

  Index(std::string key, uint32_t file_index, uint64_t file_offset, uint32_t key_size, uint32_t value_size) :
    key_(key),
    file_index_(file_index),
    file_offset_(file_offset),
    key_size_(key_size),
    value_size_(value_size) {}

  ~Index() {} 

  std::string Key() const { return key_; }

  uint32_t FileIndex() const { return file_index_; }

  uint64_t FileOffset() const { return file_offset_; }

  uint32_t KeySize() const { return key_size_; }

  uint32_t ValueSize() const { return value_size_; }

  uint32_t DataSize() const { return key_size_ + value_size_; }

  uint32_t IndexSize() const {
    return key_size_ + sizeof(uint32_t) * 3 + sizeof(uint64_t);
  }

  void SetDeleted() {
    this->key_size_ = 0;
    this->value_size_ = 0;
  }

  void operator=(const Index& index) {
   this->key_ = index.Key();
   this->file_index_ = index.FileIndex();
   this->file_offset_ = index.FileOffset();
   this->key_size_ = index.KeySize();
   this->value_size_ = index.ValueSize();
  }

 private:
  friend class DB;
  friend class DBImpl;

  std::string key_;
  uint32_t file_index_;
  uint64_t file_offset_;

  uint32_t key_size_;
  uint32_t value_size_;
};

} // namespace atendb

#endif // ATENDB_INCLUDE_INDEX_H_

