///
/// \file ThrowPlotter.cxx
/// \brief Implementation of the Plotter class
///


// std
#include <string>
#include <vector>
// Root
#include <TH1.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TPaveText.h>
#include <TGraphAsymmErrors.h>
#include <TF1.h>
// Throw
#include "Throw.h"


/**
 * \brief Main constructor of Plotter class.
 *
 * \param filePath path of the output file.
 */
Throw::Plotter::Plotter(const std::string& filePath) {
  xMin = 1.;
  xMax = -1.;
  yMin = 1.;
  yMax = -1.;

  logX = 0;
  logY = 0;

  drawLegend = true;
  legendX1 = .60;
  legendX2 = .98;
  legendY1 = .64;
  legendY2 = .92;
  legendPlacement = "";

  drawAtlasLabel = false;
  atlasLabelX1 = .25;
  atlasLabelX2 = .55;
  atlasLabelY1 = .15;
  atlasLabelY2 = .25;
  legendPlacement = "";

  colorVec.emplace_back(kBlack);
  colorVec.emplace_back(kBlue);
  colorVec.emplace_back(kRed);
  colorVec.emplace_back(kViolet);
  colorVec.emplace_back(kOrange + 1);
  colorVec.emplace_back(kGreen + 2);

  markerVec.emplace_back(20);
  markerVec.emplace_back(21);
  markerVec.emplace_back(22);
  markerVec.emplace_back(23);
  markerVec.emplace_back(24);

  xOffset = 1.3;
  yOffset = 1.3;
  tickLength = 0.03;

  outFilePath = filePath;
}

/**
 * \brief Default destructor of Plotter class.
 */
Throw::Plotter::~Plotter() {
  colorVec.clear();
  markerVec.clear();
}

/**
 * \brief Pick color for the object.
 */
int Throw::Plotter::pickColor(int index) {
  index = index % colorVec.size();
  return colorVec.at(index);
}

/**
 * \brief Pick marker style for the object.
 */
int Throw::Plotter::pickMarker(int index) {
  index = index % markerVec.size();
  return markerVec.at(index);
}

/**
 * \brief Get y-axis minimum.
 */
double Throw::Plotter::getYmin() {

  return yMin;
}

/**
 * \brief Get y-axis maximum.
 */
double Throw::Plotter::getYmax() {

  return yMax;
}

/**
 * \brief Set y-axis minimum.
 */
void Throw::Plotter::setYmin(double val) {
  yMin = val;
}

/**
 * \brief Set y-axis maximum.
 */
void Throw::Plotter::setYmax(double val) {
  yMax = val;
}

/**
 * \brief Get x-axis minimum.
 */
double Throw::Plotter::getXmin() {

  return xMin;
}

/**
 * \brief Get x-axis maximum.
 */
double Throw::Plotter::getXmax() {

  return xMax;
}

/**
 * \brief Set x-axis minimum.
 */
void Throw::Plotter::setXmin(double val) {
  xMin = val;
}

/**
 * \brief Set x-axis maximum.
 */
void Throw::Plotter::setXmax(double val) {
  xMax = val;
}

/**
 * \brief Returns whether x-axis will be in logarithmic scale.
 */
bool Throw::Plotter::getLogX() {

  return logX;
}

/**
 * \brief Returns whether y-axis will be in logarithmic scale.
 */
bool Throw::Plotter::getLogY() {

  return logY;
}

/**
 * \brief Sets whether x-axis will be in logarithmic scale.
 */
void Throw::Plotter::setLogX(bool val) {
  if (val) {
    logX = 1;
  } else {
    logX = 0;
  }
}

/**
 * \brief Sets whether y-axis will be in logarithmic scale.
 */
void Throw::Plotter::setLogY(bool val) {
  if (val) {
    logY = 1;
  } else {
    logY = 0;
  }
}

double Throw::Plotter::getXoffset() {

  return xOffset;
}

