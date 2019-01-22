/**
 * \file Throw.h
 * \brief Main header file of the library
 *
 *
 * \author Juraj Smiesko
 */

#ifndef THROW_H
#define THROW_H


// std
#include <string>
#include <vector>
// Root
#include <TH1.h>
#include <TGraphAsymmErrors.h>
#include <TF1.h>
#include <TH2.h>
#include <TGraph2D.h>
#include <TF2.h>
#include <TLegend.h>


namespace Throw {
  /**
   * \defgroup StringManipulation String Manipulation
   * \brief Functions for manipulation with strings.
   * @{
   */
  std::string RandomString();
  std::string RandomString(size_t);
  std::vector<std::string> SplitString(const std::string&, char);
  bool ReplaceString(std::string&, const std::string&, const std::string&);
  bool RemoveLastCharacter(std::string&, const std::string&);
  bool FindString(const std::string&, const std::string&);
  bool StringsMatch(const std::string&, const std::string&);
  /** @} */


  /**
   * \class InputParser
   * \brief Input parser.
   *
   * Solution found
   * <a href="https://stackoverflow.com/questions/865668">here</a>.
   */
  class InputParser {
    public:
      InputParser (int&, char**);
      const std::string& getCmdOption(const std::string&) const;
      bool cmdOptionExists(const std::string&) const;
    private:
      std::vector<std::string> tokens;
  };


  /**
   * \defgroup Time Time
   * \brief Time related functions.
   * @{
   */
  tm GetCurrentTime();
  std::string MonthNumToName(int);
  std::string GetCurrentYear();
  std::string GetCurrentMonth();
  std::string GetCurrentDay();
  std::string GetCurrentHour();
  std::string GetCurrentMinute();
  std::string GetCurrentSecond();
  std::string Now();
  std::string Today();
  std::string Yesterday();
  /** @} */


  /**
   * \defgroup Graph Graph
   * \brief Graph related functions.
   * @{
   */
  // Graph point
  double GetPointX(TGraph*, size_t);
  double GetPointY(TGraph*, size_t);
  void SetPointX(TGraph*, size_t, double);
  void SetPointY(TGraph*, size_t, double);

  double GetPointX(TGraphAsymmErrors*, size_t);
  double GetPointY(TGraphAsymmErrors*, size_t);
  void SetPointX(TGraphAsymmErrors*, size_t, double);
  void SetPointY(TGraphAsymmErrors*, size_t, double);

  double GetPointX(TGraph2D*, size_t);
  double GetPointY(TGraph2D*, size_t);
  double GetPointZ(TGraph2D*, size_t);
  void SetPointX(TGraph2D*, size_t, double);
  void SetPointY(TGraph2D*, size_t, double);
  void SetPointZ(TGraph2D*, size_t, double);

  // Graph minimum/maximum
  int GetMinimumIndex(TGraph*);
  int GetMinimumIndex(TGraph*, const std::string&);
  double GetMinimumX(TGraph*);
  double GetMinimumX(TGraph*, const std::string&);
  double GetMinimumY(TGraph*);
  double GetMinimumY(TGraph*, const std::string&);

  int GetMaximumIndex(TGraph*);
  int GetMaximumIndex(TGraph*, const std::string&);
  double GetMaximumX(TGraph*);
  double GetMaximumX(TGraph*, const std::string&);
  double GetMaximumY(TGraph*);
  double GetMaximumY(TGraph*, const std::string&);

  int GetMinimumIndex(TGraph2D*);
  int GetMinimumIndex(TGraph2D*, const std::string&);
  double GetMinimumX(TGraph2D*);
  double GetMinimumX(TGraph2D*, const std::string&);
  double GetMinimumY(TGraph2D*);
  double GetMinimumY(TGraph2D*, const std::string&);
  double GetMinimumZ(TGraph2D*);
  double GetMinimumZ(TGraph2D*, const std::string&);

  // Graph section
  TGraphAsymmErrors* MakeSection(TGraph2D*, double);
  TGraphAsymmErrors* MakeSection(TGraph2D*, double, double);
  /** @} */


