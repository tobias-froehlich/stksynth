#ifndef UTILS_H_
#define UTILS_H_

namespace utils {

  std::vector< std::string > split(std::string str, char delimiter);
    // Splits str into words, seperated by delimiter. Several delimeters
    // occuring in a group are treated as one. Delimiters at beginning or end
    // are ignored.
  std::vector< std::string > split_equal(std::string str, int size);
    // Splits str into words of length size. If length of str is not
    // a multiple of size, then the last word is shorter.
  std::string remove_comment(std::string line);
      // removes everything from the string from the first comment tag.
      // Which character this is, is specified in const.h (zCommentTag).
  float square(float number);
      // Calculates the square of a float. Just to avoid calculating
      // the number twice like (some caclulation) * (same calculation).
  int float_equal(float a, float b);
      // Tests whether a and b differ less than cFloatDelta (in const.h).

  std::string inc_ending_number(std::string name);

  std::string intToStringLeadingZeros(
          int number, int digits);

  std::vector < int > vector_str_to_int(
    std::vector < std::string >);

  struct Color {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
  };

  Color vectorToColor(std::vector < int > vec);

  double random(double min, double max);

}



#endif
