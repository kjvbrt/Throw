/**
 * \file ThrowGraph.cxx
 * \brief Implementation of TGraph (TGraphAsymmErrors) related functions.
 */


// std
#include <string>
#include <vector>
// Throw
#include "Throw.h"


// Graph point
/**
 * \ingroup Graph
 * \brief Get x coordinate of a point
 */
double Throw::GetPointX(TGraph* graph, size_t index) {
  double* xArr = graph->GetX();
  if (index < 0 || index >= graph->GetN()) {
    throw "ERROR: Out of range!";
  }

  return xArr[index];
}

/**
 * \ingroup Graph
 * \brief Get y coordinate of a point
 */
double Throw::GetPointY(TGraph* graph, size_t index) {
  double* yArr = graph->GetY();
  if (index < 0 || index >= graph->GetN()) {
    throw "ERROR: Out of range!";
  }

  return yArr[index];
}

/**
 * \ingroup Graph
 * \brief Set x coordinate of a point
 */
void Throw::SetPointX(TGraph* graph, size_t index, double val) {
  double y = GetPointY(graph, index);
  graph->SetPoint(index, val, y);
}

/**
 * \ingroup Graph
 * \brief Set y coordinate of a point
 */
void Throw::SetPointY(TGraph* graph, size_t index, double val) {
  double x = GetPointX(graph, index);
  graph->SetPoint(index, x, val);
}

/**
 * \ingroup Graph
 * \brief Get x coordinate of a point
 */
double Throw::GetPointX(TGraphAsymmErrors* graph, size_t index) {
  double* xArr = graph->GetX();
  if (index < 0 || index >= graph->GetN()) {
    throw "ERROR: Out of range!";
  }

  return xArr[index];
}

/**
 * \ingroup Graph
 * \brief Get y coordinate of a point
 */
double Throw::GetPointY(TGraphAsymmErrors* graph, size_t index) {
  double* yArr = graph->GetY();
  if (index < 0 || index >= graph->GetN()) {
    throw "ERROR: Out of range!";
  }

  return yArr[index];
}

/**
 * \ingroup Graph
 * \brief Set x coordinate of a point
 */
void Throw::SetPointX(TGraphAsymmErrors* graph, size_t index, double val) {
  double y = GetPointY(graph, index);
  graph->SetPoint(index, val, y);
}

/**
 * \ingroup Graph
 * \brief Set y coordinate of a point
 */
void Throw::SetPointY(TGraphAsymmErrors* graph, size_t index, double val) {
  double x = GetPointX(graph, index);
  graph->SetPoint(index, x, val);
}

/**
 * \ingroup Graph
 * \brief Get x coordinate of a point
 */
double Throw::GetPointX(TGraph2D* graph, size_t index) {
  double* xArr = graph->GetX();
  if (index < 0 || index >= graph->GetN()) {
    throw "ERROR: Out of range!";
  }

  return xArr[index];
}

/**
 * \ingroup Graph
 * \brief Get y coordinate of a point
 */
double Throw::GetPointY(TGraph2D* graph, size_t index) {
  double* yArr = graph->GetY();
  if (index < 0 || index >= graph->GetN()) {
    throw "ERROR: Out of range!";
  }

  return yArr[index];
}

/**
 * \ingroup Graph
 * \brief Get z coordinate of a point
 */
double Throw::GetPointZ(TGraph2D* graph, size_t index) {
  double* zArr = graph->GetZ();
  if (index < 0 || index >= graph->GetN()) {
    throw "ERROR: Out of range!";
  }

  return zArr[index];
}

/**
 * \ingroup Graph
 * \brief Set x coordinate of a point
 */
void Throw::SetPointX(TGraph2D* graph, size_t index, double val) {
  double y = GetPointY(graph, index);
  double z = GetPointZ(graph, index);
  graph->SetPoint(index, val, y, z);
}

/**
 * \ingroup Graph
 * \brief Set y coordinate of a point
 */
void Throw::SetPointY(TGraph2D* graph, size_t index, double val) {
  double x = GetPointX(graph, index);
  double z = GetPointZ(graph, index);
  graph->SetPoint(index, x, val, z);
}

/**
 * \ingroup Graph
 * \brief Set z coordinate of a point
 */
void Throw::SetPointZ(TGraph2D* graph, size_t index, double val) {
  double x = GetPointX(graph, index);
  double y = GetPointY(graph, index);
  graph->SetPoint(index, x, y, val);
}


// Graph minimum/maximum
int Throw::GetMinimumIndex(TGraph* graph) {

  return GetMinimumIndex(graph, "");
}

int Throw::GetMinimumIndex(TGraph* graph,
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

double Throw::GetMinimumX(TGraph* graph) {

  return GetMinimumX(graph, "");
}

double Throw::GetMinimumX(TGraph* graph,
                          const std::string& param = "") {
  int i = GetMinimumIndex(graph, param);
  double x, y;
  graph->GetPoint(i, x, y);

  return x;
}

double Throw::GetMinimumY(TGraph* graph) {

  return GetMinimumY(graph, "");
}

double Throw::GetMinimumY(TGraph* graph,
                          const std::string& param = "") {
  int i = GetMinimumIndex(graph, param);
  double x, y;
  graph->GetPoint(i, x, y);

  return y;
}

int Throw::GetMaximumIndex(TGraph* graph) {

  return GetMaximumIndex(graph, "");
}

int Throw::GetMaximumIndex(TGraph* graph,
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

double Throw::GetMaximumX(TGraph* graph) {

  return GetMaximumX(graph, "");
}

double Throw::GetMaximumX(TGraph* graph,
                          const std::string& param = "") {
  int i = GetMaximumIndex(graph, param);
  double x, y;
  graph->GetPoint(i, x, y);

  return x;
}

double Throw::GetMaximumY(TGraph* graph) {

  return GetMaximumY(graph, "");
}

double Throw::GetMaximumY(TGraph* graph,
                          const std::string& param = "") {
  int i = GetMaximumIndex(graph, param);
  double x, y;
  graph->GetPoint(i, x, y);

  return y;
}
