#ifndef BUFFER_H
#define BUFFER_H

#include <array>
#include <string>
#include <mutex>
#include <stdio.h>

typedef struct clip {
  unsigned int size;
  char data[4092] = {0};
}clip;

class buffer_t {
public:
  buffer_t();
  ~buffer_t();

  void insert(const std::string &str);
  std::array<std::string, 5> search(const std::string &str);
  std::array<std::string, 5> recent();

private:
  clip _buffer[100];
  unsigned int _index;
  std::string home;
  std::mutex _mutex;
  FILE *history;
};
#endif
