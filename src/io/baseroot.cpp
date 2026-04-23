#include "io/baseroot.hpp"
#include "io/evthandler.hpp"
#include "analysis/eventutils.hpp"
#include "proggy/tformat.hpp"
#include "io/treebiz.hpp"
#include "TTree.h"
#include "TFile.h"

#include <iostream>
#include <fstream>
#include <string>

using namespace eventutils;
using namespace tformat;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int Evt_to_ROOT(std::ifstream& InputEvtFile, std::string outputfilename, detector texneut){
  TFile ROOTOutputFile(outputfilename.c_str(), "RECREATE");

  TTree RTree("raw", "Raw event data");
  treebiz::RTreeData RData;
  treebiz::init_RTree(RTree, RData);

  TTree PTree("processed", "Processed event data");
  treebiz::PTreeData PData;
  treebiz::init_PTree(PTree, PData);

  TTree ATree("analysed", "Analysed event data");
  treebiz::ATreeData AData;
  treebiz::init_ATree(ATree, AData);

  bool stillreading = true;
  int eventnumber = 0;
  int goodcount = 0;

  while(stillreading){
    eventnumber++;
    std::cout<<"                                                              \r";
    std::cout<<YELLOW<<"Reading... Currently at event " << eventnumber << RESET;
    std::flush(std::cout);
    raw_event rawevent = grab_event(InputEvtFile);
    if(rawevent.unpackflag==1){
      treebiz::fill_RTreeData(RData, rawevent);
      RTree.Fill();
      processed_event processedevent = eventutils::process_event(rawevent, texneut);
      if(processedevent.keep){
        goodcount++;
        treebiz::fill_PTreeData(PData, processedevent);
        PTree.Fill();
        analysed_event analysedevent = eventutils::analyse_event(processedevent);
        treebiz::fill_ATreeData(AData, analysedevent);
        ATree.Fill();
      }else continue;
    }else if(rawevent.unpackflag==2||rawevent.unpackflag==3) stillreading = false;
    if(!stillreading)break;
  }

  std::cout<<std::endl;
  std::cout<<"Done!"<<std::endl;
  std::cout<<"Total 'events': "<<eventnumber<<std::endl;
  std::cout<<"Total events saved: "<<goodcount<<std::endl;

  std::cout<<"Saving root trees to file..."<<std::endl;
  RTree.Write();
  PTree.Write();
  ATree.Write();
  ROOTOutputFile.Close();

  std::cout<<"Done!"<<std::endl;

  return 1; //successful conversion and storage!
}