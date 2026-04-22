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
  TDirectory* R_dir = HDump.GetDirectory("R_dir"); //check for the directory
  if(R_dir) HDump.rmdir("R_dir"); //delete the directory if it already exists
  R_dir = HDump.mkdir("R_dir"); //make a new directory
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

  delete hitcount_h;
  delete chip_h;
  delete chan_h;
  delete Aint_h;
  delete Bint_h;
  delete Cint_h;
  delete Tint_h;
  delete TDCchan_h;
  delete TDCval_h;

  std::cout<<"Done!"<<std::endl;

  return;
}