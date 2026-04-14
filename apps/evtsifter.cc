#include <iostream>
#include <fstream>
#include "io/evthandler.hpp"
#include "io/dataref.hpp"

int main(){
  int runreq = 0;
  INFOCON cfg = load_config("../info/config.cfg"); //read in the config from the given path
  std::cout<<"Run to convert (0 to exit): "<<std::endl; std::cin>>runreq;
  if(runreq==0) return 0; //exit code

  std::string InputEvtName = namebuilder::get_InputEvtName(cfg, runreq); //generate an input file name
  std::ifstream evtfile = grabfile(InputEvtName); //open the input file
  if(!evtfile) return 0; //no file loaded
  
  int result = inspect_evt_file(evtfile);
  if(result==0){
    std::cout<<"Successful read!"<<std::endl;
  }else{
    std::cout<<"Incomplete read..."<<std::endl;
  } 
  evtfile.close();
  return 0;
}