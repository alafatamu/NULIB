#include <iostream>
#include <vector>

#include "analysis/histos.hpp"
#include "io/treebiz.hpp"
#include "proggy/tformat.hpp"

#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TKey.h"

using namespace tformat;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void base_histos(TFile& DataFile, TFile& HDump, int runreq){ //upgrade idea: take a single tree, not a file
  //open a root file and make histograms based on the "raw" tree

  //First, let's build our base directories
  TDirectory* inputbase = &DataFile;
  TDirectory* outputbase = &HDump;
  if(runreq!=-10){
    inputbase = DataFile.GetDirectory(Form("run%d",runreq));
    outputbase = HDump.GetDirectory(Form("run%d",runreq));
    if(!outputbase) outputbase = HDump.mkdir(Form("run%d",runreq));
  }
  if(!inputbase){std::cout<<RED<<"Error: could not find data directory"<<RESET<<std::endl;return;}
  if(!outputbase){std::cout<<RED<<"Error: could not find output directory"<<RESET<<std::endl;return;}

  //Now we can get our data of interest
  TTree* RTree = nullptr;
  inputbase->GetObject("raw", RTree);
  if(RTree==nullptr){std::cout<<"Error: could not find raw tree in histos.root"<<std::endl;return;}

  //Finally, let's make the new directory in HDump
  TDirectory* R_Dir = outputbase->GetDirectory("RAW"); //check for the directory
  if(R_Dir) HDump.rmdir("RAW"); //delete the directory (and its contents) if it already exists
  R_Dir = outputbase->mkdir("RAW"); //make the new directory
  if(R_Dir==nullptr){std::cout<<RED<<"Error: could not make raw directory in HDump"<<RESET<<std::endl;return;}

  //Make the tree variables, then set the branch addresses
  treebiz::RData RData;
  RData.bindRead(*RTree); //bind the branches to the RData struct

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
      chip_h->Fill((*RData.r_chip)[j]);
      chan_h->Fill((*RData.r_chan)[j]);
      Aint_h->Fill((*RData.r_Aint)[j]);
      Bint_h->Fill((*RData.r_Bint)[j]);
      Cint_h->Fill((*RData.r_Cint)[j]);
      Tint_h->Fill((*RData.r_Tint)[j]);
    }
    for(int j=0; j<(*RData.r_TDCchan).size(); j++){
      TDCchan_h->Fill((*RData.r_TDCchan)[j]);
      TDCval_h->Fill((*RData.r_TDCval)[j]);
    }
  }

  R_Dir->cd();
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

