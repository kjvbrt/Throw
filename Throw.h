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
  // Tools
  // https://stackoverflow.com/questions/440133
  std::string randomString(size_t);
  int GetMinimumIndex(TGraphAsymmErrors*, const std::string&);
  double GetMinimumX(TGraphAsymmErrors*, const std::string&);
  double GetMinimumY(TGraphAsymmErrors*, const std::string&);
  int GetMaximumIndex(TGraphAsymmErrors*, const std::string&);
  double GetMaximumX(TGraphAsymmErrors*, const std::string&);
  double GetMaximumY(TGraphAsymmErrors*, const std::string&);



  // Plotter
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

      float xOffset;
      float yOffset;
      float tickLength;

      std::string outFileName;

      int nObj();

      float yMin;
      float yMinNoError;
      float yMax;
      float xMin;
      float xMax;

    public:
      int logX;
      int logY;

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

      std::string getOutFileName();
      void setOutFileName(const std::string&);

      virtual void draw();
  };
}


#endif /* THROW_H */
