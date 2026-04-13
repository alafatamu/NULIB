#include <iostream>
#include "io/evthandler.hpp"
#include "io/dataref.hpp"
#include "proggy/tformat.hpp"

using namespace tformat;

int main(){
  int runreq = 0;
  Config cfg = load_config("../info/config.cfg"); //read in the config from the given path
  std::cout<<"Run to convert (0 to exit): "<<std::endl; std::cin>>runreq;
  if(runreq==0) return 0; //exit code

  std::string InputEvtName = namebuilder::get_InputEvtName(cfg, runreq); //generate an input file name
  std::ifstream InputEvtFile = grabfile(InputEvtName); //open the input file
  if(!InputEvtFile) return 0; //no file loaded
  
  int result = convert_evt_file(InputEvtFile); //read through the file
  if(result==0){
    std::cout<<RED<<"Run "<<runreq<<" read failed."<<RESET<<std::endl;
  }else if(result==1){
    std::cout<<GREEN<<"Run "<<runreq<<" successfully read."<<RESET<<std::endl;
  }
  return 0;
}