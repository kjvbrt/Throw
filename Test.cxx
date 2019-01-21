// std
#include <iostream>
// Throw
#include "Throw.h"


//std
using std::cout;
using std::endl;
// Throw
using Throw::Plotter1D;
using Throw::Plotter2D;

void testPlotter1D() {
  TH1D* testHistI = new TH1D("testHistI", "Test Histogram I;label x;label y",
                             20, -5, 8);
  testHistI->FillRandom("gaus", 1000);
  TH1D* testHistII = new TH1D("testHistII", "Test Histogram II;label x;label y",
                              20, -5, 8);
  testHistII->FillRandom("gaus", 4000);
  TGraphAsymmErrors* testGraph = new TGraphAsymmErrors(5);
  testGraph->SetPoint(0, -2, 200);
  testGraph->SetPoint(1, -1, 250);
  testGraph->SetPoint(2, 0, 300);
  testGraph->SetPoint(3, 1, 350);
  testGraph->SetPoint(4, 2, 400);

  Plotter1D* testPlot = new Plotter1D("testPlot1D");
  testPlot->addHist(testHistI);
  testPlot->addHist(testHistII);
  testPlot->addGraph(testGraph);

  testPlot->setLogY(false);

  testPlot->draw();

  delete testHistI;
  delete testHistII;
  delete testGraph;
  delete testPlot;
}

void testPlotter2D() {
  TF2 *gaus2D = new TF2("gaus2D", "xygaus", -10, 10 , -10, 10);
  gaus2D->SetParameters(1, 0, 2, 0, 2);

  TH2D* testHist = new TH2D("testHist", "Test Histogram;label x;label y",
                            20, -5, 8, 20, -5, 8);
  testHist->FillRandom("gaus2D", 1000 * 1000);

  TGraph2D* testGraph = new TGraph2D(testHist);
  testGraph->SetTitle("Test Graph;label x;label y");

  Plotter2D* testPlotHist = new Plotter2D("testPlot2Dhist");
  testPlotHist->addHist(testHist);
  testPlotHist->draw();

  Plotter2D* testPlotGraph = new Plotter2D("testPlot2Dgraph");
  testPlotGraph->addGraph(testGraph);
  testPlotGraph->draw();

  delete gaus2D;
  delete testHist;
  delete testGraph;
  delete testPlotHist;
  delete testPlotGraph;
}

void testGraphSection() {
  TF2 *gaus2D = new TF2("gaus2D", "xygaus", -10, 10 , -10, 10);
  gaus2D->SetParameters(1, 0, 2, 0, 2);

  TH2D* testHist = new TH2D("testHist", "Test Histogram;label x;label y",
                            300, -5, 8, 300, -5, 8);
  testHist->FillRandom("gaus2D", 10000 * 10000);

  TGraph2D* testGraph = new TGraph2D(testHist);
  testGraph->SetTitle("Test Graph;label x;label y");

  TGraphAsymmErrors* section = Throw::MakeSection(testGraph, 100., 1.);

  Plotter1D* sectionPlot = new Plotter1D("sectionPlot");
  sectionPlot->addGraph(section);
  sectionPlot->setDrawLegend(false);
  sectionPlot->draw();

  delete gaus2D;
  delete testHist;
  delete testGraph;
  delete section;
  delete sectionPlot;
}

int main() {
  testPlotter1D();
  testPlotter2D();
  testGraphSection();

  return 0;
}
