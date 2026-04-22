#include <iostream>
#include <vector>

#include "analysis/histos.hpp"

#include "TFile.h"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void base_histos(TFile& ROOTFile){ //open a root file and make histograms based on the "raw" tree
  TFile HDump("histos.root","UPDATE");
  if(HDump.IsZombie()){
    std::cout<<"Error: could not open histos.root"<<std::endl;
    return;
  }

}