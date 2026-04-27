#include <iostream>
#include "io/dataref.hpp"
#include "proggy/tformat.hpp"
#include "analysis/histos.hpp"
#include "TFile.h"

using namespace tformat;

int main(){
  int runreq = 0;
  int barreq = 1234;
  INFOCON cfg = load_config("../info/config.cfg"); //read in the config from the given path

  std::cout<<"Run to plot (0 to exit): "<<std::endl; std::cin>>runreq;
  if(runreq==0) return 0; //exit code
  std::cout<<"Bar to plot (1234 to plot all): "<<std::endl; std::cin>>barreq;

  std::string InputROOTFileName = namebuilder::get_OutputROOTName(cfg, runreq); //generate an output file name
  TFile DataFile(InputROOTFileName.c_str(), "READ");
  if(DataFile.IsZombie()){
    std::cout<<"Error: could not open "<<InputROOTFileName<<std::endl;
    return 1;
  }

  std::string modifier = "";
  if(barreq!=1234) modifier = "_bar"+std::to_string(barreq);
  modifier += "_histos";
  std::string HDumpName = namebuilder::get_OutputROOTName(cfg, runreq, modifier); //generate a histos file name
  TFile HDump(HDumpName.c_str(),"UPDATE");
  if(HDump.IsZombie()){
    std::cout<<"Error: could not open histos.root"<<std::endl;
    return 1;
  }

  std::cout<<"Dumping histograms to "<<HDumpName<<std::endl;
  base_histos(DataFile, HDump); //dump the histograms
  proc_histos(DataFile, HDump); //dump the histograms
  calc_histos(DataFile, HDump, -10, barreq); //dump the histograms
  
  return 0;
}