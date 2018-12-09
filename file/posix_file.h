// Copyright (c) 2018 The atendb Authors. All rights reserved.

#ifndef ATENDB_FILE_POSIX_FILE_H_
#define ATENDB_FILE_POSIX_FILE_H_

#include <atomic>

#include "file/file.h"
#include "util/coding.h"
#include "util/lock.h"

namespace atendb {

class PosixFile : public File {
 public:
  PosixFile(int32_t fd, const std::string& fname, size_t file_offset) :
    fd_(fd),
    fname_(fname),
    file_offset_(file_offset),
    buf_(nullptr),
    base_buf_(nullptr),
    map_size_(4096 * 10),
    map_pos_(0),
    map_avail_(0) { }

  virtual ~PosixFile();

  bool Read(uint64_t offset, uint32_t n, char* buf); 

  bool AppendData(const std::string& key, 
		  const std::string& value,
		  uint64_t* file_offset);

  bool AppendIndex(const std::string& key,
		   uint32_t file_index,
		   uint64_t file_offset,
		   uint32_t key_size_,
		   uint32_t value_size_);

  bool Close();

  inline uint64_t FileOffset() { return file_offset_; }

 private:
  uint32_t fd_;
  std::string fname_;
  std::atomic<uint64_t> file_offset_;

  char* buf_;
  char* base_buf_;
  uint32_t map_size_;
  uint32_t map_pos_;
  uint32_t map_avail_;

  SpinLock file_lock_;
};

} // namespace atendb

#endif // ATENDB_FILE_POSIX_FILE_H_
