#include "io/baseroot.hpp"
#include "io/evthandler.hpp"
#include "events/eventutils.hpp"
#include "TTree.h"

using namespace eventdata;

TTree makeconversiontree(){
  //Conversion tree is for data conversion from any format to TNLIB event format
  processed_event goodevent;
  TTree* treeOUT = new TTree;
  treeOUT->Branch("goodevent", &goodevent);
  return;
}