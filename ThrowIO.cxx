/**
 * \file ThrowIO.cxx
 * \brief Implementation of I/O related functions.
 */


// std
#include <string>
#include <fstream>
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
