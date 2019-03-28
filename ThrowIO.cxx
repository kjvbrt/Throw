/**
 * \file ThrowIO.cxx
 * \brief Implementation of I/O related functions.
 */


// std
#include <string>
#include <fstream>
// Root
#include <TFile.h>
// Throw
#include "Throw.h"


/**
 * \ingroup IO
 * \brief Checks whether file exists.
 *
 *  \param filePath specifies location of the file.
 */
bool Throw::FileExists(const std::string& filePath) {
  if (FILE *file = fopen(filePath.c_str(), "r")) {
    fclose(file);
    return true;
  } else {
    return false;
  }
}


/**
 * \ingroup IO
 * \brief Prints out histogram values to a file.
 *
 *  \param hist histogram to be printed out.
 *  \param filePath specifies location of the file.
 */
void Throw::PrintHist(TH1D* hist, const std::string& filePath) {
  std::ofstream outFile;
  outFile.open(filePath);

  outFile << hist->GetName() << std::endl;
  outFile << "x\ty\tx_err\ty_err" << std::endl;

  for (int i = 1; i <= hist->GetXaxis()->GetNbins(); ++i) {
    outFile << hist->GetBinCenter(i) << "\t";
    outFile << hist->GetBinContent(i) << "\t";
    outFile << hist->GetBinWidth(i) << "\t";
    outFile << hist->GetBinError(i) << std::endl;
  }

  return;
}

/**
 * \ingroup IO
 * \brief Quickly output an object to a root file.
 *
 * The root file will be created in the location of the executable and root file
 * name will be inherited from the name of the object.
 */
void Throw::QuickOut(TObject* object) {
  QuickOut(object, "", "");
}

/**
 * \ingroup IO
 * \brief Quickly output an object to a root file.
 *
 * \param path root file path without root file name.
 *
 * The name of the file will inherited from the object name.
 */
void Throw::QuickOut(TObject* object, const std::string& path) {
  QuickOut(object, path, "");
}

/**
 * \ingroup IO
 * \brief Quickly output an object to a root file.
 *
 * \param path root file path without root file name.
 * \param name object name.
 *
 * The name of the file will inherited from the object name.
 */
void Throw::QuickOut(TObject* object,
                     const std::string& path,
                     const std::string& name) {
  std::string objectPath = object->GetName();
  objectPath = "./" + objectPath;
  if (!path.empty()) {
    objectPath = path;
  }

  std::string objectName = object->GetName();
  if (!name.empty()) {
    objectName = name;
  }

  system(("mkdir -p " + objectPath).c_str());

  TFile* outFile = new TFile((objectPath + "/" + objectName + ".root").c_str(),
                             "RECREATE");
  if (outFile->IsOpen()) {
    object->Write(objectName.c_str());
    outFile->Write();
    outFile->Close();
  }

  delete outFile;
}
