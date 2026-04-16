#include "io/baseroot.hpp"
#include "io/evthandler.hpp"
#include "analysis/eventutils.hpp"
#include "analysis/analysis.hpp"
#include "proggy/tformat.hpp"
#include "TTree.h"
#include "TFile.h"

#include <iostream>
#include <fstream>
#include <string>

using namespace eventdata;
using namespace tformat;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

namespace treebiz{
  //TREE DATA STRUCTS
  struct RTreeData{ //raw data tree
    long long int timestamp = 0;
    int hitcount = 0;
    std::vector<int> chip;
    std::vector<int> chan;
    std::vector<int> Aint;
    std::vector<int> Bint;
    std::vector<int> Cint;
    std::vector<int> Tint;
    std::vector<int> TDCchan;
    std::vector<int> TDCval;
  }; 

  struct PTreeData{ //processed data tree
    long long int timestamp = 0;
    int hitcount = 0;
    std::vector<int> chip;
    std::vector<int> chan;
    std::vector<int> Aint;
    std::vector<int> Bint;
    std::vector<int> Cint;
    std::vector<int> Tint;
    std::vector<int> TDCchan;
    std::vector<int> TDCval;

    std::vector<int> chip_top,chip_bot;
    std::vector<int> chan_top,chan_bot;
    std::vector<int> Aint_top,Aint_bot;
    std::vector<int> Bint_top,Bint_bot;
    std::vector<int> Cint_top,Cint_bot;
    std::vector<int> Tint_top,Tint_bot;
    std::vector<int> TDCchan_top,TDCchan_bot;
    std::vector<int> TDCval_top,TDCval_bot;
    std::vector<int> barshit;
    int coupledhits = 0;
    int barmult = 0;
  };

  struct ATreeData{ //analysis data tree
    long long int timestamp = 0;
    int coupledhits = 0;
    std::vector<double> E_top, E_bot; //energy in top, bottom PMT
    std::vector<double> E_tot; //Total energy (top+bottom PMT)
    std::vector<double> PSD_top, PSD_bot, PSD;
    std::vector<int> PSDflag;
    std::vector<double> xhit, yhit, zhit; //hit location 
    std::vector<double> rho, theta, phi;
  };

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  //TREE BRANCH INITIALIZATION
  void init_RTree(TTree& tree, treebiz::RTreeData& RData){
    tree.Branch("timestamp", &RData.timestamp);
    tree.Branch("hitcount", &RData.hitcount);
    tree.Branch("chip", &RData.chip);
    tree.Branch("chan", &RData.chan);
    tree.Branch("Aint", &RData.Aint);
    tree.Branch("Bint", &RData.Bint);
    tree.Branch("Cint", &RData.Cint);
    tree.Branch("Tint", &RData.Tint);
    tree.Branch("TDCchan", &RData.TDCchan);
    tree.Branch("TDCval", &RData.TDCval);
    return;
  }

  void init_PTree(TTree& tree, treebiz::PTreeData& PData){
    tree.Branch("timestamp", &PData.timestamp);
    tree.Branch("hitcount", &PData.hitcount);
    tree.Branch("chip", &PData.chip);
    tree.Branch("chan", &PData.chan);
    tree.Branch("Aint", &PData.Aint);
    tree.Branch("Bint", &PData.Bint);
    tree.Branch("Cint", &PData.Cint);
    tree.Branch("Tint", &PData.Tint);
    tree.Branch("TDCchan", &PData.TDCchan);
    tree.Branch("TDCval", &PData.TDCval);

    tree.Branch("chip_top", &PData.chip_top);
    tree.Branch("chan_top", &PData.chan_top);
    tree.Branch("Aint_top", &PData.Aint_top);
    tree.Branch("Bint_top", &PData.Bint_top);
    tree.Branch("Cint_top", &PData.Cint_top);
    tree.Branch("Tint_top", &PData.Tint_top);
    tree.Branch("TDCchan_top", &PData.TDCchan_top);
    tree.Branch("TDCval_top", &PData.TDCval_top);

    tree.Branch("chip_bot", &PData.chip_bot);
    tree.Branch("chan_bot", &PData.chan_bot);
    tree.Branch("Aint_bot", &PData.Aint_bot);
    tree.Branch("Bint_bot", &PData.Bint_bot);
    tree.Branch("Cint_bot", &PData.Cint_bot);
    tree.Branch("Tint_bot", &PData.Tint_bot);
    tree.Branch("TDCchan_bot", &PData.TDCchan_bot);
    tree.Branch("TDCval_bot", &PData.TDCval_bot);

    tree.Branch("barshit", &PData.barshit);
    tree.Branch("coupledhits", &PData.coupledhits);
    tree.Branch("barmult", &PData.barmult);
    return;
  }

