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


// Plotter
int Throw::Plotter::nObj() {
  return histVec.size() + graphVec.size() + funcVec.size();
}

Throw::Plotter::Plotter(const string& fileName) {
  yMin = 1.;
  yMax = -1.;
  yMinObj = 1e-9;
  yMaxObj = 1.;
  yMinObjNoErr = 1e-9;

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
  double histMin = hist->GetBinContent(binMin) - hist->GetBinError(binMin);
  int binMinNoErr = hist->GetMinimumBin();
  double histMinNoErr = hist->GetBinContent(binMinNoErr);
  int binMax = hist->GetMaximumBin();
  double histMax = hist->GetBinContent(binMax) + hist->GetBinError(binMax);

  if (nObj() == 0) {
    xLabel = hist->GetXaxis()->GetTitle();
    yLabel = hist->GetYaxis()->GetTitle();

    yMin = histMin;
    yMinObjNoErr = histMinNoErr;
    yMax = histMax;
  } else {
    if (histMin < yMin) {
      yMin = histMin;
    }
    if (histMinNoErr < yMinObjNoErr) {
      yMinObjNoErr = histMinNoErr;
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
  double graphMinNoErr = GetMinimumY(graph, "");
  double graphMax = GetMaximumY(graph, "E");

  if (nObj() == 0) {
    xLabel = graph->GetXaxis()->GetTitle();
    yLabel = graph->GetYaxis()->GetTitle();

    yMinObj = graphMin;
    yMinObjNoErr = graphMinNoErr;
    yMaxObj = graphMax;
  } else {
    if (graphMin < yMinObj) {
      yMinObj = graphMin;
    }
    if (graphMinNoErr < yMinObjNoErr) {
      yMinObjNoErr = graphMinNoErr;
    }
    if (graphMax > yMaxObj) {
      yMaxObj = graphMax;
    }
  }
  graphVec.emplace_back(graph);
  graphDrawParamsVec.emplace_back("LE1P");

  return;
}

void Throw::Plotter::addNote(const string& note) {
  noteVec.emplace_back(note);
}

void Throw::Plotter::addNotes(const vector<std::string>& notes) {
  for (size_t i = 0; i < notes.size(); ++i) {
    noteVec.emplace_back(notes.at(i));
  }
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

  if (yMin > yMax) {
    if (logY) {
      yMin = 0.5 * yMinObj;
      yMax = 1.5 * yMaxObj;
      if (yMin <= 0.) {
        yMin = 0.5 * yMinObjNoErr;
      }
      if (yMin <= 0.) {
        yMin = 0.1 * yMaxObj;
      }
      if (yMin <= 0.) {
        yMin = 1e-9;
      }
    } else {
      if (yMinObj >= 0.) {
        yMin = 0.9 * yMinObj;
      } else {
        yMin = 1.1 * yMinObj;
      }
      if (yMaxObj >= 0.) {
        yMax = 1.1 * yMaxObj;
      } else {
        yMax = 0.9 * yMaxObj;
      }
    }
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

    if (nDraw == 0) {
      histVec.at(i)->Draw(histDrawParamsVec.at(i).c_str());
    } else {
      histVec.at(i)->Draw((histDrawParamsVec.at(i) + "same").c_str());
    }
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

    graphVec.at(i)->GetXaxis()->SetTitle(xLabel.c_str());
    graphVec.at(i)->GetYaxis()->SetTitle(yLabel.c_str());

    if (drawLegend) legend->AddEntry(graphVec.at(i), graphVec.at(i)->GetTitle(),
                                     (graphDrawParamsVec.at(i) + "L").c_str());
    graphVec.at(i)->SetTitle("");

    if (nDraw == 0) {
      graphVec.at(i)->Draw((graphDrawParamsVec.at(i) + "A").c_str());
    } else {
      graphVec.at(i)->Draw((graphDrawParamsVec.at(i) + "same").c_str());
    }
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

    funcVec.at(i)->GetXaxis()->SetTitle(xLabel.c_str());
    funcVec.at(i)->GetYaxis()->SetTitle(yLabel.c_str());

    if (drawLegend) legend->AddEntry(funcVec.at(i), funcVec.at(i)->GetTitle(),
                                     funcDrawParamsVec.at(i).c_str());
    funcVec.at(i)->SetTitle("");

    funcVec.at(i)->Draw(funcDrawParamsVec.at(i).c_str());
    if (nDraw == 0) {
      funcVec.at(i)->Draw(funcDrawParamsVec.at(i).c_str());
    } else {
      funcVec.at(i)->Draw((funcDrawParamsVec.at(i) + "same").c_str());
    }
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
  delete legend;
  delete atlasLabel;
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
