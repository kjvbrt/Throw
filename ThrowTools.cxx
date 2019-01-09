///
/// \file ThrowTools.cxx
/// \brief Implementation of tools (useful functions)
///

#include "Throw.h"

// std
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
// Root
#include <TH1.h>
#include <TH2.h>
#include <TMath.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TAxis.h>
#include <TPaveText.h>
#include <TGraphAsymmErrors.h>
#include <TGraph2D.h>
#include <TF1.h>


// std
using std::cout;
using std::endl;
using std::ostream;
using std::ofstream;
using std::streamsize;
using std::ios_base;
using std::basic_ios;
using std::string;
using std::vector;
using std::distance;
using std::to_string;
// Root
using TMath::MaxElement;
using TMath::MinElement;


// Tools

// Strings manipulation

/// \fn string Throw::RandomString()
/// \brief Returns random string of length 6
///
string Throw::RandomString() {
  string str = RandomString(6);

  return str;
}

/// \fn string Throw::RandomString(size_t length)
/// \brief Returns random string of variable length

/// Solution found
/// <a href="https://stackoverflow.com/questions/440133">here</a>.
///
string Throw::RandomString(size_t length) {
  auto randchar = []() -> char {
    const char charset[] =
    "0123456789"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz";
    const size_t max_index = (sizeof(charset) - 1);
    return charset[ rand() % max_index ];
  };
  string str(length, 0);
  std::generate_n(str.begin(), length, randchar);

  return str;
}

/// Splits string at delimiters.
/// Returns vector of strings.
/// https://www.fluentcpp.com/2017/04/21/how-to-split-a-string-in-c/
///
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

/// Replace sub-string in first string.
/// https://stackoverflow.com/questions/3418231
///
bool Throw::ReplaceString(std::string& firstString,
                          const std::string& fromString,
                          const std::string& toString) {
  size_t startPos = firstString.find(fromString, 0);
  if (startPos == string::npos) {
    return false;
  }

  firstString.replace(startPos, fromString.length(), toString);
  return true;
}

/// Remove last character from the first string if the first string contains
/// it.
///
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

/// Returns true if second string is found inside of the first one.
///
bool Throw::FindString(const std::string& firstString,
                       const std::string& secondString) {
  return firstString.find(secondString, 0) != std::string::npos;
}

/// Returns true if strings match exactly.
///
bool Throw::StringsMatch(const std::string& firstString,
                         const std::string& secondString) {

  return firstString.compare(secondString) == 0;
}


// Graph point
double Throw::GetPointX(TGraph* graph, size_t i) {
  double x, y;
  graph->GetPoint(i, x, y);

  return x;
}

double Throw::GetPointX(TGraphAsymmErrors* graph, size_t i) {
  double x, y;
  graph->GetPoint(i, x, y);

  return x;
}

double Throw::GetPointY(TGraph* graph, size_t i) {
  double x, y;
  graph->GetPoint(i, x, y);

  return y;
}

double Throw::GetPointY(TGraphAsymmErrors* graph, size_t i) {
  double x, y;
  graph->GetPoint(i, x, y);

  return y;
}

void Throw::SetPointX(TGraph* graph, size_t i, double val) {
  double x, y;
  graph->GetPoint(i, x, y);
  graph->SetPoint(i, val, y);
}

void Throw::SetPointX(TGraphAsymmErrors* graph, size_t i, double val) {
  double x, y;
  graph->GetPoint(i, x, y);
  graph->SetPoint(i, val, y);
}

void Throw::SetPointY(TGraph* graph, size_t i, double val) {
  double x, y;
  graph->GetPoint(i, x, y);
  graph->SetPoint(i, x, val);
}

void Throw::SetPointY(TGraphAsymmErrors* graph, size_t i, double val) {
  double x, y;
  graph->GetPoint(i, x, y);
  graph->SetPoint(i, x, val);
}

// Graph range
double Throw::GetYrangeMin(TGraphAsymmErrors* graph) {
  size_t n = graph->GetN();
  double* y = graph->GetY();

  int i = TMath::LocMin(n, y);

  return GetPointY(graph, i);
}

double Throw::GetYrangeMinWithErr(TGraphAsymmErrors* graph) {
  double rangeMin = 1e9;
  double* y = graph->GetY();
  double* y_err = graph->GetEYlow();

  for (size_t i = 0; i < graph->GetN(); ++i) {
    double y_min = y[i] - y_err[i];
    if (y_min < rangeMin) {
      rangeMin = y_min;
    }
  }

  return rangeMin;
}

double Throw::GetYrangeMax(TGraphAsymmErrors* graph) {
  size_t n = graph->GetN();
  double* y = graph->GetY();

  int i = TMath::LocMax(n, y);

  return GetPointY(graph, i);
}

double Throw::GetYrangeMaxWithErr(TGraphAsymmErrors* graph) {
  double rangeMax = -1e9;
  double* y = graph->GetY();
  double* y_err = graph->GetEYhigh();

  for (size_t i = 0; i < graph->GetN(); ++i) {
    double y_max = y[i] + y_err[i];
    if (y_max > rangeMax) {
      rangeMax = y_max;
    }
  }

  return rangeMax;
}

// Graph minimum/maximum
int Throw::GetMinimumIndex(TGraphAsymmErrors* graph,
                           const string& param = "") {
  unsigned int n = graph->GetN();
  double* y = graph->GetY();

  if (param.find("Err") != string::npos) {
    double y_err[n];
    for (size_t i = 0; i < n; ++i) {
      y_err[i] = y[i] - graph->GetErrorYlow(i);
    }
    int minIndexErr = TMath::LocMin(n, y_err);

    return minIndexErr;
  }

  int minIndex = TMath::LocMin(n, y);

  return minIndex;
}

double Throw::GetMinimumX(TGraphAsymmErrors* graph,
                          const string& param = "") {
  int i = GetMinimumIndex(graph, param);
  double x, y;
  graph->GetPoint(i, x, y);

  return x;
}

double Throw::GetMinimumY(TGraphAsymmErrors* graph,
                          const string& param = "") {
  int i = GetMinimumIndex(graph, param);
  double x, y;
  graph->GetPoint(i, x, y);

  return y;
}

int Throw::GetMaximumIndex(TGraphAsymmErrors* graph,
                           const string& param = "") {
  unsigned int n = graph->GetN();
  double* y = graph->GetY();

  if (param.find("Err") != string::npos) {
    double y_err[n];
    for (size_t i = 0; i < n; ++i) {
      y_err[i] = y[i] + graph->GetErrorYhigh(i);
    }
    int minIndexErr = TMath::LocMax(n, y_err);

    return minIndexErr;
  }

  int maxIndex = TMath::LocMax(n, y);

  return maxIndex;
}

double Throw::GetMaximumX(TGraphAsymmErrors* graph,
                          const string& param = "") {
  int i = GetMaximumIndex(graph, param);
  double x, y;
  graph->GetPoint(i, x, y);

  return x;
}

double Throw::GetMaximumY(TGraphAsymmErrors* graph,
                          const string& param = "") {
  int i = GetMaximumIndex(graph, param);
  double x, y;
  graph->GetPoint(i, x, y);

  return y;
}
