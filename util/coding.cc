// Copyright (c) 2018 The atendb Authors. All rights reserved.

#include <cassert>
#include <cstring>

#include "coding.h"

namespace atendb {

void EncodeData(char* buf, const std::string& key, const std::string& value) {
  memcpy(buf, key.data(), key.size());
  memcpy(buf+key.size(), value.data(), value.size());
  return;
}

void DecodeData(char* buf,
		uint32_t key_size,
		uint32_t value_size,
		std::string* value) {
  *value = std::string(buf+key_size, value_size);
  return;
}

void EncodeIndex(char* buf,
		 const std::string& key,
		 uint32_t file_index,
		 uint64_t file_offset,
		 uint32_t key_size,
		 uint32_t value_size) {
  assert(buf);
  size_t pos = 0;

  uint32_t index_size = key_size + sizeof(uint32_t) * 3 + sizeof(uint64_t);
  // The size of index
  memcpy(buf, &index_size, sizeof(uint32_t));
  pos += sizeof(uint32_t);
  // File index
  memcpy(buf+pos, &file_index, sizeof(uint32_t));
  pos += sizeof(uint32_t);
  // File offset
  memcpy(buf+pos, &file_offset, sizeof(uint64_t));
  pos += sizeof(uint64_t);
  // The size of key
  memcpy(buf+pos, &key_size, sizeof(uint32_t));
  pos += sizeof(uint32_t);
  // The key
  memcpy(buf+pos, key.data(), key_size);
  pos += key_size;
  // The size of value
  memcpy(buf+pos, &value_size, sizeof(uint32_t));

  return;
}

void DecodeIndex(char* buf,
                 std::string* key,
                 uint32_t* file_index,
                 uint64_t* file_offset,
                 uint32_t* key_size,
                 uint32_t* value_size) {
  assert(buf);

  size_t pos = 0;

  *file_index = DecodeFixed32(buf);
  pos += sizeof(uint32_t);

  *file_offset = DecodeFixed64(buf+pos);
  pos += sizeof(uint64_t);

  *key_size = DecodeFixed32(buf+pos);
  pos += sizeof(uint32_t);

  *key = std::string(buf+pos, *key_size);
  pos += *key_size;

  *value_size = DecodeFixed32(buf+pos);

  return;
}

uint32_t DecodeFixed32(const char* ptr) {
  uint32_t result;
  memcpy(&result, ptr, sizeof(result));
  return result;
}

uint64_t DecodeFixed64(const char* ptr) {
  uint64_t result;
  memcpy(&result, ptr, sizeof(result));
  return result;
}

} // namespace atendb
