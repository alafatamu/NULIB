#include "io/baseroot.hpp"
#include "io/evthandler.hpp"
#include "analysis/eventutils.hpp"
#include "proggy/tformat.hpp"
#include "io/treebiz.hpp"
#include "TTree.h"
#include "TFile.h"
#include "TString.h"

#include <iostream>
#include <fstream>
#include <string>

using namespace eventutils;
using namespace tformat;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int Evt_to_ROOT(std::ifstream& InputEvtFile, TFile& ROOTOutputFile, detector& texneut, int runid){
  TDirectory* run_dir = nullptr;
  if(runid!=-10){ //-10 is the default 
                  //ideally, the user will include the runID in the file name if they don't specify the run in the function call
    TString run_dir_name = Form("run%d",runid);
    run_dir = ROOTOutputFile.GetDirectory(run_dir_name); //check for the directory
    if(run_dir) ROOTOutputFile.rmdir(run_dir_name);  //delete the directory if it already exists
    run_dir = ROOTOutputFile.mkdir(run_dir_name); //make new directory
    run_dir->cd();//cd into the new directory
    if(run_dir==nullptr){
      std::cout<<RED<<"Error: could not make Analysed directory in histos.root"<<RESET<<std::endl;
      return 0; //unsuccessful conversion
    }
  }

  //now all of these trees should be in the correct directory regardless of the runid
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

  std::cout<<"Done!"<<std::endl;

  return 1; //successful conversion and storage!
}