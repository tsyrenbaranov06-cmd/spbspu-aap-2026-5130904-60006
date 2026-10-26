#include <cstddef>
#include <cstring>
#include <fstream>
#include <iostream>
#include <new>
#include "matrix.hpp"

int main(int argc, char** argv)
{
  constexpr int expected_argc = 4;
  constexpr int argument_error = 1;
  constexpr int processing_error = 2;
  constexpr int input_argument = 2;
  constexpr int output_argument = 3;
  if (argc != expected_argc || (std::strcmp(argv[1], "1") != 0 && std::strcmp(argv[1], "2") != 0))
  {
    std::cerr << "Usage: lab 1|2 input output\n";
    return argument_error;
  }
  std::ifstream input(argv[input_argument]);
  if (!input)
  {
    std::cerr << "Cannot open input file\n";
    return processing_error;
  }
  std::size_t result = 0;
  bool empty = false;
  try
  {
    if (!baranov::readAndCount(input, std::strcmp(argv[1], "2") == 0, result, empty))
    {
      std::cerr << "Invalid matrix\n";
      return processing_error;
    }
  }
  catch (const std::bad_alloc&)
  {
    std::cerr << "Memory allocation failed\n";
    return processing_error;
  }
  input.close();
  std::ofstream output(argv[output_argument]);
  if (!output)
  {
    std::cerr << "Cannot open output file\n";
    return processing_error;
  }
  if (empty)
  {
    output << "0 0\n";
  }
  else
  {
    output << result << '\n';
  }
  output.close();
  if (!output)
  {
    std::cerr << "Cannot write output file\n";
    return processing_error;
  }
  return 0;
}
