#include "matrix.hpp"
#include <cstddef>
#include <istream>
#include <limits>

namespace
{
  constexpr std::size_t stack_capacity = 10000;

  bool readMatrix(std::istream& input, int* matrix, std::size_t count)
  {
    for (std::size_t i = 0; i < count; ++i)
    {
      if (!(input >> matrix[i]))
      {
        return false;
      }
    }
    input >> std::ws;
    return input.eof() && !input.bad();
  }
}

std::size_t baranov::countLocalMax(const int* matrix, std::size_t rows, std::size_t cols)
{
  std::size_t count = 0;
  for (std::size_t i = 1; i + 1 < rows; ++i)
  {
    for (std::size_t j = 1; j + 1 < cols; ++j)
    {
      bool maximum = true;
      for (std::size_t row = i - 1; row <= i + 1 && maximum; ++row)
      {
        for (std::size_t col = j - 1; col <= j + 1 && maximum; ++col)
        {
          if ((row != i || col != j) && matrix[i * cols + j] <= matrix[row * cols + col])
          {
            maximum = false;
          }
        }
      }
      if (maximum)
      {
        ++count;
      }
    }
  }
  return count;
}

bool baranov::readAndCount(std::istream& input, bool dynamic, std::size_t& result, bool& empty)
{
  long long rows_input = 0;
  long long cols_input = 0;
  if (!(input >> rows_input >> cols_input) || rows_input < 0 || cols_input < 0
      || (rows_input == 0) != (cols_input == 0))
  {
    return false;
  }
  const auto limit = std::numeric_limits< std::size_t >::max() / sizeof(int);
  if (static_cast< unsigned long long >(rows_input) > limit || static_cast< unsigned long long >(cols_input) > limit)
  {
    return false;
  }
  const auto rows = static_cast< std::size_t >(rows_input);
  const auto cols = static_cast< std::size_t >(cols_input);
  if (rows != 0 && cols > limit / rows)
  {
    return false;
  }
  const std::size_t count = rows * cols;
  empty = count == 0;
  if (!dynamic)
  {
    if (count > stack_capacity)
    {
      return false;
    }
    int matrix[stack_capacity];
    if (!readMatrix(input, matrix, count))
    {
      return false;
    }
    result = countLocalMax(matrix, rows, cols);
    return true;
  }
  int* const matrix = new int[count];
  bool success = false;
  try
  {
    success = readMatrix(input, matrix, count);
  }
  catch (...)
  {
    delete[] matrix;
    throw;
  }
  if (success)
  {
    result = countLocalMax(matrix, rows, cols);
  }
  delete[] matrix;
  return success;
}
