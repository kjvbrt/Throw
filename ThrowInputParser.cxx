/**
 * \file ThrowInputParser.cxx
 * \brief Implementation of InputParser.
 */


// std
#include <string>
#include <vector>
// Throw
#include "Throw.h"


/**
 * \brief Default constructor of InputParser.
 * \param argc number of program parameters.
 * \param argv array of program parameters.
 */
Throw::InputParser::InputParser (int& argc, char** argv) {
  for (size_t i = 1; i < argc; ++i) {
    this->tokens.emplace_back(std::string(argv[i]));
  }
}

/**
 * \brief Get parameter value.
 * \param option parameter name.
 */
const std::string& Throw::InputParser::getCmdOption(
    const std::string& option) const {
  std::vector<std::string>::const_iterator itr;
  itr =  find(this->tokens.begin(), this->tokens.end(), option);
  if (itr != this->tokens.end() && ++itr != this->tokens.end()) {
    return *itr;
  }
  static const std::string empty_string;

  return empty_string;
}

/**
 * \brief Test whether the parameter exists.
 * \param option parameter name.
 */
bool Throw::InputParser::cmdOptionExists(const std::string& option) const {
    return find(this->tokens.begin(), this->tokens.end(), option)
           != this->tokens.end();
}
