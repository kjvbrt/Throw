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

  if (param.find("E") != string::npos) {
    double y_err[n];
    for (size_t i = 0; i < n; ++i) {
      y_err[i] = y[i] - graph->GetErrorYlow(i);
      cout << "y_err: " << y_err[i] << endl;
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

  if (param.find("E") != string::npos) {
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


// Plotter
int Throw::Plotter::nObj() {
  return histVec.size() + graphVec.size() + funcVec.size();
}

Throw::Plotter::Plotter(const string& fileName) {
  yMin = 0.;
  yMinNoError = 0.;
  yMax = 1.;
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

  int binMin = hist->GetMinimumBin();
  cout << "binMin: " << binMin << endl;
  double histMin = hist->GetBinContent(binMin) - hist->GetBinError(binMin);
  cout << "histMin: " <<  histMin << endl;
  int binMinNoError = hist->GetMinimumBin();
  double histMinNoError = hist->GetBinContent(binMinNoError);
  cout << "histMinNoError: " <<  histMinNoError << endl;
  int binMax = hist->GetMaximumBin();
  double histMax = hist->GetBinContent(binMax) + hist->GetBinError(binMax);

  if (nObj() == 0) {
    xLabel = hist->GetXaxis()->GetTitle();
    yLabel = hist->GetYaxis()->GetTitle();

    yMin = histMin;
    yMinNoError = histMinNoError;
    yMax = histMax;
  } else {
    if (histMin < yMin) {
      yMin = histMin;
    }
    if (histMinNoError < yMinNoError) {
      yMinNoError = histMinNoError;
    }
    if (histMax > yMax) {
      yMax = histMax;
    }
  }
  histVec.emplace_back(hist);
  histDrawParamsVec.emplace_back("LE1P");

  return;
}

void Throw::Plotter::addGraph(TGraphAsymmErrors* inGraph) {
  string graphName = inGraph->GetName();
  graphName += "_" + randomString(6);
  TGraphAsymmErrors* graph = dynamic_cast<TGraphAsymmErrors*>(
      inGraph->Clone(graphName.c_str()));

  graph->SetLineColor(colorVec.at(nObj() % colorVec.size()));
  graph->SetMarkerColor(colorVec.at(nObj() % colorVec.size()));
  graph->SetMarkerStyle(markerVec.at(nObj() % markerVec.size()));
  graph->SetLineWidth(2);
  graph->SetMarkerSize(.5);

  double graphMin = GetMinimumY(graph, "E");
  double graphMinNoError = GetMinimumY(graph);
  double graphMax = GetMaximumY(graph, "E");

  if (nObj() == 0) {
    xLabel = graph->GetXaxis()->GetTitle();
    yLabel = graph->GetYaxis()->GetTitle();

    yMin = graphMin;
    yMinNoError = graphMinNoError;
    yMax = graphMax;
  } else {
    if (graphMin < yMin) {
      yMin = graphMin;
    }
    if (graphMinNoError < yMinNoError) {
      yMinNoError = graphMinNoError;
    }
    if (graphMax > yMax) {
      yMax = graphMax;
    }
  }
  graphVec.emplace_back(graph);
  graphDrawParamsVec.emplace_back("LE1P");

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
  } else {
    legend = new TLegend();
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
  } else {
    atlasLabel = new TPaveText();
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
    yMin = yMin - (0.5 * yMin);
    yMax = yMax + (0.5 * yMax);
    cout << "yMin: " << yMin << endl;
    if (yMin <= 0.) {
      yMin = yMinNoError - (0.5 * yMinNoError);
    }
    cout << "yMin: " << yMin << endl;
    if (yMin <= 0.) {
      yMin = 1e-9;
    }
    cout << "yMin: " << yMin << endl;
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

void Throw::Plotter::rotateGraphsColors(int diff) {
  for (size_t i = 0; i < graphVec.size(); ++i) {
    graphVec.at(i)->SetLineColor(colorVec.at(i + diff));
    graphVec.at(i)->SetMarkerColor(colorVec.at(i + diff));
  }
}

void Throw::Plotter::rotateGraphsMarkers(int diff = 0) {
  for (size_t i = 0; i < graphVec.size(); ++i) {
    graphVec.at(i)->SetMarkerStyle(markerVec.at(i + diff));
  }
}

void Throw::Plotter::setYmin(double val) {
  yMin = val;
}

void Throw::Plotter::setYmax(double val) {
  yMax = val;
}
