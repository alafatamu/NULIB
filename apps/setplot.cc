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

  std::cout<<"Set to convert (0 to exit, 1 for 6He, 2 for 9Li): "<<std::endl; std::cin>>setreq;
  if(setreq==0){
    return 0; //exit code
  }else if(setreq==1){
    setfilename += "6hepn.txt";
    setfile.open(setfilename);
    if(!setfile){
      std::cout<<"Error: could not open set file"<<std::endl;
      return 1;
    }
  }else if(setreq==2){
    setfilename += "9lipn.txt";
    setfile.open(setfilename);
    if(!setfile){
      std::cout<<"Error: could not open set file"<<std::endl;
      return 1;
    }
  }else{
    std::cout<<"Error: invalid set number"<<std::endl;
    return 1;
  }

  std::string DataFileName = namebuilder::get_OutputSETName(cfg, setreq); //generate an output file name
  TFile DataFile(DataFileName.c_str(), "READ");
  if(DataFile.IsZombie()){
    std::cout<<"Error: could not open "<<DataFileName<<std::endl;
    return 1;
  }

  std::cout<<"Bar to plot (1234 to plot all): "<<std::endl; std::cin>>barreq;

  std::string modifier = "";
  if(barreq!=1234) modifier += "_bar"+std::to_string(barreq);
  modifier += "_histos";
  std::string HDumpName = namebuilder::get_OutputSETName(cfg, setreq, modifier); //generate an output file name
  TFile HDump(HDumpName.c_str(), "RECREATE");
  if(HDump.IsZombie()){
    std::cout<<RED<<"Error: could not open ROOT file"<<RESET<<std::endl;
    return 0;
  }
  std::cout<<"Dumping histograms to "<<HDumpName<<"..."<<std::endl;

  int tnrun,tarun;
  bool conask = true;
  int concheck = 0;

  while(!setfile.eof()){
    setfile>>tnrun>>tarun;
    std::cout<<"Run "<<tnrun<<"..."<<std::endl;
    base_histos(DataFile, HDump, tnrun); //dump the histograms
    proc_histos(DataFile, HDump, tnrun); //dump the histograms
    calc_histos(DataFile, HDump, tnrun, barreq); //dump the histograms

    if(!conask) continue;
    std::cout<<"Continue? 0=no, 1=yes: ";std::cin>>concheck;
    if(concheck==0){
      break;
    }else if(concheck == 11){
      conask = false;
      concheck = 0;
    }
  }
  
  DataFile.Close();
  setfile.close();
  HDump.Close();
  std::cout<<GREEN<<"Histogram generation complete!"<<RESET<<std::endl;

  return 0;
}