#include "Throw.h"

// std
#include <iostream>
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
using TMath::LocMax;
using TMath::LocMin;


// Tools
// https://stackoverflow.com/questions/440133
string Throw::randomString(size_t length) {
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

int Throw::GetMinimumIndex(TGraphAsymmErrors* graph,
                           const string& param = "") {
  unsigned int n = graph->GetN();
  double* y = graph->GetY();

  if (param.find("Err") != string::npos) {
    double y_err[n];
    for (size_t i = 0; i < n; ++i) {
      y_err[i] = y[i] - graph->GetErrorYlow(i);
    }
    int minIndexErr = LocMin(n, y_err);

    return minIndexErr;
  }

  int minIndex = LocMin(n, y);

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
    int minIndexErr = LocMax(n, y_err);

    return minIndexErr;
  }

  int maxIndex = LocMax(n, y);

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
