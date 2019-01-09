/**
 * \file ThrowStringManipulation.cxx
 * \brief Implementation of functions for string manipulation.
 */


// std
#include <sstream>
#include <string>
#include <vector>
// Throw
#include "Throw.h"


/**
 * \ingroup StringsManipulation
 * \brief Returns random string of length 6.
 */
std::string Throw::RandomString() {
  std::string str = RandomString(6);

  return str;
}

/**
 * \ingroup StringsManipulation
 * \brief Returns random string of variable length.
 *
 * Solution found
 * <a href="https://stackoverflow.com/questions/440133">here</a>.
 */
std::string Throw::RandomString(size_t length) {
  auto randchar = []() -> char {
    const char charset[] =
    "0123456789"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz";
    const size_t max_index = (sizeof(charset) - 1);
    return charset[ rand() % max_index ];
  };
  std::string str(length, 0);
  std::generate_n(str.begin(), length, randchar);

  return str;
}

/**
 * \ingroup StringsManipulation
 * \brief Splits string at delimiters and returns vector of strings.
 *
 * Solution found
 * <a href="https://www.fluentcpp.com/2017/04/21/">here</a>.
 */
std::vector<std::string> Throw::SplitString(const std::string& s,
                                            char delimiter) {
  std::vector<std::string> tokens;
  std::string token;
  std::istringstream tokenStream(s);
  while (std::getline(tokenStream, token, delimiter)) {
    tokens.emplace_back(token);
  }

  return tokens;
}

/**
 * \ingroup StringsManipulation
 * \brief Replace sub-string in first string.
 *
 * Solution found
 * <a href="https://stackoverflow.com/questions/3418231">here</a>.
 */
bool Throw::ReplaceString(std::string& firstString,
                          const std::string& fromString,
                          const std::string& toString) {
  size_t startPos = firstString.find(fromString, 0);
  if (startPos == std::string::npos) {
    return false;
  }

  firstString.replace(startPos, fromString.length(), toString);
  return true;
}

/**
 * \ingroup StringsManipulation
 * \brief Remove last character from the first string if the first string
 * contains it.
 */
bool Throw::RemoveLastCharacter(std::string& firstString,
                                const std::string& c) {
  if (firstString.empty()) {
    return false;
  }

  if (c.size() != 1) {
    return false;
  }

  if (c.compare(&firstString.back()) != 0) {
    return false;
  }

  firstString.replace(firstString.size() - 1, firstString.size(), "");

  return true;
}

/**
 * \ingroup StringsManipulation
 * \brief Returns true if second string is found inside of the first one.
 */
bool Throw::FindString(const std::string& firstString,
                       const std::string& secondString) {
  return firstString.find(secondString, 0) != std::string::npos;
}

/**
 * \ingroup StringsManipulation
 * \brief Returns true if strings match exactly.
 */
bool Throw::StringsMatch(const std::string& firstString,
                         const std::string& secondString) {

  return firstString.compare(secondString) == 0;
}
