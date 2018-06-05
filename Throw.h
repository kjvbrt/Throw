#ifndef THROW_H
#define THROW_H


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


namespace Throw {
  class Plotter {
    private:
      vector<TH1D*> histVec;
      vector<TGraphAsymmErrors*> graphVec;
      vector<TF1*> funcVec;
  
      vector<string> histDrawParamsVec;
      vector<string> graphDrawParamsVec;
      vector<string> funcDrawParamsVec;
  
      vector<string> noteVec;

      vector<int> colorVec;
      vector<int> markerVec;
  
      string outFileName;
      float tickLength;
  
    public:
      float yMin;
      float yMax;
      float xMin;
      float xMax;
      int logX;
      int logY;

      bool drawLegend;
      float legendX1;
      float legendX2;
      float legendY1;
      float legendY2;
      string legendPlacement;
      bool drawAtlasLabel;

      float atlasLabelX1;
      float atlasLabelX2;
      float atlasLabelY1;
      float atlasLabelY2;
      string atlasLabelPlacement;
  
      Plotter(const string&);
      ~Plotter();
      void addHistogram(TH1D*);
      void addGraph(TGraphAsymmErrors*);
      void addFunc(TF1*);
      TH1D* getHist(int);
      TGraphAsymmErrors* getGraph(int);
      TF1* getFunc(int);

      string getHistDrawParam(int);
      string getGraphDrawParam(int);
      string getFuncDrawParam(int);
      void setHistDrawParam(int, const string&);
      void setGraphDrawParam(int, const string&);
      void setFuncDrawParam(int, const string&);

      string getNote(int);
      void setNote(int, const string&);
      void addNote(const string&);
      void addNotes(const vector<string>&);

      string getOutFileName();
      void setOutFileName(const string&);

      virtual void draw();
  };
}

Throw::Plotter::Plotter(const string& fileName) {
  outFileName = fileName;

  yMin = -1e7;
  yMax = -1e7;
  xMin = -1e7;
  xMax = -1e7;
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
  colorVec.emplace_back(kOrange+1);
  colorVec.emplace_back(kGreen+2);

  markerVec.emplace_back(20);
  markerVec.emplace_back(21);
  markerVec.emplace_back(22);
  markerVec.emplace_back(23);
  markerVec.emplace_back(24);
}

Throw::Plotter::~Plotter() {
  for (auto &hist : histVec) {
    delete hist;
  }
  for (auto &graph : graphVec) {
    delete graph;
  }
  for (auto &func : funcVec) {
    delete func;
  }
  
  histVec.clear();
  graphVec.clear();
  funcVec.clear();
}

void Throw::Plotter::draw() {

}

#endif /* THROW_H */
