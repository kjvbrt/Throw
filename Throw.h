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
  // https://stackoverflow.com/questions/440133
  string randomString(size_t length) {
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


  class Plotter {
    private:
      vector<TH1D*> histVec;
      vector<TGraphAsymmErrors*> graphVec;
      vector<TF1*> funcVec;
  
      vector<string> histDrawParamsVec;
      vector<string> graphDrawParamsVec;
      vector<string> funcDrawParamsVec;
  
      string xLabel;
      string yLabel;
      vector<string> noteVec;

      vector<int> colorVec;
      vector<int> markerVec;
 
      float xOffset;
      float yOffset;
      float tickLength;

      string outFileName;

      int nObj();
  
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
      virtual ~Plotter();
      void addHist(TH1D*);
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

      string getXlabel();
      string getYlabel();
      string setXlabel();
      string setYlabel();
      string getNote(int);
      void setNote(int, const string&);
      void addNote(const string&);
      void addNotes(const vector<string>&);

      string getOutFileName();
      void setOutFileName(const string&);

      virtual void draw();
  };
}

int Throw::Plotter::nObj() {
  return histVec.size() + graphVec.size() + funcVec.size();
}

Throw::Plotter::Plotter(const string& fileName) {
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

  xOffset = 1.;
  yOffset = 1.5;
  tickLength = 0.03;

  outFileName = fileName;
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

void Throw::Plotter::addHist(TH1D* inHist) {
  string histName = inHist->GetName();
  histName += "_" + randomString(6);
  TH1D* hist = dynamic_cast<TH1D*>(inHist->Clone(histName.c_str()));

  hist->SetLineColor(colorVec.at(nObj() % colorVec.size()));
  hist->SetMarkerColor(colorVec.at(nObj() % colorVec.size()));
  hist->SetMarkerStyle(markerVec.at(nObj() % markerVec.size()));
  hist->SetLineWidth(2);
  hist->SetMarkerSize(.5);

  if (nObj() == 0) {
    xLabel = hist->GetXaxis()->GetTitle();
    yLabel = hist->GetYaxis()->GetTitle();

    yMin = hist->GetMinimum();
    yMax = hist->GetMaximum();
  } else {
    if (yMin > hist->GetMinimum()) {
      yMin = hist->GetMinimum();
    }
    if (yMax < hist->GetMaximum()) {
      yMax = hist->GetMaximum();
    }
  }
  histVec.emplace_back(hist);
  histDrawParamsVec.emplace_back("LE1P");

  return;
}

void Throw::Plotter::draw() {
  TCanvas *canvas = new TCanvas("canvas", "Canvas", 350, 350);
  gPad->SetTopMargin(.05);
  gPad->SetLeftMargin(.10);
  gPad->SetBottomMargin(.10);
  gPad->SetRightMargin(.05);

  TLegend *legend;
  if (drawLegend) {
    if (legendPlacement.find("Up") != string::npos) {
      legendY1 = .55;
      legendY2 = .85;
    }
    if (legendPlacement.find("Down") != string::npos) {
      legendY1 = .24;
      legendY2 = .45;
    }
    if (legendPlacement.find("Right") != string::npos) {
      legendX1 = .60;
      legendX2 = .98;
    }
    if (legendPlacement.find("Left") != string::npos) {
      legendX1 = .13;
      legendX2 = .55;
    }
    legend = new TLegend(legendX1, legendY1, legendX2, legendY2);
    legend->SetFillStyle(0);
    legend->SetFillColor(0);
    legend->SetShadowColor(0);
    legend->SetBorderSize(0);
    legend->SetTextFont(43);
    legend->SetTextSize(12);
  }

  TPaveText *atlasLabel;
  if (drawAtlasLabel) {
    atlasLabel = new TPaveText(atlasLabelX1, atlasLabelY1,
                               atlasLabelX2, atlasLabelY2, "NDC");
    atlasLabel->SetFillStyle(0);
    atlasLabel->SetFillColor(0);
    atlasLabel->SetShadowColor(0);
    atlasLabel->SetBorderSize(0);
    atlasLabel->SetTextFont(43);
    atlasLabel->SetTextSize(14);
    atlasLabel->AddText("#bf{#it{ATLAS} Internal #sqrt{s} = 8 TeV}");
  }

  gStyle->SetOptStat(0);
  gPad->SetLogx(logX);
  gPad->SetLogy(logY);

  int xMinBin = 1;
  int xMaxBin = 1;
  if (xMin > -1e6 && xMax > -1e6) {
    for (int i = 0; i < histVec.size(); ++i) {
      xMinBin = histVec.at(i)->FindBin(xMin) - 1;
      if (xMinBin < 1) xMinBin = 1;
      xMaxBin = histVec.at(i)->FindBin(xMax) + 1;
      if (xMaxBin > histVec.at(i)->GetNbinsX()) xMaxBin = histVec.at(i)->GetNbinsX();
      histVec.at(i)->GetXaxis()->SetRange(xMinBin, xMaxBin);
    }
    for (int i = 0; i < graphVec.size(); ++i) {
      graphVec.at(i)->GetXaxis()->SetRange(xMin, xMax);
    }
  }

  if (logY) {
    if (yMin == 0) {
      yMin = 1e-9;
    }

    yMin = yMin - 0.5 * yMin;
    yMax = yMax + 0.5 * yMax;
  } else {
    yMin = yMin - 0.1 * yMin;
    yMax = yMax + 0.1 * yMax;
  }

  int nDraw = 0;
  for (int i = 0; i < histVec.size(); ++i) {
    histVec.at(i)->GetXaxis()->SetLabelFont(43);
    histVec.at(i)->GetXaxis()->SetLabelSize(12);
    histVec.at(i)->GetXaxis()->SetTitleFont(43);
    histVec.at(i)->GetXaxis()->SetTitleSize(12);
    histVec.at(i)->GetXaxis()->SetTitleOffset(xOffset);

    histVec.at(i)->GetYaxis()->SetLabelFont(43);
    histVec.at(i)->GetYaxis()->SetLabelSize(12);
    histVec.at(i)->GetYaxis()->SetTitleFont(43);
    histVec.at(i)->GetYaxis()->SetTitleSize(12);
    histVec.at(i)->GetYaxis()->SetTitleOffset(yOffset);

    histVec.at(i)->SetMinimum(yMin);
    histVec.at(i)->SetMaximum(yMax);

    histVec.at(i)->GetXaxis()->SetTitle(xLabel.c_str());
    histVec.at(i)->GetYaxis()->SetTitle(yLabel.c_str());

    if (drawLegend) legend->AddEntry(histVec.at(i), histVec.at(i)->GetTitle(),
                                     histDrawParamsVec.at(i).c_str());
    histVec.at(i)->SetTitle("");

    if (nDraw == 0) histVec.at(i)->Draw(histDrawParamsVec.at(i).c_str());
    else histVec.at(i)->Draw((histDrawParamsVec.at(i) + "same").c_str());
    ++nDraw;
  }

  for (int i = 0; i < graphVec.size(); ++i) {
    graphVec.at(i)->GetXaxis()->SetLabelFont(43);
    graphVec.at(i)->GetXaxis()->SetLabelSize(12);
    graphVec.at(i)->GetXaxis()->SetTitleFont(43);
    graphVec.at(i)->GetXaxis()->SetTitleSize(12);
    graphVec.at(i)->GetXaxis()->SetTitleOffset(xOffset);

    graphVec.at(i)->GetYaxis()->SetLabelFont(43);
    graphVec.at(i)->GetYaxis()->SetLabelSize(12);
    graphVec.at(i)->GetYaxis()->SetTitleFont(43);
    graphVec.at(i)->GetYaxis()->SetTitleSize(12);
    graphVec.at(i)->GetYaxis()->SetTitleOffset(yOffset);

    graphVec.at(i)->SetMinimum(yMin);
    graphVec.at(i)->SetMaximum(yMax);

    if (drawLegend) legend->AddEntry(graphVec.at(i), graphVec.at(i)->GetTitle(),
                                     (graphDrawParamsVec.at(i) + "L").c_str());
    graphVec.at(i)->SetTitle("");

    if (nDraw == 0) graphVec.at(i)->Draw((graphDrawParamsVec.at(i) + "A").c_str());
    else graphVec.at(i)->Draw((graphDrawParamsVec.at(i) + "same").c_str());
    ++nDraw;
  }

  for (int i = 0; i < funcVec.size(); ++i) {
    funcVec.at(i)->GetXaxis()->SetLabelFont(43);
    funcVec.at(i)->GetXaxis()->SetLabelSize(12);
    funcVec.at(i)->GetXaxis()->SetTitleFont(43);
    funcVec.at(i)->GetXaxis()->SetTitleSize(12);
    funcVec.at(i)->GetXaxis()->SetTitleOffset(xOffset);

    funcVec.at(i)->GetYaxis()->SetLabelFont(43);
    funcVec.at(i)->GetYaxis()->SetLabelSize(12);
    funcVec.at(i)->GetYaxis()->SetTitleFont(43);
    funcVec.at(i)->GetYaxis()->SetTitleSize(12);
    funcVec.at(i)->GetYaxis()->SetTitleOffset(yOffset);

    if (drawLegend) legend->AddEntry(funcVec.at(i), funcVec.at(i)->GetTitle(),
                                     funcDrawParamsVec.at(i).c_str());
    funcVec.at(i)->SetTitle("");

    funcVec.at(i)->Draw(funcDrawParamsVec.at(i).c_str());
    if (nDraw == 0) funcVec.at(i)->Draw(funcDrawParamsVec.at(i).c_str());
    else funcVec.at(i)->Draw((funcDrawParamsVec.at(i) + "same").c_str());
    ++nDraw;
  }

  for (int i = 0; i < noteVec.size(); ++i) {
    legend->AddEntry((TObject*)0, noteVec.at(i).c_str(), "");
  }

  if (drawLegend) legend->Draw();
  if (drawAtlasLabel) atlasLabel->Draw();

  canvas->Update();
  canvas->Print((outFileName + ".pdf").c_str());

  delete canvas;
  if (drawLegend) delete legend;
  if (drawAtlasLabel) delete atlasLabel;
}

#endif /* THROW_H */
