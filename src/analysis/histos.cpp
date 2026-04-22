#include <iostream>
#include <vector>

#include "analysis/histos.hpp"
#include "io/treebiz.hpp"
#include "proggy/tformat.hpp"

#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TH1F.h"

using namespace tformat;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void base_histos(std::string DataFileName, std::string HDumpName){ //open a root file and make histograms based on the "raw" tree
  TFile DataFile(DataFileName.c_str(), "READ");
  if(DataFile.IsZombie()){
    std::cout<<"Error: could not open "<<DataFileName<<std::endl;
    return;
  }

  TFile HDump(HDumpName.c_str(),"UPDATE");
  if(HDump.IsZombie()){
    std::cout<<"Error: could not open histos.root"<<std::endl;
    return;
  }

  TTree* RTree = nullptr;
  DataFile.GetObject("raw", RTree);
  if(RTree==nullptr){
    std::cout<<"Error: could not find raw tree in histos.root"<<std::endl;
    return;
  }

  //Now that we have our data, let's make a new directory in HDump
  TDirectory* R_dir = HDump.GetDirectory("RAW"); //check for the directory
  if(R_dir) HDump.rmdir("RAW"); //delete the directory if it already exists
  R_dir = HDump.mkdir("RAW"); //make a new directory
  if(R_dir==nullptr){
    std::cout<<RED<<"Error: could not make RData directory in histos.root"<<RESET<<std::endl;
    return;
  }

  //Make the tree variables, then set the branch addresses
  treebiz::RTreeReadData RData;
  treebiz::set_RTreeBranches(*RTree, RData);

  //Make the histograms
  TH1F* hitcount_h = new TH1F("hitcount", "hitcount", 5, 0, 5);
  TH1F* chip_h = new TH1F("chip", "chip", 16, 0, 16);
  TH1F* chan_h = new TH1F("chan", "chan", 8, 0, 8);
  TH1F* Aint_h = new TH1F("Aint", "Aint", 1250, 0, 3500);
  TH1F* Bint_h = new TH1F("Bint", "Bint", 1250, 0, 3500);
  TH1F* Cint_h = new TH1F("Cint", "Cint", 1250, 0, 3500);
  TH1F* Tint_h = new TH1F("Tint", "Tint", 2000, 0, 8000);
  TH1F* TDCchan_h = new TH1F("TDCchan", "TDCchan", 16, 0, 16);
  TH1F* TDCval_h = new TH1F("TDCval", "TDCval", 2500, 0, 5000);

  //loop through each event and fill the histograms
  for(int i=0; i<RTree->GetEntries(); i++){
    RTree->GetEntry(i);
    hitcount_h->Fill(RData.hitcount);
    for(int j=0; j<RData.hitcount; j++){
      chip_h->Fill((*RData.chip)[j]);
      chan_h->Fill((*RData.chan)[j]);
      Aint_h->Fill((*RData.Aint)[j]);
      Bint_h->Fill((*RData.Bint)[j]);
      Cint_h->Fill((*RData.Cint)[j]);
      Tint_h->Fill((*RData.Tint)[j]);
    }
    for(int j=0; j<(*RData.TDCchan).size(); j++){
      TDCchan_h->Fill((*RData.TDCchan)[j]);
      TDCval_h->Fill((*RData.TDCval)[j]);
    }
  }

  R_dir->cd();
  hitcount_h->Write();
  chip_h->Write();
  chan_h->Write();
  Aint_h->Write();
  Bint_h->Write();
  Cint_h->Write();
  Tint_h->Write();
  TDCchan_h->Write();
  TDCval_h->Write();

  std::cout<<"Raw histograms saved..."<<std::endl;

  return;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void proc_histos(std::string DataFileName, std::string HDumpName){ //open a root file and make histograms based on the "raw" tree
  TFile DataFile(DataFileName.c_str(), "READ");
  if(DataFile.IsZombie()){
    std::cout<<"Error: could not open "<<DataFileName<<std::endl;
    return;
  }

  TFile HDump(HDumpName.c_str(),"UPDATE");
  if(HDump.IsZombie()){
    std::cout<<"Error: could not open histos.root"<<std::endl;
    return;
  }

  TTree* PTree = nullptr;
  DataFile.GetObject("processed", PTree);
  if(PTree==nullptr){
    std::cout<<"Error: could not find processed tree in histos.root"<<std::endl;
    return;
  }

  //Now that we have our data, let's make a new directory in HDump
  TDirectory* P_dir = HDump.GetDirectory("PROCESSED"); //check for the directories
  if(P_dir) HDump.rmdir("PROCESSED");  //delete the directory if it already exists
  P_dir = HDump.mkdir("PROCESSED"); //make new directories
  TDirectory* P_dirtop = HDump.mkdir("PROCESSED/top", "top");
  TDirectory* P_dirbot = HDump.mkdir("PROCESSED/bottom", "bottom");
  if(P_dir==nullptr||P_dirtop==nullptr||P_dirbot==nullptr){
    std::cout<<RED<<"Error: could not make Processed directory in histos.root"<<RESET<<std::endl;
    return;
  }

  //Make the tree variables, then set the branch addresses
  treebiz::PTreeReadData PData;
  treebiz::set_PTreeBranches(*PTree, PData);

  //Make the histograms
  TH1F* hitcount_h = new TH1F("hitcount", "hitcount", 5, 0, 5);

  TH1F* chip_h = new TH1F("chip", "chip", 16, 0, 16);
  TH1F* chan_h = new TH1F("chan", "chan", 8, 0, 8);
  TH1F* Aint_h = new TH1F("Aint", "Aint", 1250, 0, 3500);
  TH1F* Bint_h = new TH1F("Bint", "Bint", 1250, 0, 3500);
  TH1F* Cint_h = new TH1F("Cint", "Cint", 1250, 0, 3500);
  TH1F* Tint_h = new TH1F("Tint", "Tint", 2000, 0, 8000);
  TH1F* TDCchan_h = new TH1F("TDCchan", "TDCchan", 16, 0, 16);
  TH1F* TDCval_h = new TH1F("TDCval", "TDCval", 2500, 0, 5000);

  TH1F* chip_top_h = new TH1F("chip_top", "chip_top", 16, 0, 16);
  TH1F* chan_top_h = new TH1F("chan_top", "chan_top", 8, 0, 8);
  TH1F* Aint_top_h = new TH1F("Aint_top", "Aint_top", 1250, 0, 3500);
  TH1F* Bint_top_h = new TH1F("Bint_top", "Bint_top", 1250, 0, 3500);
  TH1F* Cint_top_h = new TH1F("Cint_top", "Cint_top", 1250, 0, 3500);
  TH1F* Tint_top_h = new TH1F("Tint_top", "Tint_top", 2000, 0, 8000);
  //TH1F* TDCchan_top_h = new TH1F("TDCchan_top", "TDCchan_top", 16, 0, 16);
  //TH1F* TDCval_top_h = new TH1F("TDCval_top", "TDCval_top", 2500, 0, 5000);

  TH1F* chip_bot_h = new TH1F("chip_bot", "chip_bot", 16, 0, 16);
  TH1F* chan_bot_h = new TH1F("chan_bot", "chan_bot", 8, 0, 8);
  TH1F* Aint_bot_h = new TH1F("Aint_bot", "Aint_bot", 1250, 0, 3500);
  TH1F* Bint_bot_h = new TH1F("Bint_bot", "Bint_bot", 1250, 0, 3500);
  TH1F* Cint_bot_h = new TH1F("Cint_bot", "Cint_bot", 1250, 0, 3500);
  TH1F* Tint_bot_h = new TH1F("Tint_bot", "Tint_bot", 2000, 0, 8000);
  //TH1F* TDCchan_bot_h = new TH1F("TDCchan_bot", "TDCchan_bot", 16, 0, 16);
  //TH1F* TDCval_bot_h = new TH1F("TDCval_bot", "TDCval_bot", 2500, 0, 5000);

  TH1F* barshit_h = new TH1F("barshit", "barshit", 128, 0, 128);
  TH1F* coupledhits_h = new TH1F("coupledhits", "coupledhits", 5, 0, 5);
  TH1F* barmult_h = new TH1F("barmult", "barmult", 5, 0, 5);

  //loop through each event and fill the histograms
  for(int i=0; i<PTree->GetEntries(); i++){
    PTree->GetEntry(i);
    hitcount_h->Fill(PData.hitcount);
    for(int j=0; j<PData.hitcount; j++){
      barshit_h->Fill((*PData.barshit)[j]);
      chip_h->Fill((*PData.chip)[j]);
      chan_h->Fill((*PData.chan)[j]);
      Aint_h->Fill((*PData.Aint)[j]);
      Bint_h->Fill((*PData.Bint)[j]);
      Cint_h->Fill((*PData.Cint)[j]);
      Tint_h->Fill((*PData.Tint)[j]);
    }

    for(int j=0; j<PData.coupledhits; j++){
      chip_top_h->Fill((*PData.chip_top)[j]);
      chan_top_h->Fill((*PData.chan_top)[j]);
      Aint_top_h->Fill((*PData.Aint_top)[j]);
      Bint_top_h->Fill((*PData.Bint_top)[j]);
      Cint_top_h->Fill((*PData.Cint_top)[j]);
      Tint_top_h->Fill((*PData.Tint_top)[j]);

      chip_bot_h->Fill((*PData.chip_bot)[j]);
      chan_bot_h->Fill((*PData.chan_bot)[j]);
      Aint_bot_h->Fill((*PData.Aint_bot)[j]);
      Bint_bot_h->Fill((*PData.Bint_bot)[j]);
      Cint_bot_h->Fill((*PData.Cint_bot)[j]);
      Tint_bot_h->Fill((*PData.Tint_bot)[j]);
    }
    
    //std::cout<<"TDC data size: "<<(*PData.TDCchan).size()<<std::endl;
    for(int j=0; j<(*PData.TDCchan).size(); j++){
      TDCchan_h->Fill((*PData.TDCchan)[j]);
      TDCval_h->Fill((*PData.TDCval)[j]);
    }
    /*
    std::cout<<"TDC top chan size: "<<(*PData.TDCchan_top).size()<<std::endl;
    std::cout<<"TDC top val size: "<<(*PData.TDCval_top).size()<<std::endl;
    for(int j=0; j<(*PData.TDCchan_top).size(); j++){
      TDCchan_top_h->Fill((*PData.TDCchan_top)[j]);
      TDCval_top_h->Fill((*PData.TDCval_top)[j]);
    }
    std::cout<<"TDC bot chan size: "<<(*PData.TDCchan_bot).size()<<std::endl;
    std::cout<<"TDC bot val size: "<<(*PData.TDCval_top).size()<<std::endl;
    for(int j=0; j<(*PData.TDCchan_bot).size(); j++){
      TDCchan_bot_h->Fill((*PData.TDCchan_bot)[j]);
      TDCval_bot_h->Fill((*PData.TDCval_bot)[j]);
    }*/

    coupledhits_h->Fill(PData.coupledhits);
    barmult_h->Fill(PData.barmult);
  }

  P_dir->cd();
  hitcount_h->Write();
  chip_h->Write();
  chan_h->Write();
  Aint_h->Write();
  Bint_h->Write();
  Cint_h->Write();
  Tint_h->Write();
  TDCchan_h->Write();
  TDCval_h->Write();
  barshit_h->Write();
  coupledhits_h->Write();
  barmult_h->Write();

  P_dirtop->cd();
  chip_top_h->Write();
  chan_top_h->Write();
  Aint_top_h->Write();
  Bint_top_h->Write();
  Cint_top_h->Write();
  Tint_top_h->Write();
  //TDCchan_top_h->Write();
  //TDCval_top_h->Write();

  P_dirbot->cd();
  chip_bot_h->Write();
  chan_bot_h->Write();
  Aint_bot_h->Write();
  Bint_bot_h->Write();
  Cint_bot_h->Write();
  Tint_bot_h->Write();
  //TDCchan_bot_h->Write();
  //TDCval_bot_h->Write();

  std::cout<<"Processed histograms saved..."<<std::endl;

  return;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......