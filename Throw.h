#ifndef THROW_H
#define THROW_H


// std
#include <string>
#include <vector>
// Root
#include <TH1.h>
#include <TGraphAsymmErrors.h>
#include <TF1.h>


// std
using std::string;
using std::vector;


namespace Throw {
  // Tools
  // https://stackoverflow.com/questions/440133
  string randomString(size_t);


  // Plotter
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


#endif /* THROW_H */
