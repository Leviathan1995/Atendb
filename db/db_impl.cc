// Copyright (c) 2018 The atendb Authors. All rights reserved.

#include <iostream>
#include <thread>

#include "db/db_impl.h"
#include "util/coding.h"
#include "util/hash.h"

namespace atendb {

DBImpl::DBImpl(const std::string& dbname, const Options& options) :
  dbname_(dbname),
  options_(options),
  cursor_(0),
  max_file_(0) { }

DBImpl::~DBImpl() {
  for (auto file : data_files_) {
    delete file;
  }
  for (auto file : index_files_) {
    delete file;
  }
  for (auto table: tables_) {
    delete table;
  }
}

bool DBImpl::Put(const std::string& key, const std::string& value) {
  uint32_t cursor = cursor_++;
  uint8_t file_index = (cursor & (max_file_ - 1));
  File* data_file = data_files_[file_index];
  uint64_t file_offset;
  auto s = data_file->AppendData(key, value, &file_offset);
  if (likely(s)) {
    File* index_file = index_files_[file_index];
    s = index_file->AppendIndex(key, file_index, file_offset,
				key.size(), value.size());
    if (likely(s)) {
      Index index(key, file_index, file_offset, key.size(), value.size());
      int32_t slot = (Hash(key.c_str()) & (table_size_ - 1));
      tables_[slot]->Insert(index);
    }
  }
  return s;
}

bool DBImpl::Get(const std::string& key, std::string* value) {
  int32_t slot = (Hash(key.data()) & (table_size_ - 1));
  Index search_index(key), internal_index;
  auto s = tables_[slot]->Get(search_index, &internal_index);
  if (likely(s)) {
    if (unlikely(internal_index.KeySize() == 0 &&
		  internal_index.ValueSize() == 0)) {
      // The key has been deleted.
      *value = "";
      return false;
    }
    char buf[internal_index.DataSize()];
    auto s = data_files_[internal_index.FileIndex()]->Read(internal_index.FileOffset(),
                                                           internal_index.DataSize(),
                                                           buf);
    if (likely(s)) {
      DecodeData(buf,
		 internal_index.key_size_,
		 internal_index.value_size_,
		 value);
      return true;
    } else {
      return s;
    }
  } else {
    return s;
  }
}

bool DBImpl::Delete(const std::string& key) {
  int32_t slot = (Hash(key.data()) & (table_size_ - 1));
  Index search_index(key), internal_index;
  auto s = tables_[slot]->Get(search_index, &internal_index);
  if (likely(s)) {
    internal_index.SetDeleted();
    tables_[slot]->Insert(internal_index);
    return s;
  } else {
    return s;
  }
}

void DBImpl::IndexCallback(File* file) {
  uint64_t file_offset = file->FileOffset();
  if (file_offset != 0) { // Load index files into skiplist
    uint64_t pos = 0;
    while(pos < file_offset) {
      char size_buf[sizeof(uint32_t)];
      auto s = file->Read(pos, sizeof(uint32_t), size_buf);
      if (s) {
	      pos += sizeof(uint32_t);
	      uint32_t index_size = DecodeFixed32(size_buf);
	      char index_buf[index_size];
	      auto s = file->Read(pos, index_size, index_buf);
	      if (s) {
	        pos += index_size;
		Index index;
		DecodeIndex(index_buf,
			    &index.key_,
			    &index.file_index_,
			    &index.file_offset_,
			    &index.key_size_,
			    &index.value_size_);
	  int32_t slot = (Hash(index.key_.data()) & (table_size_ - 1));
	  tables_[slot]->Insert(index);
        }
      }
    }
  }
}

bool DBImpl::Recover() {
  // Recover
  std::vector<std::thread > threads;
  for (uint32_t i = 0; i < max_file_; i++) {
    std::thread recover(&DBImpl::IndexCallback, this, index_files_[i]);
    threads.push_back(std::move(recover));
  }

  for (uint32_t i = 0; i < max_file_; i++) {
    threads[i].join();
  }
  return true;
}


bool DB::Open(const std::string& dbname,
              const Options& options,
              DB** db) {
  *db = nullptr;
  DBImpl* impl = new DBImpl(dbname, options);

  // Get the number of processor
  int32_t num_processor = sysconf(_SC_NPROCESSORS_CONF);
  if (num_processor == -1) {
    return false;
  }

  impl->max_file_ = num_processor * 2;
  impl->table_size_ = num_processor;
  // Create skiplist tables
  for (uint32_t i = 0; i < impl->table_size_; ++i) {
    Table* table = new Table(Compare(options.comparator_));
    impl->tables_.push_back(table);
  }

  // Create data directory
  std::string current_dir;
  if (!Env::GetCurrentDir(&current_dir)) {
    return false;
  }

  std::string data_dir = current_dir + "/" + dbname;
  if (!Env::FileExists(data_dir)) {
    auto s = Env::CreateDir(data_dir);
    if (!s) {
      return false;
    }
  }

  // Open data
  for (uint32_t i = 1; i <= impl->max_file_; i++) {
    std::string data_file = data_dir + DataFileName + std::to_string(i);
    File* file;
    auto s = Env::NewFile(data_file, &file);
    if (s) {
      impl->data_files_.push_back(file);
    } else {
      return false;
    }
  }

  // Open index
  for (uint32_t i = 1; i <= impl->max_file_; i++) {
    std::string index_file = data_dir + IndexFileName + std::to_string(i);
    File* file;
    auto s = Env::NewFile(index_file, &file);
    if (s) {
      impl->index_files_.push_back(file);
    } else {
      return false;
    }
  }

  // Recover
  impl->Recover();
 
  *db = impl;
  return true;
}

} // namespace atendb
