#ifndef BUFFER_H
#define BUFFER_H

#include <boost/algorithm/string.hpp>
#include <boost/serialization/string.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <array>
#include <string>

class buffer_t {
public:
  buffer_t();
  buffer_t(const buffer_t& rhs);
  ~buffer_t();

  void insert(const std::string &str);
  std::array<std::string, 5> search(const std::string &str);

private:
  friend class boost::serialization::access;
  std::array<std::string, 100> _buffer;
  size_t _index;
  bool _full;

  template <class Archive>
  void serialize(Archive &ar, const unsigned int version) {
    ar & _buffer;
    ar & _index;
  }
};
#endif