void proc_histos(TFile& DataFile, TFile& HDump, int runreq){ 
  //open a root file and make histograms based on the "processed" tree
  
  //First, let's build our base directories
  TDirectory* inputbase = &DataFile;
  TDirectory* outputbase = &HDump;
  if(runreq!=-10){
    inputbase = DataFile.GetDirectory(Form("run%d",runreq));
    outputbase = HDump.GetDirectory(Form("run%d",runreq));
    if(!outputbase) outputbase = HDump.mkdir(Form("run%d",runreq));
  }
  if(!inputbase){std::cout<<RED<<"Error: could not find data directory"<<RESET<<std::endl;return;}
  if(!outputbase){std::cout<<RED<<"Error: could not find output directory"<<RESET<<std::endl;return;}

  //Now we can get our data of interest
  TTree* PTree = nullptr;
  inputbase->GetObject("processed", PTree);
  if(PTree==nullptr){std::cout<<"Error: could not find processed tree in histos.root"<<std::endl;return;}

  //Finally, let's make the new directories in HDump
  TDirectory* P_Dir = outputbase->GetDirectory("PROCESSED"); //check for the directory
  if(P_Dir) HDump.rmdir("PROCESSED"); //delete the directory (and its contents) if it already exists
  P_Dir = outputbase->mkdir("PROCESSED"); //make the new directory
  if(P_Dir==nullptr){std::cout<<RED<<"Error: could not make PROCESSED directory in HDump"<<RESET<<std::endl;return;}
  //And make the subdirectories
  TDirectory* P_DirTop = P_Dir->mkdir("top"); //make the top directory
  TDirectory* P_DirBot = P_Dir->mkdir("bottom"); //make the bottom directory
  TDirectory* P_DirOverview = P_Dir->mkdir("overview"); //make the overview directory
  if(P_Dir==nullptr||P_DirTop==nullptr||P_DirBot==nullptr||P_DirOverview==nullptr){
    std::cout<<RED<<"Error: could not make processed subdirectories in histos.root"<<RESET<<std::endl;
    return;
  }

  //Make the tree variables, then set the branch addresses
  treebiz::PData PData;
  PData.bindRead(*PTree); //bind the branches to the PData struct

  //Make the histograms
  TH1F* hitcount_h = new TH1F("hitcount", "hitcount", 5, 0, 5);

  TH1F* chip_h = new TH1F("chip", "chip", 16, 0, 16);
  TH1F* chan_h = new TH1F("chan", "chan", 8, 0, 8);
  TH1F* Aint_h = new TH1F("Aint", "Aint", 1750, 0, 3500);
  TH1F* Bint_h = new TH1F("Bint", "Bint", 1750, 0, 3500);
  TH1F* Cint_h = new TH1F("Cint", "Cint", 1750, 0, 3500);
  TH1F* Tint_h = new TH1F("Tint", "Tint", 2000, 0, 8000);
  TH1F* TDCchan_h = new TH1F("TDCchan", "TDCchan", 16, 0, 16);
  TH1F* TDCval_h = new TH1F("TDCval", "TDCval", 2500, 0, 5000);

  TH1F* chip_top_h = new TH1F("chip_top", "chip_top", 16, 0, 16);
  TH1F* chan_top_h = new TH1F("chan_top", "chan_top", 8, 0, 8);
  TH1F* Aint_top_h = new TH1F("Aint_top", "Aint_top", 1750, 0, 3500);
  TH1F* Bint_top_h = new TH1F("Bint_top", "Bint_top", 1750, 0, 3500);
  TH1F* Cint_top_h = new TH1F("Cint_top", "Cint_top", 1750, 0, 3500);
  TH1F* Tint_top_h = new TH1F("Tint_top", "Tint_top", 2000, 0, 8000);
  //TH1F* TDCchan_top_h = new TH1F("TDCchan_top", "TDCchan_top", 16, 0, 16);
  //TH1F* TDCval_top_h = new TH1F("TDCval_top", "TDCval_top", 2500, 0, 5000);

  TH1F* chip_bot_h = new TH1F("chip_bot", "chip_bot", 16, 0, 16);
  TH1F* chan_bot_h = new TH1F("chan_bot", "chan_bot", 8, 0, 8);
  TH1F* Aint_bot_h = new TH1F("Aint_bot", "Aint_bot", 1750, 0, 3500);
  TH1F* Bint_bot_h = new TH1F("Bint_bot", "Bint_bot", 1750, 0, 3500);
  TH1F* Cint_bot_h = new TH1F("Cint_bot", "Cint_bot", 1750, 0, 3500);
  TH1F* Tint_bot_h = new TH1F("Tint_bot", "Tint_bot", 2000, 0, 8000);
  //TH1F* TDCchan_bot_h = new TH1F("TDCchan_bot", "TDCchan_bot", 16, 0, 16);
  //TH1F* TDCval_bot_h = new TH1F("TDCval_bot", "TDCval_bot", 2500, 0, 5000);

  TH1F* bar_id_h = new TH1F("bar_id", "bar_id", 128, 0, 128);
  TH1F* coupledhits_h = new TH1F("coupledhits", "coupledhits", 5, 0, 5);
  TH1F* barmult_h = new TH1F("barmult", "barmult", 5, 0, 5);

  //some overview histograms
  TH2F* AvB_h = new TH2F("AvB", "Aint vs Bint", 1750, 0, 3500, 1750, 0, 3500);
  TH2F* AvBtop_h = new TH2F("AvBtop", "Aint_top vs Bint_top", 1750, 0, 3500, 1750, 0, 3500);
  TH2F* AvBbot_h = new TH2F("AvBbot", "Aint_bot vs Bint_bot", 1750, 0, 3500, 1750, 0, 3500);

  //loop through each event and fill the histograms
  for(int i=0; i<PTree->GetEntries(); i++){
    PTree->GetEntry(i);
    hitcount_h->Fill(PData.hitcount);
    for(int j=0; j<PData.hitcount; j++){
      bar_id_h->Fill((*PData.r_bar_id)[j]);
      chip_h->Fill((*PData.r_chip)[j]);
      chan_h->Fill((*PData.r_chan)[j]);
      Aint_h->Fill((*PData.r_Aint)[j]);
      Bint_h->Fill((*PData.r_Bint)[j]);
      Cint_h->Fill((*PData.r_Cint)[j]);
      Tint_h->Fill((*PData.r_Tint)[j]);

      AvB_h->Fill((*PData.r_Aint)[j], (*PData.r_Bint)[j]);
    }

    for(int j=0; j<PData.coupledhits; j++){
      chip_top_h->Fill((*PData.r_chip_top)[j]);
      chan_top_h->Fill((*PData.r_chan_top)[j]);
      Aint_top_h->Fill((*PData.r_Aint_top)[j]);
      Bint_top_h->Fill((*PData.r_Bint_top)[j]);
      Cint_top_h->Fill((*PData.r_Cint_top)[j]);
      Tint_top_h->Fill((*PData.r_Tint_top)[j]);

      chip_bot_h->Fill((*PData.r_chip_bot)[j]);
      chan_bot_h->Fill((*PData.r_chan_bot)[j]);
      Aint_bot_h->Fill((*PData.r_Aint_bot)[j]);
      Bint_bot_h->Fill((*PData.r_Bint_bot)[j]);
      Cint_bot_h->Fill((*PData.r_Cint_bot)[j]);
      Tint_bot_h->Fill((*PData.r_Tint_bot)[j]);

      AvBtop_h->Fill((*PData.r_Aint_top)[j], (*PData.r_Bint_top)[j]);
      AvBbot_h->Fill((*PData.r_Aint_bot)[j], (*PData.r_Bint_bot)[j]);
    }
    
    //std::cout<<"TDC data size: "<<(*PData.TDCchan).size()<<std::endl;
    for(int j=0; j<(*PData.r_TDCchan).size(); j++){
      TDCchan_h->Fill((*PData.r_TDCchan)[j]);
      TDCval_h->Fill((*PData.r_TDCval)[j]);
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

  P_Dir->cd();
  hitcount_h->Write();
  chip_h->Write();
  chan_h->Write();
  Aint_h->Write();
  Bint_h->Write();
  Cint_h->Write();
  Tint_h->Write();
  TDCchan_h->Write();
  TDCval_h->Write();
  bar_id_h->Write();
  coupledhits_h->Write();
  barmult_h->Write();

  P_DirTop->cd();
  chip_top_h->Write();
  chan_top_h->Write();
  Aint_top_h->Write();
  Bint_top_h->Write();
  Cint_top_h->Write();
  Tint_top_h->Write();
  //TDCchan_top_h->Write();
  //TDCval_top_h->Write();

  P_DirBot->cd();
  chip_bot_h->Write();
  chan_bot_h->Write();
  Aint_bot_h->Write();
  Bint_bot_h->Write();
  Cint_bot_h->Write();
  Tint_bot_h->Write();
  //TDCchan_bot_h->Write();
  //TDCval_bot_h->Write();

  P_DirOverview->cd();
  AvB_h->Write();

  std::cout<<"Processed histograms saved..."<<std::endl;

  return;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void calc_histos(TFile& DataFile, TFile& HDump, int runreq, int barreq){ 
  //open a root file and make histograms based on the "analysed" tree
  
  //First, let's build our base directories
  TDirectory* inputbase = &DataFile;
  TDirectory* outputbase = &HDump;
  if(runreq!=-10){
    inputbase = DataFile.GetDirectory(Form("run%d",runreq));
    outputbase = HDump.GetDirectory(Form("run%d",runreq));
    if(!outputbase) outputbase = HDump.mkdir(Form("run%d",runreq));
  }
  if(!inputbase){std::cout<<RED<<"Error: could not find data directory"<<RESET<<std::endl;return;}
  if(!outputbase){std::cout<<RED<<"Error: could not find output directory"<<RESET<<std::endl;return;}

  //Now we can get our data of interest
  TTree* ATree = nullptr;
  inputbase->GetObject("analysed", ATree);
  if(ATree==nullptr){std::cout<<"Error: could not find analysed tree in histos.root"<<std::endl;return;}

  //Finally, let's make the new directory in HDump
  TDirectory* A_Dir = outputbase->GetDirectory("ANALYSED"); //check for the directory
  if(A_Dir) HDump.rmdir("ANALYSED"); //delete the directory (and its contents) if it already exists
  A_Dir = outputbase->mkdir("ANALYSED"); //make the new directory
  if(A_Dir==nullptr){std::cout<<RED<<"Error: could not make ANALYSED directory in HDump"<<RESET<<std::endl;return;}

  //Make the tree variables, then set the branch addresses
  treebiz::AData AData;
  AData.bindRead(*ATree);

  //Make the histograms
  TH1F* CmAB = new TH1F("CmAB", "CmAB", 3000, -3000, 3000);
  TH1F* BmA = new TH1F("BmA", "BmA", 3000, -3000, 3000);

  TH1F* PSD = new TH1F("PSD", "PSD", 400, -2, 2);
  TH1F* PSDtop = new TH1F("PSDtop", "PSDtop", 400, -2, 2);
  TH1F* PSDbot = new TH1F("PSDbot", "PSDbot", 400, -2, 2);

  TH2F* PSDvEcalc = new TH2F("PSDvEcalc", "PSDvEcalc", 1000, 0, 10000, 100, 0, 1);
  TH2F* PSDvAB = new TH2F("PSDvAB", "PSDvAB", 1000, 0, 10000, 100, 0, 1);
  TH2F* PSDvC = new TH2F("PSDvC", "PSDvC", 1000, 0, 10000, 100, 0, 1);
  TH2F* PSDtopC = new TH2F("PSDtopvC", "PSDtopvC", 1000, 0, 10000, 100, 0, 1);
  TH2F* PSDbotC = new TH2F("PSDbotvC", "PSDbotvC", 1000, 0, 10000, 100, 0, 1);

  TH2F* PSDvT = new TH2F("PSDvT", "PSDvT", 1000, 0, 4000, 100, 0, 1);

  //loop through each event and fill the histograms
  for(int i=0; i<ATree->GetEntries(); i++){
    bool plot = false;
    ATree->GetEntry(i);
    for (int h=0;h<AData.coupledhits;h++){
      if((*AData.r_bar_id)[h]==barreq||barreq==1234) plot = true;
      if(!plot)continue;

      int ABval = (*AData.r_Aint_top)[h]+(*AData.r_Aint_bot)[h]+(*AData.r_Bint_top)[h]+(*AData.r_Bint_bot)[h];
      int BmAval = (*AData.r_Bint_top)[h]+(*AData.r_Bint_bot)[h]-(*AData.r_Aint_top)[h]-(*AData.r_Aint_bot)[h];
      int Cval = (*AData.r_Cint_top)[h]+(*AData.r_Cint_bot)[h];
      CmAB->Fill(Cval-ABval);
      BmA->Fill(BmAval);


      PSD->Fill((*AData.r_PSD)[h]);
      PSDtop->Fill((*AData.r_PSD_top)[h]);
      PSDbot->Fill((*AData.r_PSD_bot)[h]);

      double Ecalc = (double)(*AData.r_E_calc)[h];
      PSDvEcalc->Fill(Ecalc,(*AData.r_PSD)[h]);
      PSDvC->Fill(Cval,(*AData.r_PSD)[h]);
      PSDvAB->Fill(ABval,(*AData.r_PSD)[h]);
      PSDtopC->Fill(Cval,(*AData.r_PSD_top)[h]);
      PSDbotC->Fill(Cval,(*AData.r_PSD_bot)[h]);

      double Tavg = ((double)(*AData.r_Tint_top)[h]+(double)(*AData.r_Tint_bot)[h])/2.;
      PSDvT->Fill(Tavg,(*AData.r_PSD)[h]);
    }
  }

  A_Dir->cd();
  CmAB->Write();
  BmA->Write();
  PSD->Write();
  PSDtop->Write();
  PSDbot->Write();

  PSDvEcalc->Write();
  PSDvAB->Write();
  PSDvC->Write();
  PSDtopC->Write();
  PSDbotC->Write();

  PSDvT->Write();
  
  std::cout<<"Analysed histograms saved..."<<std::endl;
  return;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void set_histos(TFile& DataFile, TFile& HDump, int barreq){
  TString out_dirname = Form("bar%d",barreq);
  if(barreq==1234) out_dirname = "all";

  //make an output directory for the selected bar
  TDirectory* B_Dir = HDump.GetDirectory(out_dirname); //check for the directory
  if(B_Dir) HDump.rmdir(out_dirname); //delete the directory (and its contents) if it already exists
  B_Dir = HDump.mkdir(out_dirname); //make the new directory
  if(B_Dir==nullptr){std::cout<<RED<<"Error: could not make BAR directory in HDump"<<RESET<<std::endl;return;}

  //Make the histograms before starting any loops
  TH1F* PSD = new TH1F("PSD", "PSD", 400, -2, 2);
  TH2F* PSDvC = new TH2F("PSDvC", "PSDvC", 1000, 0, 10000, 100, 0, 1);
  TH2F* PSDvAB = new TH2F("PSDvAB", "PSDvAB", 1000, 0, 10000, 100, 0, 1);
  TH2F* PSDvE = new TH2F("PSDvE", "PSDvE", 1000, 0, 10000, 100, 0, 1);

  //loop through all top-level objects in the data file
  TIter next(DataFile.GetListOfKeys());
  TKey* key = nullptr;

  while((key=(TKey*)next())){
    TObject* obj = key->ReadObj();
    if(obj==nullptr) continue;
    if (!obj->InheritsFrom(TDirectory::Class())) {
      delete obj;
      continue;
    } 
    TDirectory* inputbase = (TDirectory*)obj; //now we can designate our object as the input directory
    //this SHOULD be titled "runxxxx" and SHOULD contain 3 trees: raw, processed, analysed
    TString in_dirname = inputbase->GetName();
    if(!in_dirname.BeginsWith("run")){
      delete obj;
      continue; //skip this directory if it doesn't match the naming convention
    }

    //Now we can grab the analysis tree
    TTree* ATree = nullptr;
    inputbase->GetObject("analysed", ATree);
    if(ATree==nullptr){
      std::cout<<YELLOW<<"Warning: could not find analysed tree in "
               <<in_dirname<<RESET<<std::endl;
      delete obj;
      continue;
    }

    //prepare the branches and variables for reading the tree
    treebiz::AData AData;
    AData.bindRead(*ATree); //bind the branches to the AData struct

    //loop through each event and fill the histograms
    std::cout<<"Reading "<<ATree->GetEntries()<<" events from "<<in_dirname<<std::flush;
    int plottedhits = 0;
    for(int i=0; i<ATree->GetEntries(); i++){
      ATree->GetEntry(i);
      for (int h=0;h<AData.coupledhits;h++){

        if((*AData.r_bar_id)[h]!=barreq && barreq!=1234) continue;
        plottedhits++;
        PSD->Fill((*AData.r_PSD)[h]);
        double EfromC = (double)(*AData.r_Cint_top)[h]+(double)(*AData.r_Cint_bot)[h];
        double EfromAB = (double)(*AData.r_Aint_top)[h]+(double)(*AData.r_Aint_bot)[h]+(double)(*AData.r_Bint_top)[h]+(double)(*AData.r_Bint_bot)[h];
        double EfromA = (double)(*AData.r_Aint_top)[h]+(double)(*AData.r_Aint_bot)[h];
        double Qtop = (double)(*AData.r_Aint_top)[h]+(double)(*AData.r_Bint_top)[h];
        double Qbot = (double)(*AData.r_Aint_bot)[h]+(double)(*AData.r_Bint_bot)[h];
        double Qcalc = 0.5*(Qtop+Qbot);
        PSDvC->Fill(EfromC,(*AData.r_PSD)[h]);
        PSDvAB->Fill(EfromAB,(*AData.r_PSD)[h]);
        PSDvE->Fill(EfromA,(*AData.r_PSD)[h]);
      }
    }
    std::cout<<" ---> "<<plottedhits<<" hits plotted."<<std::endl;
    
    //done with the directory? Delete it.
    delete obj;

  }

  //now that everything has been looped through, let's write the histograms to the output directory
  B_Dir->cd();
  PSD->Write();
  PSDvC->Write();
  PSDvAB->Write();
  PSDvE->Write();

  std::cout<<"Histograms saved ";
  if(barreq==1234){
    std::cout<<"for all bars."<<std::endl;
  }else{
    std::cout<<"for bar "<<barreq<<"."<<std::endl;
  }

  return;
}