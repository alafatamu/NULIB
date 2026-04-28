#include <iostream>
#include <fstream>
#include "io/dataref.hpp"
#include "proggy/tformat.hpp"
#include "analysis/histos.hpp"
#include "TFile.h"

using namespace tformat;

int main(){
  int setreq = 0;
  int barreq = 1234;
  INFOCON cfg = load_config("../info/config.cfg"); //read in the config from the given path

  std::string setfilename = cfg.ExpInfoDir;
  std::ifstream setfile;

  std::cout<<"Set to plot (0 to exit, 1 for 6He, 2 for 9Li): "<<std::endl; std::cin>>setreq;
  if(setreq==0){
    return 0; //exit code
  }else if(setreq!=1 && setreq!=2){
    std::cout<<"Error: invalid set number"<<std::endl;
    return 1;
  }

  std::string DataFileName = namebuilder::get_OutputSETName(cfg, setreq); //generate an input file name
  TFile DataFile(DataFileName.c_str(), "READ");
  if(DataFile.IsZombie()){
    std::cout<<"Error: could not open "<<DataFileName<<std::endl;
    return 1;
  }

  std::cout<<"Bar to plot (1234 to plot all): "<<std::endl; std::cin>>barreq;

  std::string HDumpName = namebuilder::get_OutputSETName(cfg, setreq, "_histos"); //generate an output file name
  TFile HDump(HDumpName.c_str(), "UPDATE");
  if(HDump.IsZombie()){
    std::cout<<RED<<"Error: could not open ROOT file"<<RESET<<std::endl;
    return 0;
  }
  std::cout<<"Dumping histograms to "<<HDumpName<<"..."<<std::endl;

  if(barreq==1234){
    set_histos(DataFile, HDump, 1234); //dump the histograms
    for(int i=0;i<48;i++){ //expand this to include more bars based on a config input
      set_histos(DataFile, HDump, i); //dump the histograms
    }
  }else{
    set_histos(DataFile, HDump, barreq); //dump the histograms
  }
  
  DataFile.Close();
  HDump.Close();
  std::cout<<GREEN<<"Histogram generation complete!"<<RESET<<std::endl;

  return 0;
}