  void init_ATree(TTree& tree, treebiz::ATreeData& AData){
    tree.Branch("timestamp", &AData.timestamp);
    tree.Branch("coupledhits", &AData.coupledhits);
    tree.Branch("E_top", &AData.E_top);
    tree.Branch("E_bot", &AData.E_bot);
    tree.Branch("E_tot", &AData.E_tot);
    tree.Branch("PSD_top", &AData.PSD_top);
    tree.Branch("PSD_bot", &AData.PSD_bot);
    tree.Branch("PSD", &AData.PSD);
    //tree.Branch("PSDflag", &AData.PSDflag);
    //tree.Branch("xhit", &AData.xhit);
    //tree.Branch("yhit", &AData.yhit);
    //tree.Branch("zhit", &AData.zhit);
    //tree.Branch("rho", &AData.rho);
    //tree.Branch("theta", &AData.theta);
    //tree.Branch("phi", &AData.phi);
    return;
  }

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  //TREE FILLING
  void fill_RTreeData(treebiz::RTreeData& RData, eventdata::raw_event& rawevent){
    RData.timestamp = rawevent.timestamp;
    RData.hitcount = rawevent.hitcount;
    RData.chip = rawevent.chip;
    RData.chan = rawevent.chan;
    RData.Aint = rawevent.Aint;
    RData.Bint = rawevent.Bint;
    RData.Cint = rawevent.Cint;
    RData.Tint = rawevent.Tint;
    RData.TDCchan = rawevent.TDCchan;
    RData.TDCval = rawevent.TDCval;
    return;
  }

  void fill_PTreeData(treebiz::PTreeData& PData, eventdata::processed_event& processedevent){
    PData.timestamp = processedevent.timestamp;
    PData.hitcount = processedevent.hitcount;
    PData.chip = processedevent.chip;
    PData.chan = processedevent.chan;
    PData.Aint = processedevent.Aint;
    PData.Bint = processedevent.Bint;
    PData.Cint = processedevent.Cint;
    PData.Tint = processedevent.Tint;
    PData.TDCchan = processedevent.TDCchan;
    PData.TDCval = processedevent.TDCval;

    PData.chip_top = processedevent.chip_top;
    PData.chan_top = processedevent.chan_top;
    PData.Aint_top = processedevent.Aint_top;
    PData.Bint_top = processedevent.Bint_top;
    PData.Cint_top = processedevent.Cint_top;
    PData.Tint_top = processedevent.Tint_top;
    PData.TDCchan_top = processedevent.TDCchan_top;
    PData.TDCval_top = processedevent.TDCval_top;

    PData.chip_bot = processedevent.chip_bot;
    PData.chan_bot = processedevent.chan_bot;
    PData.Aint_bot = processedevent.Aint_bot;
    PData.Bint_bot = processedevent.Bint_bot;
    PData.Cint_bot = processedevent.Cint_bot;
    PData.Tint_bot = processedevent.Tint_bot;
    PData.TDCchan_bot = processedevent.TDCchan_bot;
    PData.TDCval_bot = processedevent.TDCval_bot;

    PData.barshit = processedevent.barshit;
    PData.coupledhits = processedevent.coupledhits;
    PData.barmult = processedevent.barmult;
    return;
  }

  void fill_ATreeData(treebiz::ATreeData& AData, analysis_feed& AFeed){
    AData.timestamp = AFeed.timestamp;
    AData.coupledhits = AFeed.coupledhits;
    AData.E_top = AFeed.E_top;
    AData.E_bot = AFeed.E_bot;
    AData.E_tot = AFeed.E_tot;
    AData.PSD_top = AFeed.PSD_top;
    AData.PSD_bot = AFeed.PSD_bot;
    AData.PSD = AFeed.PSD;
    //AData.PSDflag = AFeed.PSDflag;
    //AData.xhit = AFeed.xhit;
    //AData.yhit = AFeed.yhit;
    //AData.zhit = AFeed.zhit;
    //AData.rho = AFeed.rho;
    //AData.theta = AFeed.theta;
    //AData.phi = AFeed.phi;
    return;
  }
}

int Evt_to_ROOT(std::ifstream& InputEvtFile, std::string outputfilename, detector texneut){
  TFile ROOTOutputFile(outputfilename.c_str(), "RECREATE");

  TTree RTree("raw", "Raw event data");
  treebiz::RTreeData RData;
  treebiz::init_RTree(RTree, RData);

  TTree PTree("processed", "Processed event data");
  treebiz::PTreeData PData;
  treebiz::init_PTree(PTree, PData);

  TTree ATree("analysis", "Analysis event data");
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
      processed_event processedevent = eventdata::process_event(rawevent, texneut);
      if(processedevent.keep){
        goodcount++;
        treebiz::fill_PTreeData(PData, processedevent);
        PTree.Fill();
        analysis_feed AFeed = Analyse(processedevent, texneut);
        std::cout<<" // AFeed.timstamp: "<<AFeed.timestamp;
        std::cout<<" // AFeed.coupledhits: "<<AFeed.coupledhits<<std::endl;
        treebiz::fill_ATreeData(AData, AFeed);
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