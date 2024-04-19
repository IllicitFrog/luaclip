#include "buffer.h"
#include <string.h>

buffer_t::buffer_t() : home(getenv("HOME")) {
  home.append("/.cache/awesome/clipboard.history");
  history = fopen(home.c_str(), "r+b");
  if (history) {
    fread(&_index, sizeof(_index), 1, history);
    if ((_index > BUFFER_SIZE - 1) || (_index < 0))
      _index = 0;
    int i = 0;
    while (fread(&_buffer[i], 4096, 1, history) && (i != BUFFER_SIZE)) {
      i++;
    }
  } else {
    history = fopen(home.c_str(), "w+b");
    _index = 0;
    fwrite(&_index, sizeof(_index), 1, history);
  }
}

buffer_t::~buffer_t() { fclose(history); }

std::array<std::string, 5> buffer_t::recent() {
  std::array<std::string, 5> ret;
  unsigned int index = _index;
  int i = 0;
  while (index != _index + 1) {
    if (_buffer[index].size != 0) {
      ret[i] = std::string(_buffer[index].data, _buffer[index].size);
      i++;
      if (i == 5)
        break;
    }
    (index <= 0) ? index = BUFFER_SIZE - 1 : index--;
  }
  return ret;
}

std::array<std::string, 5> buffer_t::search(const std::string &str) {
  std::array<std::string, 5> ret;
  unsigned int index = _index;
  int i = 0;
  while (index != _index + 1) {
    if (strstr(std::string(_buffer[index].data, _buffer[index].size).c_str(),
               str.c_str())) {
      ret[i] = std::string(_buffer[index].data, _buffer[index].size);
      i++;
      if (i == 5)
        break;
    }
    (index <= 0) ? index = BUFFER_SIZE - 1 : index--;
  }
  return ret;
}

void buffer_t::insert(std::string &str) {
  (_index >= BUFFER_SIZE - 1) ? _index = 0 : _index++;

  if (str.size() > 4092)
    str.resize(4092);
  std::lock_guard<std::mutex> lock(_mutex);
  strncpy(_buffer[_index].data, str.c_str(), str.size());
  _buffer[_index].size = str.size();

  fseek(history, 0, SEEK_SET);
  fwrite(&_index, sizeof(_index), 1, history);
  fseek(history, _index * 4096 + sizeof(_index), SEEK_SET);
  fwrite(&_buffer[_index], sizeof(clip), 1, history);
}

void buffer_t::remove(std::string &str) {
  unsigned int index = _index;
  while (index != _index + 1) {
    if (str == std::string(_buffer[index].data, _buffer[index].size)) {
      std::lock_guard<std::mutex> lock(_mutex);
      _buffer[index].size = 0;
      memset(_buffer[index].data, 0, str.size());
      fseek(history, index * 4096 + sizeof(_index), SEEK_SET);
      fwrite(&_buffer[index], sizeof(clip), 1, history);
      break;
    }
    (index <= 0) ? index = BUFFER_SIZE - 1 : index--;
  }
}
