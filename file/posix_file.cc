// Copyright (c) 2018 The atendb Authors. All rights reserved.

#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <cstring>

#include "posix_file.h"

namespace atendb {

PosixFile::~PosixFile() {
  if (fd_ > 0) {
   Close();
  } 
}

bool PosixFile::Read(uint64_t offset, uint32_t n, char* buf) {
  if (offset > file_offset_) {
    uint32_t pos = offset - file_offset_;
    if (pos > map_size_) {
      return false;
    } else {
      memcpy(buf, base_buf_+pos, n);
    }
  } else {
    size_t read = pread(fd_, buf, n, offset);
    if (read < 0) {
      return false;
    }
  }
  return true;
}

bool PosixFile::AppendData(const std::string& key,
			   const std::string& value,
			   uint64_t* file_offset) {
  uint32_t data_size = key.size() + value.size();
  char data_buf[data_size];
  EncodeData(data_buf, key, value);

  *file_offset = file_offset_.fetch_add(data_size);
  int64_t cnt = 0;
  while (cnt < data_size) {
    ssize_t n = pwrite(fd_, data_buf, data_size - cnt, *file_offset + cnt);
    if (n > 0) {
      cnt += n;
    } else if (n < 0) {
      if (errno != EINTR) {
	cnt = -1;
	break;
      }
    } else {
      break;
    }
  }

  if (cnt == -1) {
    return false;
  } else {
    return true;
  }
}

bool PosixFile::AppendIndex(const std::string& key,
			    uint32_t file_index,
			    uint64_t file_offset,
			    uint32_t key_size,
			    uint32_t value_size) {
  uint32_t index_size = key.size() + sizeof(uint32_t) * 4 + sizeof(uint64_t);
  char index_buf[index_size];
  EncodeIndex(index_buf, key, file_index,
	      file_offset, key_size, value_size);

  uint64_t offset = file_offset_.fetch_add(index_size);
  int64_t cnt = 0;
  while (cnt < index_size) {
    ssize_t n = pwrite(fd_, index_buf, index_size - cnt, offset + cnt);
    if (n > 0) {
      cnt += n;
    } else if (n < 0) {
      if (errno != EINTR) {
	cnt = -1;
	break;
      }
    } else {
      break;
    }
  }

  if (cnt == -1) {
    return false;
  } else {
    return true;
  }
}

bool PosixFile::Close() {
  ::close(fd_);
  fd_ = -1;
  return true;
}

} // namespace atendb
