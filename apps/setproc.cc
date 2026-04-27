#include <iostream>
#include "io/evthandler.hpp"
#include "io/dataref.hpp"
#include "proggy/tformat.hpp"
#include "io/baseroot.hpp"
#include "analysis/detector.hpp"
#include "analysis/histos.hpp"

using namespace tformat;

int main(){
  int setreq = 0;
  int runnum = 0;
  INFOCON cfg = load_config("../info/config.cfg"); //read in the config from the given path
  detector texneut; 
  bool goodmapread = texneut.fill_maps(cfg); //load the detector object
  if(!goodmapread){
    std::cout<<RED<<"Detector map read failed."<<RESET<<std::endl;
    return 0;
  }

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

  std::string OutputSETFileName = namebuilder::get_OutputSETName(cfg, setreq); //generate an output file name
  TFile OutputSETFile(OutputSETFileName.c_str(), "RECREATE");
  if(OutputSETFile.IsZombie()){
    std::cout<<RED<<"Error: could not open ROOT file"<<RESET<<std::endl;
    return 0;
  }

  int tnrun,tarun;
  bool conask = true;
  int concheck = 0;

  while(!setfile.eof()){
    setfile>>tnrun>>tarun;
    std::string InputEvtName = namebuilder::get_InputEvtName(cfg, tnrun); //generate an input file name
    std::ifstream InputEvtFile = grabfile(InputEvtName); //open the input file
    if(!InputEvtFile) return 0; //no file loaded

    int result = Evt_to_ROOT(InputEvtFile, OutputSETFile, texneut, tnrun); //read through the file
    if(result==0){
      std::cout<<RED<<"Run "<<tnrun<<" read failed."<<RESET<<std::endl;
    }else if(result==1){
      std::cout<<GREEN<<"Run "<<tnrun<<" successfully read."<<RESET<<std::endl;
    }
    std::cout<<"Done!"<<std::endl;
    InputEvtFile.close();
    
    if(!conask) continue;
    std::cout<<"Continue? 0=no, 1=yes: ";std::cin>>concheck;
    if(concheck==0){
      break;
    }else if(concheck == 11){
      conask = false;
      concheck = 0;
    }
  }

  setfile.close();
  OutputSETFile.Close();
  std::cout<<GREEN<<"Set conversion complete!"<<RESET<<std::endl;
  
  return 0;
}