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
/**
 * \ingroup Graph
 * \brief Get index of graph minimum.
 */
int Throw::GetMinimumIndex(TGraph* graph) {

  return GetMinimumIndex(graph, "");
}

/**
 * \ingroup Graph
 * \brief Get index of graph minimum.
 */
int Throw::GetMinimumIndex(TGraph* graph,
                           const std::string& param = "") {
  unsigned int n = graph->GetN();
  double* y = graph->GetY();

  double yMin = y[0];
  int minIndex = -1;
  if (param.compare("") == 0) {
    for (size_t i = 0; i < n; ++i) {
      if (y[i] < yMin) {
        yMin = y[i];
        minIndex = i;
      }
    }
  } else if (param.compare("Err") == 0) {
    double* yErr = graph->GetEYlow();
    for (size_t i = 0; i < n; ++i) {
      if (y[i] - yErr[i] < yMin) {
        yMin = y[i] - yErr[i];
        minIndex = i;
      }
    }
  } else {
    minIndex = -1;
  }

  return minIndex;
}

/**
 * \ingroup Graph
 * \brief Get x coordinate of graph minimum.
 */
double Throw::GetMinimumX(TGraph* graph) {

  return GetMinimumX(graph, "");
}

/**
 * \ingroup Graph
 * \brief Get x coordinate of graph minimum.
 */
double Throw::GetMinimumX(TGraph* graph,
                          const std::string& param = "") {
  int i = GetMinimumIndex(graph, param);

  return GetPointX(graph, i);
}

/**
 * \ingroup Graph
 * \brief Get y coordinate of graph minimum.
 */
double Throw::GetMinimumY(TGraph* graph) {

  return GetMinimumY(graph, "");
}

/**
 * \ingroup Graph
 * \brief Get y coordinate of graph minimum.
 */
double Throw::GetMinimumY(TGraph* graph,
                          const std::string& param = "") {
  int i = GetMinimumIndex(graph, param);

  return GetPointY(graph, i);
}

/**
 * \ingroup Graph
 * \brief Get index of graph maximum.
 */
int Throw::GetMaximumIndex(TGraph* graph) {

  return GetMaximumIndex(graph, "");
}

/**
 * \ingroup Graph
 * \brief Get index of graph maximum.
 */
int Throw::GetMaximumIndex(TGraph* graph,
                           const std::string& param = "") {
  unsigned int n = graph->GetN();
  double* y = graph->GetY();

  double yMax = y[0];
  int maxIndex = -1;
  if (param.compare("") == 0) {
    for (size_t i = 0; i < n; ++i) {
      if (y[i] > yMax) {
        yMax = y[i];
        maxIndex = i;
      }
    }
  } else if (param.compare("Err") == 0) {
    double* yErr = graph->GetEYhigh();
    for (size_t i = 0; i < n; ++i) {
      if (y[i] + yErr[i] > yMax) {
        yMax = y[i] + yErr[i];
        maxIndex = i;
      }
    }
  } else {
    maxIndex = -1;
  }

  return maxIndex;
}

/**
 * \ingroup Graph
 * \brief Get x coordinate of graph maximum.
 */
double Throw::GetMaximumX(TGraph* graph) {

  return GetMaximumX(graph, "");
}

/**
 * \ingroup Graph
 * \brief Get x coordinate of graph maximum.
 */
double Throw::GetMaximumX(TGraph* graph,
                          const std::string& param = "") {
  int i = GetMaximumIndex(graph, param);

  return GetPointX(graph, i);
}

/**
 * \ingroup Graph
 * \brief Get y coordinate of graph maximum.
 */
double Throw::GetMaximumY(TGraph* graph) {

  return GetMaximumY(graph, "");
}

/**
 * \ingroup Graph
 * \brief Get y coordinate of graph maximum.
 */
double Throw::GetMaximumY(TGraph* graph,
                          const std::string& param = "") {
  int i = GetMaximumIndex(graph, param);

  return GetPointY(graph, i);
}

// Graph section
/**
 * \ingroup Graph
 * \brief Make a 2D graph section.
 */
TGraphAsymmErrors* Throw::MakeSection(TGraph2D* inGraph, double val) {

  return MakeSection(inGraph, val, val / 5.);
}

/**
 * \ingroup Graph
 * \brief Make a 2D graph section.
 */
TGraphAsymmErrors* Throw::MakeSection(TGraph2D* inGraph,
                                      double val, double delta) {
  TGraphAsymmErrors* graph = new TGraphAsymmErrors();
  std::string graphName = inGraph->GetName();
  graphName += "_at_";
  graphName += std::to_string(val);
  graph->SetName(graphName.c_str());
  std::string graphTitle = inGraph->GetTitle();
  graphTitle += " at ";
  graphTitle += std::to_string(val);
  graph->SetTitle(graphTitle.c_str());
  graph->GetXaxis()->SetTitle(inGraph->GetXaxis()->GetTitle());
  graph->GetYaxis()->SetTitle(inGraph->GetYaxis()->GetTitle());

  double x = 0.;
  double y = 0.;
  double z = 0.;
  for (size_t i = 0; i < inGraph->GetN(); ++i) {
    z = GetPointZ(inGraph, i);
    if (z > val - delta && z < val + delta) {
      x = GetPointX(inGraph, i);
      y = GetPointY(inGraph, i);
      graph->SetPoint(graph->GetN(), x, y);
    }
  }

  return graph;
}
