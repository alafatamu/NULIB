#include <iostream>
#include "io/dataref.hpp"
#include "proggy/tformat.hpp"
#include "analysis/histos.hpp"

using namespace tformat;

int main(){
  int runreq = 0;
  INFOCON cfg = load_config("../info/config.cfg"); //read in the config from the given path

  std::cout<<"Run to plot (0 to exit): "<<std::endl; std::cin>>runreq;
  if(runreq==0) return 0; //exit code

  std::string InputROOTFileName = namebuilder::get_OutputROOTName(cfg, runreq); //generate an output file name
  std::string HDumpName = namebuilder::get_OutputROOTName(cfg, runreq, "_histos"); //generate a histos file name

  std::cout<<"Dumping histograms to "<<HDumpName<<std::endl;
  base_histos(InputROOTFileName, HDumpName); //dump the histograms
  proc_histos(InputROOTFileName, HDumpName); //dump the histograms
  calc_histos(InputROOTFileName, HDumpName); //dump the histograms
  
  return 0;
}