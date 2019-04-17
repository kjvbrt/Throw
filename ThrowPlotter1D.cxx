///
/// \file ThrowPlotter.cxx
/// \brief Implementation of the Plotter1D class
///


// std
#include <string>
#include <vector>
// Root
#include <TH1.h>
#include <TGraphAsymmErrors.h>
#include <TF1.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TPaveText.h>
// Throw
#include "Throw.h"


/**
 * \brief Main constructor of Plotter1D class.
 *
 * \param fileName name of the output file.
 */
Throw::Plotter1D::Plotter1D(const std::string& fileName) :
    Plotter::Plotter(fileName) {
}

/**
 * \brief Default destructor of Plotter1D class.
 */
Throw::Plotter1D::~Plotter1D() {
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

/**
 * \brief Total number of objects which should be plotted (histograms, graps
 * and functions).
 */
int Throw::Plotter1D::nObj() {
  return histVec.size() + graphVec.size() + funcVec.size();
}

/**
 * \brief Rotate color vector.and assign new colors to graphs.
 *
 * \param diff index difference.
 */
void Throw::Plotter1D::rotateGraphsColors(int diff) {
  for (size_t i = 0; i < graphVec.size(); ++i) {
    graphVec.at(i)->SetLineColor(pickColor(i + diff));
    graphVec.at(i)->SetMarkerColor(pickColor(i + diff));
  }
}

/**
 * \brief Rotate marker vector.and assign new colors to graphs.
 *
 * \param diff index difference.
 */
void Throw::Plotter1D::rotateGraphsMarkers(int diff = 0) {
  for (size_t i = 0; i < graphVec.size(); ++i) {
    graphVec.at(i)->SetMarkerStyle(pickMarker(i + diff));
  }
}

/**
 * \brief Add histogram to the list of histograms.
 *
 * \param inHist histogram to be added.
 */
void Throw::Plotter1D::addHist(TH1D* inHist) {
  if (!inHist) {
    throw "ERROR: Empty histogram added!";
  }

  std::string histName = inHist->GetName();
  histName += "_" + RandomString();
  TH1D* hist = dynamic_cast<TH1D*>(inHist->Clone(histName.c_str()));

  hist->SetLineColor(pickColor(nObj()));
  hist->SetMarkerColor(pickColor(nObj()));
  hist->SetMarkerStyle(pickMarker(nObj()));
  hist->SetLineWidth(2);
  hist->SetMarkerSize(.5);

  double histMin = hist->GetBinContent(1);
  double histMax = hist->GetBinContent(1);
  for (size_t i = 1; i <= hist->GetNbinsX(); ++i) {
    if (hist->GetBinContent(i) + hist->GetBinErrorUp(i) > histMax) {
      histMax = hist->GetBinContent(i) + hist->GetBinErrorUp(i);
    }
    if (hist->GetBinContent(i) - hist->GetBinErrorLow(i) < histMin) {
      histMin = hist->GetBinContent(i) - hist->GetBinErrorLow(i);
    }
  }

  if (nObj() == 0) {
    setXlabel(hist->GetXaxis()->GetTitle());
    setYlabel(hist->GetYaxis()->GetTitle());

    setYmin(histMin);
    setYmax(histMax);
  } else {
    if (histMin < getYmin()) {
      setYmin(histMin);
    }
    if (histMax > getYmax()) {
      setYmax(histMax);
    }
  }

  histVec.emplace_back(hist);
  addHistDrawParam("LE1P");
}

/**
 * \brief Add graph to the list of graphs.
 *
 * \param inGraph graph to be added.
 */
void Throw::Plotter1D::addGraph(TGraph* inGraph) {
  if (!inGraph) {
    throw "ERROR: Empty graph added!";
  }

  TGraphAsymmErrors* graph = new TGraphAsymmErrors(inGraph->GetN());
  double x = -1e9;
  double y = -1e9;
  for (size_t i = 0; i < inGraph->GetN(); ++i) {
    inGraph->GetPoint(i, x, y);
    graph->SetPoint(i, x, y);
  }

  std::string graphName = inGraph->GetName();
  graphName += "_" + RandomString();
  graph->SetName(graphName.c_str());
  graph->SetTitle(inGraph->GetTitle());
  graph->GetXaxis()->SetTitle(inGraph->GetXaxis()->GetTitle());
  graph->GetYaxis()->SetTitle(inGraph->GetYaxis()->GetTitle());

  addGraph(graph);

  delete graph;
}

/**
 * \brief Add graph to the list of graphs.
 *
 * \param inGraph graph to be added.
 */
void Throw::Plotter1D::addGraph(TGraphAsymmErrors* inGraph) {
  if (!inGraph) {
    throw "ERROR: Empty graph added!";
  }

  std::string graphName = inGraph->GetName();
  graphName += "_" + RandomString();
  TGraphAsymmErrors* graph = dynamic_cast<TGraphAsymmErrors*>(
      inGraph->Clone(graphName.c_str()));

  graph->SetLineColor(pickColor(nObj()));
  graph->SetMarkerColor(pickColor(nObj()));
  graph->SetMarkerStyle(pickMarker(nObj()));
  graph->SetLineWidth(2);
  graph->SetMarkerSize(.5);

  double graphMin = GetPointY(graph, 0);
  double graphMax = GetPointY(graph, 0);
  for (size_t i = 0; i < graph->GetN(); ++i) {
    if (GetPointY(graph, i) + graph->GetErrorYhigh(i) > graphMax) {
      graphMax = GetPointY(graph, i) + graph->GetErrorYhigh(i);
    }
    if (GetPointY(graph, i) - graph->GetErrorYlow(i) < graphMin) {
      graphMin = GetPointY(graph, i) - graph->GetErrorYlow(i);
    }
  }

  if (nObj() == 0) {
    setXlabel(graph->GetXaxis()->GetTitle());
    setYlabel(graph->GetYaxis()->GetTitle());

    setYmin(graphMin);
    setYmax(graphMax);
  } else {
    if (graphMin < getYmin()) {
      setYmin(graphMin);
    }
    if (graphMax > getYmax()) {
      setYmax(graphMax);
    }
  }

  graphVec.emplace_back(graph);
  addGraphDrawParam("E1P");
}

/**
 * \brief Get pointer to histogram at index.
 * \param index index of the histogram
 */
TH1D* Throw::Plotter1D::getHist(int index) {
  if (index >= 0 && index < histVec.size()) {
    return histVec.at(index);
  }

  return nullptr;
}

/**
 * \brief Get pointer to graph at index.
 * \param index index of the graph
 */
TGraphAsymmErrors* Throw::Plotter1D::getGraph(int index) {
  if (index >= 0 && index < graphVec.size()) {
    return graphVec.at(index);
  }

  return nullptr;
}

/**
 * \brief Plot drawing function.
 */
void Throw::Plotter1D::draw() {
  TCanvas *canvas = new TCanvas("canvas", "Canvas", 350, 350);
  gPad->SetTopMargin(.05);
  gPad->SetLeftMargin(.10);
  gPad->SetBottomMargin(.10);
  gPad->SetRightMargin(.05);

  TLegend *legend;
  if (drawLegend) {
    if (legendPlacement.find("Up") != std::string::npos) {
      legendY1 = .55;
      legendY2 = .85;
    }
    if (legendPlacement.find("Down") != std::string::npos) {
      legendY1 = .24;
      legendY2 = .45;
    }
    if (legendPlacement.find("Right") != std::string::npos) {
      legendX1 = .60;
      legendX2 = .98;
    }
    if (legendPlacement.find("Left") != std::string::npos) {
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

  gStyle->SetOptStat(0);
  gPad->SetLogx(getLogX());
  gPad->SetLogy(getLogY());

  if (getYmin() < getYmax()) {
    if (getLogY()) {
      setYmin(0.5 * getYmin());
      setYmax(1.5 * getYmax());
      if (getYmin() <= 0.) {
        setYmin(0.1 * getYmax());
        std::cerr << "WARNING: At least one data point is not positive!"
                  << std::endl;
        /**
         * \todo Find first non-zero data point in separate function.
         */
      }
    } else {
      double padding = 0.1 * fabs(getYmax());
      if (padding < 0.1 * fabs(getYmin())) {
        padding = 0.1 * fabs(getYmin());
      }

      setYmin(getYmin() - padding);
      setYmax(getYmax() + padding);
    }
  }

  int nDraw = 0;
  for (int i = 0; i < histVec.size(); ++i) {
    histVec.at(i)->GetXaxis()->SetLabelFont(43);
    histVec.at(i)->GetXaxis()->SetLabelSize(12);
    histVec.at(i)->GetXaxis()->SetTitleFont(43);
    histVec.at(i)->GetXaxis()->SetTitleSize(12);
    histVec.at(i)->GetXaxis()->SetTitleOffset(getXoffset());

    histVec.at(i)->GetYaxis()->SetLabelFont(43);
    histVec.at(i)->GetYaxis()->SetLabelSize(12);
    histVec.at(i)->GetYaxis()->SetTitleFont(43);
    histVec.at(i)->GetYaxis()->SetTitleSize(12);
    histVec.at(i)->GetYaxis()->SetTitleOffset(getYoffset());

    histVec.at(i)->SetMinimum(getYmin());
    histVec.at(i)->SetMaximum(getYmax());

    histVec.at(i)->GetXaxis()->SetTitle(getXlabel().c_str());
    histVec.at(i)->GetYaxis()->SetTitle(getYlabel().c_str());

    if (drawLegend) legend->AddEntry(histVec.at(i), histVec.at(i)->GetTitle(),
                                     getHistDrawParam(i).c_str());
    histVec.at(i)->SetTitle("");

    if (nDraw == 0) {
      histVec.at(i)->Draw(getHistDrawParam(i).c_str());
    } else {
      histVec.at(i)->Draw((getHistDrawParam(i) + "same").c_str());
    }
    ++nDraw;
  }

  for (int i = 0; i < graphVec.size(); ++i) {
    graphVec.at(i)->GetXaxis()->SetLabelFont(43);
    graphVec.at(i)->GetXaxis()->SetLabelSize(12);
    graphVec.at(i)->GetXaxis()->SetTitleFont(43);
    graphVec.at(i)->GetXaxis()->SetTitleSize(12);
    graphVec.at(i)->GetXaxis()->SetTitleOffset(getXoffset());

    graphVec.at(i)->GetYaxis()->SetLabelFont(43);
    graphVec.at(i)->GetYaxis()->SetLabelSize(12);
    graphVec.at(i)->GetYaxis()->SetTitleFont(43);
    graphVec.at(i)->GetYaxis()->SetTitleSize(12);
    graphVec.at(i)->GetYaxis()->SetTitleOffset(getYoffset());

    graphVec.at(i)->SetMinimum(getYmin());
    graphVec.at(i)->SetMaximum(getYmax());

    graphVec.at(i)->GetXaxis()->SetTitle(getXlabel().c_str());
    graphVec.at(i)->GetYaxis()->SetTitle(getYlabel().c_str());

    if (drawLegend) legend->AddEntry(graphVec.at(i), graphVec.at(i)->GetTitle(),
                                     (getGraphDrawParam(i) + "L").c_str());
    graphVec.at(i)->SetTitle("");

    if (nDraw == 0) {
      graphVec.at(i)->Draw((getGraphDrawParam(i) + "A").c_str());
    } else {
      graphVec.at(i)->Draw((getGraphDrawParam(i) + "same").c_str());
    }
    ++nDraw;
  }

  for (int i = 0; i < funcVec.size(); ++i) {
    funcVec.at(i)->GetXaxis()->SetLabelFont(43);
    funcVec.at(i)->GetXaxis()->SetLabelSize(12);
    funcVec.at(i)->GetXaxis()->SetTitleFont(43);
    funcVec.at(i)->GetXaxis()->SetTitleSize(12);
    funcVec.at(i)->GetXaxis()->SetTitleOffset(getXoffset());

    funcVec.at(i)->GetYaxis()->SetLabelFont(43);
    funcVec.at(i)->GetYaxis()->SetLabelSize(12);
    funcVec.at(i)->GetYaxis()->SetTitleFont(43);
    funcVec.at(i)->GetYaxis()->SetTitleSize(12);
    funcVec.at(i)->GetYaxis()->SetTitleOffset(getYoffset());

    funcVec.at(i)->GetXaxis()->SetTitle(getXlabel().c_str());
    funcVec.at(i)->GetYaxis()->SetTitle(getYlabel().c_str());

    if (drawLegend) legend->AddEntry(funcVec.at(i), funcVec.at(i)->GetTitle(),
                                     getFuncDrawParam(i).c_str());
    funcVec.at(i)->SetTitle("");

    funcVec.at(i)->Draw(getFuncDrawParam(i).c_str());
    if (nDraw == 0) {
      funcVec.at(i)->Draw(getFuncDrawParam(i).c_str());
    } else {
      funcVec.at(i)->Draw((getFuncDrawParam(i) + "same").c_str());
    }
    ++nDraw;
  }

  if (drawLegend) {
    putNotesToLegend(legend);
    legend->Draw();
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

    atlasLabel->Draw();
  } else {
    atlasLabel = new TPaveText();
  }

  canvas->Update();
  canvas->Print((getOutFilePath() + ".pdf").c_str());

  delete canvas;
  delete legend;
  delete atlasLabel;
}
