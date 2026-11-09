#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <cstddef>
#include <iosfwd>

namespace baranov
{
  std::size_t countLocalMax(const int* matrix, std::size_t rows, std::size_t cols);
  bool readAndCount(std::istream& input, bool dynamic, std::size_t& result, bool& empty);
}

#endif
