#include <iostream>
#include "io/evthandler.hpp"
#include "io/dataref.hpp"
#include "proggy/tformat.hpp"
#include "io/baseroot.hpp"
#include "analysis/detector.hpp"

using namespace tformat;

int main(){
  int runreq = 0;
  INFOCON cfg = load_config("../info/config.cfg"); //read in the config from the given path
  detector texneut; 
  bool goodmapread = texneut.fill_maps(cfg); //load the detector object
  if(!goodmapread){
    std::cout<<RED<<"Detector map read failed."<<RESET<<std::endl;
    return 0;
  }
  std::cout<<"Run to convert (0 to exit): "<<std::endl; std::cin>>runreq;
  if(runreq==0) return 0; //exit code

  std::string InputEvtName = namebuilder::get_InputEvtName(cfg, runreq); //generate an input file name
  std::ifstream InputEvtFile = grabfile(InputEvtName); //open the input file
  if(!InputEvtFile) return 0; //no file loaded

  std::string OutputROOTFileName = namebuilder::get_OutputROOTName(cfg, runreq); //generate an output file name

  int result = Evt_to_ROOT(InputEvtFile, OutputROOTFileName, texneut); //read through the file
  if(result==0){
    std::cout<<RED<<"Run "<<runreq<<" read failed."<<RESET<<std::endl;
  }else if(result==1){
    std::cout<<GREEN<<"Run "<<runreq<<" successfully read."<<RESET<<std::endl;
  }

  InputEvtFile.close();
  return 0;
}