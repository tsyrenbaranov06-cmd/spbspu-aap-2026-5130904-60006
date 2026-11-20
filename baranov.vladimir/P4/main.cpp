#include <iostream>
#include <cstddef>
#include <cctype>
#include <limits>
#include <stdexcept>

namespace baranov
{
  char * read_line(std::istream & input)
  {
    std::size_t capacity = 16;
    std::size_t size = 0;
    char * text = new char[capacity];

    try
    {
      char symbol = '\0';

      while (input.get(symbol) && symbol != '\n')
      {
        if (size == capacity - 1)
        {
          if (capacity > std::numeric_limits<std::size_t>::max() / 2)
          {
            throw std::length_error("Input is too long");
          }

          std::size_t new_capacity = capacity * 2;
          char * expanded = new char[new_capacity];

          for (std::size_t i = 0; i < size; ++i)
          {
            expanded[i] = text[i];
          }

          delete[] text;
          text = expanded;
          capacity = new_capacity;
        }

        text[size] = symbol;
        ++size;
      }

      if (input.bad() || (input.fail() && !input.eof()))
      {
        throw std::runtime_error("Input error");
      }

      text[size] = '\0';
      return text;
    }
    catch (...)
    {
      delete[] text;
      throw;
    }
  }

  char * remove_vowels(char * dest, const char * source) noexcept
  {
    std::size_t j = 0;

    for (std::size_t i = 0; source[i] != '\0'; ++i)
    {
      unsigned char symbol = source[i];
      int lower = std::tolower(symbol);

      if (lower != 'a' && lower != 'e' &&
          lower != 'i' && lower != 'o' &&
          lower != 'u' && lower != 'y')
      {
        dest[j] = source[i];
        ++j;
      }
    }

    dest[j] = '\0';
    return dest;
  }

  char * remove_spaces(char * dest, const char * source) noexcept
  {
    std::size_t j = 0;

    for (std::size_t i = 0; source[i] != '\0'; ++i)
    {
      if (source[i] != ' ')
      {
        dest[j] = source[i];
        ++j;
      }
      else if (j != 0 && dest[j - 1] != ' ')
      {
        dest[j] = ' ';
        ++j;
      }
    }

    if (j != 0 && dest[j - 1] == ' ')
    {
      --j;
    }

    dest[j] = '\0';
    return dest;
  }
}

int main()
{
  char * source = nullptr;
  char * result = nullptr;

  try
  {
    source = baranov::read_line(std::cin);

    std::size_t size = 0;

    while (source[size] != '\0')
    {
      ++size;
    }

    result = new char[size + 1];

    baranov::remove_vowels(result, source);
    std::cout << result << '\n';

    baranov::remove_spaces(result, source);
    std::cout << result << '\n';

    if (!std::cout)
    {
      throw std::runtime_error("Output error");
    }
  }
  catch (const std::exception & error)
  {
    delete[] result;
    delete[] source;
    std::cerr << error.what() << '\n';
    return 1;
  }

  delete[] result;
  delete[] source;
  return 0;
}
