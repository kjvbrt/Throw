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


namespace Throw {
  /**
   * \defgroup StringManipulation
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
   * \defgroup Time
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
   * \defgroup Graph
   * \brief Graph related functions.
   * @{
   */
  // Graph point
  double GetPointX(TGraph*, size_t);
  double GetPointX(TGraphAsymmErrors*, size_t);
  double GetPointY(TGraph*, size_t);
  double GetPointY(TGraphAsymmErrors*, size_t);
  void SetPointX(TGraph*, size_t, double);
  void SetPointX(TGraphAsymmErrors*, size_t, double);
  void SetPointY(TGraph*, size_t, double);
  void SetPointY(TGraphAsymmErrors*, size_t, double);

  // Graph range
  double GetXrangeMin(TGraphAsymmErrors*);
  double GetXrangeMax(TGraphAsymmErrors*);
  double GetYrangeMin(TGraphAsymmErrors*);
  double GetYrangeMinWithErr(TGraphAsymmErrors*);
  double GetYrangeMax(TGraphAsymmErrors*);
  double GetYrangeMaxWithErr(TGraphAsymmErrors*);

  // Graph minimum/maximum
  int GetMinimumIndex(TGraphAsymmErrors*);
  int GetMinimumIndex(TGraphAsymmErrors*, const std::string&);
  double GetMinimumX(TGraphAsymmErrors*);
  double GetMinimumX(TGraphAsymmErrors*, const std::string&);
  double GetMinimumY(TGraphAsymmErrors*);
  double GetMinimumY(TGraphAsymmErrors*, const std::string&);
  int GetMaximumIndex(TGraphAsymmErrors*);
  int GetMaximumIndex(TGraphAsymmErrors*, const std::string&);
  double GetMaximumX(TGraphAsymmErrors*);
  double GetMaximumX(TGraphAsymmErrors*, const std::string&);
  double GetMaximumY(TGraphAsymmErrors*);
  double GetMaximumY(TGraphAsymmErrors*, const std::string&);
  /** @} */


  /**
   * \defgroup IO
   * \brief I/O related functions.
   * @{
   */
  bool FileExists(const std::string&);
  void PrintHist(TH1D*, const std::string&);
  /** @} */


  /**
   * \class Plotter
   * \brief Plotting class for various objects combined in one
   */
  class Plotter {
    private:
      std::vector<TH1D*> histVec;
      std::vector<TGraphAsymmErrors*> graphVec;
      std::vector<TF1*> funcVec;

      std::vector<std::string> histDrawParamsVec;
      std::vector<std::string> graphDrawParamsVec;
      std::vector<std::string> funcDrawParamsVec;

      std::string xLabel;
      std::string yLabel;
      std::vector<std::string> noteVec;

      std::vector<int> colorVec;
      std::vector<int> markerVec;

      float yMin;
      float yMax;
      float yMinObj;
      float yMaxObj;
      float yMinObjNoErr;
      int logX;
      int logY;

      float xOffset;
      float yOffset;
      float tickLength;

      std::string outFileName;

      int nObj();

    public:
      bool drawLegend;
      float legendX1;
      float legendX2;
      float legendY1;
      float legendY2;
      std::string legendPlacement;

      bool drawAtlasLabel;
      float atlasLabelX1;
      float atlasLabelX2;
      float atlasLabelY1;
      float atlasLabelY2;
      std::string atlasLabelPlacement;

      Plotter(const std::string&);
      virtual ~Plotter();
      void addHist(TH1D*);
      void addGraph(TGraphAsymmErrors*);
      void addFunc(TF1*);
      TH1D* getHist(int);
      TGraphAsymmErrors* getGraph(int);
      TF1* getFunc(int);

      std::string getHistDrawParam(int);
      std::string getGraphDrawParam(int);
      std::string getFuncDrawParam(int);
      void setHistDrawParam(int, const std::string&);
      void setGraphDrawParam(int, const std::string&);
      void setFuncDrawParam(int, const std::string&);

      void setXmin(double);
      void setXmax(double);
      void setYmin(double);
      void setYmax(double);
      void setLogX(bool);
      void setLogY(bool);

      std::string getXlabel();
      std::string getYlabel();
      std::string setXlabel();
      std::string setYlabel();
      std::string getNote(int);
      void setNote(int, const std::string&);
      void addNote(const std::string&);
      void addNotes(const std::vector<std::string>&);
      void rotateGraphsColors(int);
      void rotateGraphsMarkers(int);
      void setDrawLegend(bool);
      void setDrawAtlasLabel(bool);

      std::string getOutFileName();
      void setOutFileName(const std::string&);

      virtual void draw();
  };
}


#endif /* THROW_H */
