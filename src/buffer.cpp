#include "buffer.h"

buffer_t::buffer_t() {}

buffer_t::buffer_t(const buffer_t &rhs) : _index(rhs._index) {
  std::copy(rhs._buffer.begin(), rhs._buffer.end(), _buffer.begin());
}

buffer_t::~buffer_t() {}

std::array<std::string, 5> buffer_t::search(const std::string &str) {
  std::array<std::string, 5> ret;
  size_t index = _index;
  if (str.empty()) {
    for (int i = 0; i < 5; i++) {
      ret[i] = _buffer[index];
      (index > 0) ? index-- : index = 99;
    }
  } else {
    int i = 0;
    while (index != _index + 1) {
      if (boost::algorithm::contains(_buffer[_index], str)) {
        ret[i] = _buffer[index];
        i++;
        if (i == 5)
          break;
      }
      (index > 0) ? index-- : index = 99;
    }
  }
  return ret;
}

void buffer_t::insert(const std::string &str) {
  if (_index == 99)
    _index = 0;
  else
    _index++;
  _buffer[_index] = str;
}
