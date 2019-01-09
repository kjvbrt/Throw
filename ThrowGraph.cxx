/**
 * \file ThrowGraph.cxx
 * \brief Implementation of useful functions.
 */


// std
#include <string>
#include <vector>
// Throw
#include "Throw.h"


// Graph point
/**
 * \brief Get x coordinate of a point
 */
double Throw::GetPointX(TGraph* graph, size_t i) {
  double x, y;
  graph->GetPoint(i, x, y);

  return x;
}

/**
 * \brief Get x coordinate of a point
 */
double Throw::GetPointX(TGraphAsymmErrors* graph, size_t i) {
  double x, y;
  graph->GetPoint(i, x, y);

  return x;
}

/**
 * \brief Get y coordinate of a point
 */
double Throw::GetPointY(TGraph* graph, size_t i) {
  double x, y;
  graph->GetPoint(i, x, y);

  return y;
}

/**
 * \brief Get y coordinate of a point
 */
double Throw::GetPointY(TGraphAsymmErrors* graph, size_t i) {
  double x, y;
  graph->GetPoint(i, x, y);

  return y;
}

/**
 * \brief Set x coordinate of a point
 */
void Throw::SetPointX(TGraph* graph, size_t i, double val) {
  double x, y;
  graph->GetPoint(i, x, y);
  graph->SetPoint(i, val, y);
}

/**
 * \brief Set x coordinate of a point
 */
void Throw::SetPointX(TGraphAsymmErrors* graph, size_t i, double val) {
  double x, y;
  graph->GetPoint(i, x, y);
  graph->SetPoint(i, val, y);
}

/**
 * \brief Set y coordinate of a point
 */
void Throw::SetPointY(TGraph* graph, size_t i, double val) {
  double x, y;
  graph->GetPoint(i, x, y);
  graph->SetPoint(i, x, val);
}

/**
 * \brief Set y coordinate of a point
 */
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
                           const std::string& param = "") {
  unsigned int n = graph->GetN();
  double* y = graph->GetY();

  if (param.find("Err") != std::string::npos) {
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
                          const std::string& param = "") {
  int i = GetMinimumIndex(graph, param);
  double x, y;
  graph->GetPoint(i, x, y);

  return x;
}

double Throw::GetMinimumY(TGraphAsymmErrors* graph,
                          const std::string& param = "") {
  int i = GetMinimumIndex(graph, param);
  double x, y;
  graph->GetPoint(i, x, y);

  return y;
}

int Throw::GetMaximumIndex(TGraphAsymmErrors* graph,
                           const std::string& param = "") {
  unsigned int n = graph->GetN();
  double* y = graph->GetY();

  if (param.find("Err") != std::string::npos) {
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
                          const std::string& param = "") {
  int i = GetMaximumIndex(graph, param);
  double x, y;
  graph->GetPoint(i, x, y);

  return x;
}

double Throw::GetMaximumY(TGraphAsymmErrors* graph,
                          const std::string& param = "") {
  int i = GetMaximumIndex(graph, param);
  double x, y;
  graph->GetPoint(i, x, y);

  return y;
}