#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include "const.h"
#include "utils.h"



std::vector< std::string > utils::split(std::string str, char delimiter) {
    std::vector< std::string > words{};
    std::string word = "";
    for (char& c : str) {
      if (c != delimiter) {
        word.append(1, c);
      }
      else if (word.size() > 0) {
        words.push_back(word);
        word = "";
      }
    }
    if (word.size() > 0) {
      words.push_back(word);
      word = "";
    }
    return words;
}

std::vector< std::string > utils::split_equal(std::string str, int size) {
  std::string word = "";
  std::vector< std::string > words{};
  int i = 0;

  for (char& c : str) {
    i += 1;  
    word.append(1, c);
    if ((i % size) == 0) {
      words.push_back(word);
      word = "";
    }
  }
  if (word.size() > 0) {
    words.push_back(word);
  }
  return words;
}

std::string utils::remove_comment(std::string line) {
  unsigned int first_pos = line.find(cCommentTag);
  return line.substr(0, first_pos);
}

float utils::square(float number) {
  return number * number;
}

int utils::float_equal(float a, float b) {
  return (std::abs(a - b) < cFloatDelta);
}

std::string utils::inc_ending_number(
 std::string name) {
  unsigned int first_index = 0;
  for ( unsigned int i=0; i<name.size(); i++) {
    if ( ! (std::isdigit(name[i])) ) {
      first_index = i;
    }
  }
  std::string trunc = "";
  std::string ending = "";
  for ( unsigned int i=0; i<name.size(); i++) {
    if (i <= first_index) {
      trunc += name[i];
    }
    else {
      ending += name[i];
    }
  }
  ending = std::to_string(std::stoi(ending) + 1);
  return trunc + ending;
}


std::string utils::intToStringLeadingZeros(
          int number, int digits) {
    std::ostringstream ss;
    ss << std::setw(digits)
            <<std::setfill('0') << number;
    return ss.str();
}


std::vector < int > utils::vector_str_to_int(
    std::vector < std::string > str_vector) {
    std::vector < int > result{};
    for(std::string s : str_vector) {
        result.push_back(std::stoi(s));
    }
    return result;
}

utils::Color utils::vectorToColor(
        std::vector < int > vec) {
    if (vec.size() != 3) {
        throw std::invalid_argument(
            "Color needs three channels!"
        );
    }
    if ((vec[0] < 0) || (vec[0] > 255)
            || (vec[1] < 0) || (vec[1] > 255)
            || (vec[2] < 0) || (vec[2] > 255)) {
        throw std::invalid_argument(
            "Color channels need values 0..255 ."
        );
    }
    Color color;
    color.red = vec[0];
    color.green = vec[1];
    color.blue = vec[2];
    return color;
}

double utils::random(double min, double max) {
    if (max - min < 0.000001) {
        return min;
    } else {
        return (
            std::rand()
         % (int)((max - min) * 1000000)
       ) * 0.000001 + min;
    }
}
