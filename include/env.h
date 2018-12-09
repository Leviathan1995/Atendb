// Copyright (c) 2018 The atendb Authors. All rights reserved.

#ifndef ATENDB_INCLUDE_ENV_H_
#define ATENDB_INCLUDE_ENV_H_

#include <string>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <sys/types.h>
#include <unistd.h>

#include "file/file.h"
#include "file/posix_file.h"

namespace atendb {

class Env {
 public:
  Env() {}
  virtual ~Env() {}

  static bool NewFile(const std::string& fname, File** file) {
    int32_t fd = ::open(fname.c_str(), O_RDWR | O_CREAT | O_APPEND, 0645);
    if (fd < 0) {
      file = nullptr;
      return false;
    } else {
      uint64_t offset;
      GetFileSize(fname, &offset);
      *file = new PosixFile(fd, fname, offset);
      return true;
    }
  }

  static bool FileExists(const std::string& fname) {
    if (access(fname.c_str(), F_OK) == 0) {
      return true;
    } else{
      return false;
    }
  }

  static bool CreateDir(const std::string& dirname) {
    if (mkdir(dirname.c_str(), 0755) != 0) {
      return false;
    }
    return true;
  }

  static bool GetFileSize(const std::string& fname, uint64_t* size) {
    struct stat sbuf;
    if (stat(fname.c_str(), &sbuf) != 0) {
      *size = 0;
      return false;
    } else {
      *size = sbuf.st_size;
    }
    return true;
  }

  static bool GetCurrentDir(std::string* dir) {
    char cwd[125];
    if (getcwd(cwd, sizeof(cwd)) != nullptr) {
      *dir = std::string(cwd);
      return true;
    }
    return false;
  }

};

} // namespace atendb

#endif // ATENDB_INCLUDE_ENV_H_