double Throw::Plotter::getYoffset() {

  return yOffset;
}

/**
 * \brief Get label on x-axis.
 */
std::string Throw::Plotter::getXlabel() {
  return xLabel;
}

/**
 * \brief Get label on y-axis.
 */
std::string Throw::Plotter::getYlabel() {
  return yLabel;
}

/**
 * \brief Set label on x-axis.
 */
void Throw::Plotter::setXlabel(const std::string& param) {
  xLabel = param;
}

/**
 * \brief Set label on y-axis.
 */
void Throw::Plotter::setYlabel(const std::string& param) {
  yLabel = param;
}

/**
 * \brief Add note to the list of notes.
 */
void Throw::Plotter::addNote(const std::string& note) {
  noteVec.emplace_back(note);
}

/**
 * \brief Add several notes to the list of notes at once.
 */
void Throw::Plotter::addNotes(const std::vector<std::string>& notes) {
  for (size_t i = 0; i < notes.size(); ++i) {
    noteVec.emplace_back(notes.at(i));
  }
}

/**
 * \brief Insert notes to the legend object.
 */
void Throw::Plotter::putNotesToLegend(TLegend* legend) {
  for (int i = 0; i < noteVec.size(); ++i) {
    legend->AddEntry((TObject*)0, noteVec.at(i).c_str(), "");
  }
}

/**
 * \brief Set whether legend will be drowned.
 */
void Throw::Plotter::setDrawLegend(bool val) {
  drawLegend = val;
}

/**
 * \brief Set x-axis offset.
 */
void Throw::Plotter::setXoffset(double val) {
  xOffset = val;
}

/**
 * \brief Set y-axis offset.
 */
void Throw::Plotter::setYoffset(double val) {
  yOffset = val;
}

/**
 * \brief Get drawing parameter of histogram at index.
 */
std::string Throw::Plotter::getHistDrawParam(int index) {

  return histDrawParamsVec.at(index);
}

/**
 * \brief Get drawing parameter of graph at index.
 */
std::string Throw::Plotter::getGraphDrawParam(int index) {

  return graphDrawParamsVec.at(index);
}

/**
 * \brief Get drawing parameter of function at index.
 */
std::string Throw::Plotter::getFuncDrawParam(int index) {

  return funcDrawParamsVec.at(index);
}

/**
 * \brief Set drawing parameter of histogram at index.
 */
void Throw::Plotter::setHistDrawParam(int index,
                                          const std::string& param) {
  histDrawParamsVec.at(index) = param;
}

/**
 * \brief Set drawing parameter of graph at index.
 */
void Throw::Plotter::setGraphDrawParam(int index,
                                           const std::string& param) {
  graphDrawParamsVec.at(index) = param;
}

/**
 * \brief Set drawing parameter of function at index.
 */
void Throw::Plotter::setFuncDrawParam(int index,
                                          const std::string& param) {
  funcDrawParamsVec.at(index) = param;
}

/**
 * \brief Add histogram drawing parameter to the list of histogram drawing
 * parameters.
 */
void Throw::Plotter::addHistDrawParam(const std::string& param) {
  histDrawParamsVec.emplace_back(param);
}

/**
 * \brief Add graph drawing parameter to the list of graph drawing parameters.
 */
void Throw::Plotter::addGraphDrawParam(const std::string& param) {
  graphDrawParamsVec.emplace_back(param);
}

/**
 * \brief Add function drawing parameter to the list of function drawing
 * parameters.
 */
void Throw::Plotter::addFuncDrawParam(const std::string& param) {
  funcDrawParamsVec.emplace_back(param);
}

/**
 * Get plot output file path.
 */
std::string Throw::Plotter::getOutFilePath() {

  return outFilePath;
}

/**
 * Set plot output file path.
 */
void Throw::Plotter::setOutFilePath(const std::string& filePath) {
  outFilePath = filePath;
}
