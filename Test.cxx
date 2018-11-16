#include "Throw.h"

// std
#include <iostream>


//std
using std::cout;
using std::endl;
using Throw::Plotter;


int main() {

  TH1D* testHistI = new TH1D("testHistI", "Test Histogram I;label x;label y",
                             20, -5, 8);
  testHistI->FillRandom("gaus", 1000);
  TH1D* testHistII = new TH1D("testHistII", "Test Histogram II;label x;label y",
                              20, -5, 8);
  testHistII->FillRandom("gaus", 4000);
  // TGraphAsymmErrors* = new TGraphAsymmErrors(5);


  Plotter* testPlot = new Plotter("testPlot");
  testPlot->addHist(testHistI);
  testPlot->addHist(testHistII);

  testPlot->setLogY(false);

  testPlot->draw();

  delete testPlot;

  return 0;
}
