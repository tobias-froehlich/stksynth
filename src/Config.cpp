#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include "const.h"
#include "utils.h"
#include "Config.h"

Config::Config() {
  
}

Config::Config(std::string filename) {
    read_file(filename);
}

Config::~Config() {

}

void Config::init() {
  zNames.clear();
  zValues.clear();
}

void Config::read_line(std::string line) {
  std::vector < std::string > words =
    utils::split(utils::remove_comment(line), ' ');

  if (words.size() > 0) {
    if (name_occurs(words.front())) {
      throw std::invalid_argument(
        "Parameter name must occur only once."
      );
    }

    zNames.push_back(words.front());
    words.erase(std::begin(words));
    zValues.push_back(words);
  }
}

int Config::name_occurs(std::string name) {
  return (
    std::find(
      zNames.begin(),
      zNames.end(),
      name
    )
    != zNames.end()
  );
}

std::vector < std::string > Config::get_strings(
 std::string name) {
  for ( unsigned int i = 0; i < zNames.size(); i++ ) {
    if (zNames[i] == name) {
      return zValues[i];
    }
  }
  throw std::invalid_argument(
    "Parameter name does not occur."
  );
}

std::vector < int > Config::get_ints(
  std::string name) {
   std::vector < std::string > values =
     get_strings(name);
   std::vector < int > integers;
   for(std::string value : values) {
      integers.push_back(
          std::stoi(value)
      );
   }
   return integers;
}

std::vector < double > Config::get_floats(
  std::string name) {
   std::vector < std::string > values =
     get_strings(name);
   std::vector < double > floats;
   for(std::string value : values) {
      floats.push_back(
          std::stof(value)
      );
   }
   return floats;
}

std::string Config::get_string(
  std::string name) {
  
  std::vector< std::string > values =
    get_strings(name);

  if (values.size() == 1) {
    return values[0];
  }
  throw std::invalid_argument(
    "Parameter should be just one value."
  );
}

int Config::get_int(
  std::string name) {
  
  std::vector< int > values =
    get_ints(name);

  if (values.size() == 1) {
    return values[0];
  }
  throw std::invalid_argument(
    "Parameter should be just one value."
  );
}

double Config::get_float(
  std::string name) {
  
  std::vector< double > values =
    get_floats(name);

  if (values.size() == 1) {
    return values[0];
  }
  throw std::invalid_argument(
    "Parameter should be just one value."
  );
}

void Config::read_file(std::string filename) {
  std::ifstream file(filename.c_str());

  init();

  if ( ! file ) {
    throw std::invalid_argument(
      "File not found: parameter file"
    );
  }

  std::string line;
  while ( getline(file, line) ) {
    read_line(line);
  }
}

