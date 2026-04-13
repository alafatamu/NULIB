#include "io/baseroot.hpp"
#include "io/evthandler.hpp"
#include "analysis/eventutils.hpp"
#include "proggy/tformat.hpp"
#include "TTree.h"

#include <iostream>
#include <fstream>
#include <string>

using namespace eventdata;
using namespace tformat;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int Evt_to_ROOT(std::string inputfilename, std::string outputfilename){
  std::cout<<"Looking for "<<inputfilename<<std::endl;
  std::ifstream evtfile(inputfilename);
  if(!evtfile.is_open()){
    std::cout<<RED<<"Error: could not open evt file"<<RESET<<std::endl;
    return 0; //process failed somewhere
  }else std::cout<<GREEN<<"Found!"<<RESET<<std::endl;

  //make evthandler spit out events -- CURRENT WORK
  //need the evt file to be opened outside of the evthandler (new function for global evt use?)
  //need unpacker to return raw events (with a quality flag)
  //then we can save the raw event data based on their quality (worthy or not)

  return 1; //successful conversion and storage!
}