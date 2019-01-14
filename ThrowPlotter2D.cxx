///
/// \file ThrowPlotter2D.cxx
/// \brief Implementation of the Plotter class
///


// std
#include <string>
#include <vector>
// Root
#include <TH2.h>
#include <TGraph2D.h>
#include <TF2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TPaveText.h>
// Throw
#include "Throw.h"


/**
 * \brief Main constructor of Plotter2D class.
 *
 * \param fileName name of the output file.
 */
Throw::Plotter2D::Plotter2D(const std::string& fileName) :
    Plotter::Plotter(fileName) {
  drawLegend = false;
}

/**
 * \brief Default destructor of Plotter2D class.
 */
Throw::Plotter2D::~Plotter2D() {
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
int Throw::Plotter2D::nObj() {
  return histVec.size() + graphVec.size() + funcVec.size();
}

/**
 * \brief Rotate color vector.and assign new colors to graphs.
 *
 * \param diff index difference.
 */
void Throw::Plotter2D::rotateGraphsColors(int diff) {
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
void Throw::Plotter2D::rotateGraphsMarkers(int diff = 0) {
  for (size_t i = 0; i < graphVec.size(); ++i) {
    graphVec.at(i)->SetMarkerStyle(pickMarker(i + diff));
  }
}

/**
 * \brief Add histogram to the list of histograms.
 *
 * \param inHist histogram to be added.
 */
void Throw::Plotter2D::addHist(TH2D* inHist) {
  std::string histName = inHist->GetName();
  histName += "_" + RandomString();
  TH2D* hist = dynamic_cast<TH2D*>(inHist->Clone(histName.c_str()));

  hist->SetLineColor(pickColor(nObj()));
  hist->SetMarkerColor(pickColor(nObj()));
  hist->SetMarkerStyle(pickMarker(nObj()));
  hist->SetLineWidth(2);
  hist->SetMarkerSize(.5);

  if (nObj() == 0) {
    setXlabel(hist->GetXaxis()->GetTitle());
    setYlabel(hist->GetYaxis()->GetTitle());
  }

  histVec.emplace_back(hist);
  addHistDrawParam("COLZ");
}

/**
 * \brief Add graph to the list of graphs.
 *
 * \param inGraph graph to be added.
 */
void Throw::Plotter2D::addGraph(TGraph2D* inGraph) {
  std::string graphName = inGraph->GetName();
  graphName += "_" + RandomString();
  TGraph2D* graph = dynamic_cast<TGraph2D*>(inGraph->Clone(graphName.c_str()));

  graph->SetLineColor(pickColor(nObj()));
  graph->SetMarkerColor(pickColor(nObj()));
  graph->SetMarkerStyle(pickMarker(nObj()));
  graph->SetLineWidth(2);
  graph->SetMarkerSize(.5);

  if (nObj() == 0) {
    setXlabel(graph->GetXaxis()->GetTitle());
    setYlabel(graph->GetYaxis()->GetTitle());
  }

  graphVec.emplace_back(graph);
  addGraphDrawParam("COLZ");
}

/**
 * \brief Plot drawing function.
 */
void Throw::Plotter2D::draw() {
  TCanvas *canvas = new TCanvas("canvas", "Canvas", 350, 350);
  gPad->SetTopMargin(.05);
  gPad->SetLeftMargin(.10);
  gPad->SetBottomMargin(.10);
  gPad->SetRightMargin(.10);

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

    histVec.at(i)->GetXaxis()->SetTitle(getXlabel().c_str());
    histVec.at(i)->GetYaxis()->SetTitle(getYlabel().c_str());

    if (drawLegend) legend->AddEntry(histVec.at(i), histVec.at(i)->GetTitle(),
                                     getHistDrawParam(i).c_str());
    histVec.at(i)->SetTitle("");

    if (nDraw == 0) {
      histVec.at(i)->Draw(getHistDrawParam(i).c_str());
      if (histVec.at(i)->GetMaximum() > 99) {
        gPad->SetRightMargin(.15);
      }
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

    graphVec.at(i)->GetXaxis()->SetTitle(getXlabel().c_str());
    graphVec.at(i)->GetHistogram()->GetXaxis()->SetTitle(getXlabel().c_str());
    graphVec.at(i)->GetYaxis()->SetTitle(getYlabel().c_str());
    graphVec.at(i)->GetHistogram()->GetYaxis()->SetTitle(getYlabel().c_str());

    if (drawLegend) legend->AddEntry(graphVec.at(i), graphVec.at(i)->GetTitle(),
                                     (getGraphDrawParam(i) + "L").c_str());
    graphVec.at(i)->SetTitle("");

    if (nDraw == 0) {
      graphVec.at(i)->Draw(getGraphDrawParam(i).c_str());
      if (graphVec.at(i)->GetZmax() > 99) {
        gPad->SetRightMargin(.15);
      }
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
