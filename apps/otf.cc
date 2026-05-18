#include <iostream>
#include <fstream>

#include "analysis/detector.hpp"
#include "analysis/histos.hpp"

#include "io/baseroot.hpp"
#include "io/dataref.hpp"
#include "io/evthandler.hpp"

#include "proggy/config.hpp"
#include "proggy/tformat.hpp"

#include "TFile.h"

using namespace tformat;

int main(){

  INFOCON cfg = load_config("../info/config.cfg"); //read in the config from the given path
  detector texneut; 
  bool goodmapread = texneut.fill_maps(cfg); //load the detector object
  if(!goodmapread){
    std::cout<<RED<<"Detector map read failed."<<RESET<<std::endl;
    return 0;
  }

  int runreq = 1822;
  int barreq = 1234;
  std::cout<<"Run to view: "<<std::endl; std::cin>>runreq;
  std::cout<<"Bar to plot (1234 to plot all): ";std::cin>>barreq;std::cout<<std::endl; 
  if(runreq==0) return 0;

  int reproc = 0;
  std::cout<<"Reprocess? 0=no, 1=yes: ";std::cin>>reproc;
  if(reproc==1){
    std::string InputEvtName = namebuilder::get_InputEvtName(cfg, runreq); //generate an input file name
    std::ifstream InputEvtFile = grabfile(InputEvtName); //open the input file
    if(!InputEvtFile){
      std::cout<<RED<<"Error: could not open evt file"<<RESET<<std::endl;
      return 0;
    }

    std::string OutputROOTFileName = namebuilder::get_OutputROOTName(cfg, runreq); //generate an output file name
    TFile OutputROOTFile(OutputROOTFileName.c_str(), "RECREATE");
    if(OutputROOTFile.IsZombie()){
      std::cout<<RED<<"Error: could not open ROOT file"<<RESET<<std::endl;
      return 0;
    }

    int result = Evt_to_ROOT(InputEvtFile, OutputROOTFile, texneut); //read through the file
    if(result==0){
      std::cout<<RED<<"Run "<<runreq<<" read failed."<<RESET<<std::endl;
    }else if(result==1){
      std::cout<<GREEN<<"Run "<<runreq<<" successfully read."<<RESET<<std::endl;
    }

    std::cout<<"Done!"<<std::endl;
    InputEvtFile.close();
    OutputROOTFile.Close();
  }

  std::string InputROOTFileName = namebuilder::get_OutputROOTName(cfg, runreq); //generate an output file name
  TFile DataFile(InputROOTFileName.c_str(), "READ");
  if(DataFile.IsZombie()){
    std::cout<<"Error: could not open "<<InputROOTFileName<<std::endl;
  }

  std::string modifier = "";
  if(barreq!=1234) modifier = "_bar"+std::to_string(barreq);
  modifier += "_histos";
  std::string HDumpName = namebuilder::get_OutputROOTName(cfg, runreq, modifier); //generate a histos file name
  TFile HDump(HDumpName.c_str(),"UPDATE");
  if(HDump.IsZombie()){
    std::cout<<"Error: could not open histos.root"<<std::endl;
  }

  std::cout<<"Dumping histograms to "<<HDumpName<<std::endl;
  base_histos(DataFile, HDump); //dump the histograms
  proc_histos(DataFile, HDump); //dump the histograms
  calc_histos(DataFile, HDump, -10, barreq); //dump the histograms

  return 0;
}