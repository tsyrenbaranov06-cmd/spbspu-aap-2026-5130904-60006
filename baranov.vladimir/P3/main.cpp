#include <cstddef>
#include <fstream>
#include <iostream>
#include <limits>
#include <stdexcept>

namespace baranov {
  const int argument_error = 1;
  const int processing_error = 2;
  const std::size_t fixed_capacity = 10000;

  bool readMatrix(std::istream& input, int* matrix, std::size_t count);
  long long calculateDiagonalSum(
      const int* matrix, std::size_t rows, std::size_t cols, std::size_t row, std::size_t col);
  bool isLocalMax(const int* matrix, std::size_t cols, std::size_t row, std::size_t col);
  std::size_t countLocalMax(const int* matrix, std::size_t rows, std::size_t cols);
  long long findMaxDiagonalSum(const int* matrix, std::size_t rows, std::size_t cols);
}

bool baranov::readMatrix(std::istream& input, int* matrix, std::size_t count)
{
  for (std::size_t index = 0; index < count; ++index) {
    if (!(input >> matrix[index])) {
      return false;
    }
  }
  input >> std::ws;
  return input.eof() && !input.bad();
}

long long baranov::calculateDiagonalSum(
    const int* matrix, std::size_t rows, std::size_t cols, std::size_t row, std::size_t col)
{
  long long sum = 0;
  while ((row < rows) && (col < cols)) {
    const int value = matrix[row * cols + col];
    if ((value > 0) && (sum > std::numeric_limits< long long >::max() - value)) {
      throw std::overflow_error("Diagonal sum overflow");
    }
    if ((value < 0) && (sum < std::numeric_limits< long long >::lowest() - value)) {
      throw std::overflow_error("Diagonal sum overflow");
    }
    sum += value;
    ++row;
    ++col;
  }
  return sum;
}

bool baranov::isLocalMax(const int* matrix, std::size_t cols, std::size_t row, std::size_t col)
{
  const std::size_t position = row * cols + col;
  const int value = matrix[position];

  if ((value <= matrix[position - cols - 1]) || (value <= matrix[position - cols])
      || (value <= matrix[position - cols + 1])) {
    return false;
  }
  if ((value <= matrix[position - 1]) || (value <= matrix[position + 1])) {
    return false;
  }
  if ((value <= matrix[position + cols - 1]) || (value <= matrix[position + cols])
      || (value <= matrix[position + cols + 1])) {
    return false;
  }
  return true;
}

std::size_t baranov::countLocalMax(const int* matrix, std::size_t rows, std::size_t cols)
{
  if ((rows < 3) || (cols < 3)) {
    return 0;
  }
  std::size_t count = 0;
  for (std::size_t row = 1; row < rows - 1; ++row) {
    for (std::size_t col = 1; col < cols - 1; ++col) {
      if (isLocalMax(matrix, cols, row, col)) {
        ++count;
      }
    }
  }
  return count;
}

long long baranov::findMaxDiagonalSum(const int* matrix, std::size_t rows, std::size_t cols)
{
  if ((rows == 0) || (cols == 0)) {
    return 0;
  }
  if ((rows == 1) && (cols == 1)) {
    return 0;
  }
  long long maximum = std::numeric_limits< long long >::lowest();
  for (std::size_t row = 1; row < rows; ++row) {
    const long long sum = calculateDiagonalSum(matrix, rows, cols, row, 0);
    if (sum > maximum) {
      maximum = sum;
    }
  }
  for (std::size_t col = 1; col < cols; ++col) {
    const long long sum = calculateDiagonalSum(matrix, rows, cols, 0, col);
    if (sum > maximum) {
      maximum = sum;
    }
  }
  return maximum;
}

int main(int argc, char** argv)
{
  const int expected_arguments = 4;
  if (argc != expected_arguments) {
    std::cerr << "Usage: lab 1|2 input output\n";
    return baranov::argument_error;
  }
  if ((argv[1][0] == '\0') || (argv[1][1] != '\0')) {
    std::cerr << "Storage mode must be 1 or 2\n";
    return baranov::argument_error;
  }
  const char mode = argv[1][0];
  if ((mode != '1') && (mode != '2')) {
    std::cerr << "Storage mode must be 1 or 2\n";
    return baranov::argument_error;
  }
  std::ifstream input(argv[2]);
  if (!input) {
    std::cerr << "Cannot open input file\n";
    return baranov::argument_error;
  }
  int rows_input = 0;
  int cols_input = 0;
  if (!(input >> rows_input >> cols_input) || (rows_input < 0) || (cols_input < 0)
      || ((rows_input == 0) != (cols_input == 0))) {
    std::cerr << "Invalid matrix dimensions\n";
    return baranov::processing_error;
  }
  const std::size_t limit = std::numeric_limits< std::size_t >::max() / sizeof(int);
  const std::size_t rows = rows_input;
  const std::size_t cols = cols_input;
  if ((rows != 0) && (cols > limit / rows)) {
    std::cerr << "Matrix is too large\n";
    return baranov::processing_error;
  }
  const std::size_t count = rows * cols;
  if ((mode == '1') && (count > baranov::fixed_capacity)) {
    std::cerr << "Fixed array is too small\n";
    return baranov::processing_error;
  }
  int fixed_matrix[baranov::fixed_capacity]{};
  int* matrix = fixed_matrix;
  std::size_t local_maxima = 0;
  long long diagonal_sum = 0;

  try {
    if (mode == '2') {
      matrix = new int[count];
    }
    if (!baranov::readMatrix(input, matrix, count)) {
      throw std::invalid_argument("Invalid matrix elements");
    }
    local_maxima = baranov::countLocalMax(matrix, rows, cols);
    diagonal_sum = baranov::findMaxDiagonalSum(matrix, rows, cols);
  } catch (...) {
    if (matrix != fixed_matrix) {
      delete[] matrix;
    }
    std::cerr << "Input, memory allocation or calculation error\n";
    return baranov::processing_error;
  }
  if (matrix != fixed_matrix) {
    delete[] matrix;
  }
  input.close();
  std::ofstream output(argv[3]);
  if (!output) {
    std::cerr << "Cannot open output file\n";
    return baranov::argument_error;
  }
  if (count == 0) {
    output << "0 0\n";
  } else {
    output << local_maxima << '\n';
  }
  output << diagonal_sum << '\n';
  output.close();
  if (!output) {
    std::cerr << "Cannot write output file\n";
    return baranov::processing_error;
  }
  return 0;
}