  /**
   * \defgroup IO Input/Output
   * \brief I/O related functions.
   * @{
   */
  bool FileExists(const std::string&);
  void PrintHist(TH1D*, const std::string&);
  /** @} */


  /**
   * \class Plotter
   * \brief Base of the plotting classes.
   */
  class Plotter {
    private:
      std::vector<std::string> histDrawParamsVec;
      std::vector<std::string> graphDrawParamsVec;
      std::vector<std::string> funcDrawParamsVec;

      std::string xLabel;
      std::string yLabel;
      std::vector<std::string> noteVec;

      std::vector<int> colorVec;
      std::vector<int> markerVec;

      double xMin;
      double xMax;
      double yMin;
      double yMax;
      int logX;
      int logY;

      double xOffset;
      double yOffset;
      double tickLength;

      std::string outFilePath;

    public:
      void addHistDrawParam(const std::string&);
      void addGraphDrawParam(const std::string&);
      void addFuncDrawParam(const std::string&);
      std::string getHistDrawParam(int);
      std::string getGraphDrawParam(int);
      std::string getFuncDrawParam(int);
      void setHistDrawParam(int, const std::string&);
      void setGraphDrawParam(int, const std::string&);
      void setFuncDrawParam(int, const std::string&);

      bool drawLegend;
      double legendX1;
      double legendX2;
      double legendY1;
      double legendY2;
      std::string legendPlacement;

      bool drawAtlasLabel;
      double atlasLabelX1;
      double atlasLabelX2;
      double atlasLabelY1;
      double atlasLabelY2;
      std::string atlasLabelPlacement;

      Plotter(const std::string&);
      ~Plotter();

      void setXmin(double);
      void setXmax(double);
      void setYmin(double);
      void setYmax(double);
      double getXmin();
      double getXmax();
      double getYmin();
      double getYmax();
      void setLogX(bool);
      void setLogY(bool);
      bool getLogX(void);
      bool getLogY(void);

      void setXoffset(double);
      void setYoffset(double);
      double getXoffset();
      double getYoffset();

      void setXlabel(const std::string&);
      void setYlabel(const std::string&);
      std::string getXlabel();
      std::string getYlabel();

      std::string getNote(int);
      void setNote(int, const std::string&);
      void addNote(const std::string&);
      void addNotes(const std::vector<std::string>&);
      void putNotesToLegend(TLegend*);

      void setDrawLegend(bool);
      void setDrawAtlasLabel(bool);

      int pickColor(int);
      int pickMarker(int);

      std::string getOutFilePath();
      void setOutFilePath(const std::string&);
  };

  /**
   * \class Plotter1D
   * \brief Plotting class for 1D graphs, histograms and functions.
   */
  class Plotter1D : public Plotter {
    private:
      std::vector<TH1D*> histVec;
      std::vector<TGraphAsymmErrors*> graphVec;
      std::vector<TF1*> funcVec;

      int nObj();

      void rotateGraphsColors(int);
      void rotateGraphsMarkers(int);

      double firstNonZero();

    public:
      Plotter1D(const std::string&);
      ~Plotter1D();

      void addHist(TH1D*);
      void addGraph(TGraphAsymmErrors*);
      void addFunc(TF1*);
      TH1D* getHist(int);
      TGraphAsymmErrors* getGraph(int);
      TF1* getFunc(int);

      void draw();
  };


  /**
   * \class Plotter2D
   * \brief Plotting class for 2D graphs, histograms and functions.
   */
  class Plotter2D : public Plotter {
    private:
      std::vector<TH2D*> histVec;
      std::vector<TGraph2D*> graphVec;
      std::vector<TF2*> funcVec;

      int nObj();

      void rotateGraphsColors(int);
      void rotateGraphsMarkers(int);

    public:
      Plotter2D(const std::string&);
      ~Plotter2D();

      void addHist(TH2D*);
      void addGraph(TGraph2D*);
      void addFunc(TF2*);
      TH2D* getHist(int);
      TGraph2D* getGraph(int);
      TF2* getFunc(int);

      void draw();
  };
}


#endif /* THROW_H */
