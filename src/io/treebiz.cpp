#include "io/treebiz.hpp"
#include "analysis/eventutils.hpp"
#include "TTree.h"

#include <vector>

void AData::clear(){
  timestamp = 0;
  coupledhits = 0;

  bar_id.clear();

  chip_top.clear(); chip_bot.clear();
  chan_top.clear(); chan_bot.clear();

  Aint_top.clear(); Aint_bot.clear();
  Bint_top.clear(); Bint_bot.clear();
  Cint_top.clear(); Cint_bot.clear();
  Tint_top.clear(); Tint_bot.clear();

  PSD_top.clear(); PSD_bot.clear();
  PSD.clear();

  xhit.clear(); yhit.clear(); zhit.clear();
  rho.clear(); theta.clear(); phi.clear();

  E_calc.clear();

  PIDtag.clear();
  return;
}

void AData::bindWrite(TTree& tree){
  tree.Branch("timestamp", &timestamp);
  tree.Branch("coupledhits", &coupledhits);
  tree.Branch("bar_id", &bar_id);

  tree.Branch("chip_top", &chip_top);
  tree.Branch("chip_bot", &chip_bot);
  tree.Branch("chan_top", &chan_top);
  tree.Branch("chan_bot", &chan_bot);

  tree.Branch("Aint_top", &Aint_top);
  tree.Branch("Aint_bot", &Aint_bot);
  tree.Branch("Bint_top", &Bint_top);
  tree.Branch("Bint_bot", &Bint_bot);
  tree.Branch("Cint_top", &Cint_top);
  tree.Branch("Cint_bot", &Cint_bot);
  tree.Branch("Tint_top", &Tint_top);
  tree.Branch("Tint_bot", &Tint_bot);

  tree.Branch("PSD_top", &PSD_top);
  tree.Branch("PSD_bot", &PSD_bot);
  tree.Branch("PSD", &PSD);

  tree.Branch("xhit", &xhit);
  tree.Branch("yhit", &yhit);
  tree.Branch("zhit", &zhit);
  tree.Branch("rho", &rho);
  tree.Branch("theta", &theta);
  tree.Branch("phi", &phi);
  tree.Branch("E_calc", &E_calc);

  tree.Branch("PIDtag", &PIDtag);
  return;
}

void AData::bindRead(TTree& tree){
  tree.SetBranchAddress("timestamp", &timestamp);
  tree.SetBranchAddress("coupledhits", &coupledhits);
  tree.SetBranchAddress("bar_id", &bar_id);

  tree.SetBranchAddress("chip_top", &chip_top);
  tree.SetBranchAddress("chip_bot", &chip_bot);
  tree.SetBranchAddress("chan_top", &chan_top);
  tree.SetBranchAddress("chan_bot", &chan_bot);

  tree.SetBranchAddress("Aint_top", &Aint_top);
  tree.SetBranchAddress("Aint_bot", &Aint_bot);
  tree.SetBranchAddress("Bint_top", &Bint_top);
  tree.SetBranchAddress("Bint_bot", &Bint_bot);
  tree.SetBranchAddress("Cint_top", &Cint_top);
  tree.SetBranchAddress("Cint_bot", &Cint_bot);
  tree.SetBranchAddress("Tint_top", &Tint_top);
  tree.SetBranchAddress("Tint_bot", &Tint_bot);

  tree.SetBranchAddress("PSD_top", &PSD_top);
  tree.SetBranchAddress("PSD_bot", &PSD_bot);
  tree.SetBranchAddress("PSD", &PSD);

  tree.SetBranchAddress("xhit", &xhit);
  tree.SetBranchAddress("yhit", &yhit);
  tree.SetBranchAddress("zhit", &zhit);
  tree.SetBranchAddress("rho", &rho);
  tree.SetBranchAddress("theta", &theta);
  tree.SetBranchAddress("phi", &phi);
  tree.SetBranchAddress("E_calc", &E_calc);

  tree.SetBranchAddress("PIDtag", &PIDtag);
  return;
}

void AData::fillFrom(const eventutils::analysed_event& event){
  timestamp = event.timestamp;
  coupledhits = event.coupledhits;
  bar_id = event.bar_id;

  chip_top = event.chip_top;
  chip_bot = event.chip_bot;
  chan_top = event.chan_top;
  chan_bot = event.chan_bot;

  Aint_top = event.Aint_top;
  Aint_bot = event.Aint_bot;
  Bint_top = event.Bint_top;
  Bint_bot = event.Bint_bot;
  Cint_top = event.Cint_top;
  Cint_bot = event.Cint_bot;
  Tint_top = event.Tint_top;
  Tint_bot = event.Tint_bot;

  PSD_top = event.PSDtop;
  PSD_bot = event.PSDbot;
  PSD = event.PSD;

  xhit = event.xhit;
  yhit = event.yhit;
  zhit = event.zhit;
  rho = event.rho;
  theta = event.theta;
  phi = event.phi;
  E_calc = event.E_calc;

  PIDtag = event.PIDtag;
  return;
}

namespace treebiz{
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
    //tree.Branch("TDCchan_top", &PData.TDCchan_top);
    //tree.Branch("TDCval_top", &PData.TDCval_top);

    tree.Branch("chip_bot", &PData.chip_bot);
    tree.Branch("chan_bot", &PData.chan_bot);
    tree.Branch("Aint_bot", &PData.Aint_bot);
    tree.Branch("Bint_bot", &PData.Bint_bot);
    tree.Branch("Cint_bot", &PData.Cint_bot);
    tree.Branch("Tint_bot", &PData.Tint_bot);
    //tree.Branch("TDCchan_bot", &PData.TDCchan_bot);
    //tree.Branch("TDCval_bot", &PData.TDCval_bot);

    tree.Branch("bar_id", &PData.bar_id);
    tree.Branch("coupledhits", &PData.coupledhits);
    tree.Branch("barmult", &PData.barmult);
    return;
  }

  void init_ATree(TTree& tree, treebiz::ATreeData& AData){
    tree.Branch("timestamp", &AData.timestamp);
    tree.Branch("coupledhits", &AData.coupledhits);
    tree.Branch("bar_id", &AData.bar_id);
    tree.Branch("nmaybe", &AData.nmaybe);
    tree.Branch("chip_top", &AData.chip_top);
    tree.Branch("chip_bot", &AData.chip_bot);
    tree.Branch("chan_top", &AData.chan_top);
    tree.Branch("chan_bot", &AData.chan_bot);
    tree.Branch("Aint_top", &AData.Aint_top);
    tree.Branch("Aint_bot", &AData.Aint_bot);
    tree.Branch("Bint_top", &AData.Bint_top);
    tree.Branch("Bint_bot", &AData.Bint_bot);
    tree.Branch("Cint_top", &AData.Cint_top);
    tree.Branch("Cint_bot", &AData.Cint_bot);
    tree.Branch("Tint_top", &AData.Tint_top);
    tree.Branch("Tint_bot", &AData.Tint_bot);

    tree.Branch("PSD_top", &AData.PSD_top);
    tree.Branch("PSD_bot", &AData.PSD_bot);
    tree.Branch("PSD", &AData.PSD); 
    tree.Branch("xhit", &AData.xhit);
    tree.Branch("yhit", &AData.yhit);
    tree.Branch("zhit", &AData.zhit);
    tree.Branch("rho", &AData.rho);
    tree.Branch("theta", &AData.theta);
    tree.Branch("phi", &AData.phi);
    tree.Branch("E_calc", &AData.E_calc);
    return;
  }

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  //TREE FILLING
  void fill_RTreeData(treebiz::RTreeData& RData, eventutils::raw_event& rawevent){
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

  void fill_PTreeData(treebiz::PTreeData& PData, eventutils::processed_event& processedevent){
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
    //PData.TDCchan_top = processedevent.TDCchan_top;
    //PData.TDCval_top = processedevent.TDCval_top;

    PData.chip_bot = processedevent.chip_bot;
    PData.chan_bot = processedevent.chan_bot;
    PData.Aint_bot = processedevent.Aint_bot;
    PData.Bint_bot = processedevent.Bint_bot;
    PData.Cint_bot = processedevent.Cint_bot;
    PData.Tint_bot = processedevent.Tint_bot;
    //PData.TDCchan_bot = processedevent.TDCchan_bot;
    //PData.TDCval_bot = processedevent.TDCval_bot;

    PData.bar_id = processedevent.bar_id;
    PData.coupledhits = processedevent.coupledhits;
    PData.barmult = processedevent.barmult;
    return;
  }

  void fill_ATreeData(treebiz::ATreeData& AData, eventutils::analysed_event& analysedevent){
    AData.timestamp = analysedevent.timestamp;
    AData.coupledhits = analysedevent.coupledhits;
    AData.bar_id = analysedevent.bar_id;
    AData.nmaybe = analysedevent.PIDtag;
    AData.chip_top = analysedevent.chip_top;
    AData.chip_bot = analysedevent.chip_bot;
    AData.chan_top = analysedevent.chan_top;
    AData.chan_bot = analysedevent.chan_bot;
    AData.Aint_top = analysedevent.Aint_top;
    AData.Aint_bot = analysedevent.Aint_bot;
    AData.Bint_top = analysedevent.Bint_top;
    AData.Bint_bot = analysedevent.Bint_bot;
    AData.Cint_top = analysedevent.Cint_top;
    AData.Cint_bot = analysedevent.Cint_bot;
    AData.Tint_top = analysedevent.Tint_top;
    AData.Tint_bot = analysedevent.Tint_bot;

    AData.PSD_top = analysedevent.PSDtop;
    AData.PSD_bot = analysedevent.PSDbot;
    AData.PSD = analysedevent.PSD;

    AData.xhit = analysedevent.xhit;
    AData.yhit = analysedevent.yhit;
    AData.zhit = analysedevent.zhit;
    AData.rho = analysedevent.rho;
    AData.theta = analysedevent.theta;
    AData.phi = analysedevent.phi;
    AData.E_calc = analysedevent.E_calc;
    return;
  }

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  //BRANCH SETTING
  void set_RTreeBranches(TTree& tree, treebiz::RTreeReadData& RData){
    tree.SetBranchAddress("timestamp", &RData.timestamp);
    tree.SetBranchAddress("hitcount", &RData.hitcount);
    tree.SetBranchAddress("chip", &RData.chip);
    tree.SetBranchAddress("chan", &RData.chan);
    tree.SetBranchAddress("Aint", &RData.Aint);
    tree.SetBranchAddress("Bint", &RData.Bint);
    tree.SetBranchAddress("Cint", &RData.Cint);
    tree.SetBranchAddress("Tint", &RData.Tint);
    tree.SetBranchAddress("TDCchan", &RData.TDCchan);
    tree.SetBranchAddress("TDCval", &RData.TDCval);
    return;
  }

  void set_PTreeBranches(TTree& tree, treebiz::PTreeReadData& PData){
    tree.SetBranchAddress("timestamp", &PData.timestamp);
    tree.SetBranchAddress("hitcount", &PData.hitcount);
    tree.SetBranchAddress("chip", &PData.chip);
    tree.SetBranchAddress("chan", &PData.chan);
    tree.SetBranchAddress("Aint", &PData.Aint);
    tree.SetBranchAddress("Bint", &PData.Bint);
    tree.SetBranchAddress("Cint", &PData.Cint);
    tree.SetBranchAddress("Tint", &PData.Tint);
    tree.SetBranchAddress("TDCchan", &PData.TDCchan);
    tree.SetBranchAddress("TDCval", &PData.TDCval);

    tree.SetBranchAddress("chip_top", &PData.chip_top);
    tree.SetBranchAddress("chan_top", &PData.chan_top);
    tree.SetBranchAddress("Aint_top", &PData.Aint_top);
    tree.SetBranchAddress("Bint_top", &PData.Bint_top);
    tree.SetBranchAddress("Cint_top", &PData.Cint_top);
    tree.SetBranchAddress("Tint_top", &PData.Tint_top);
    //tree.SetBranchAddress("TDCchan_top", &PData.TDCchan_top);
    //tree.SetBranchAddress("TDCval_top", &PData.TDCval_top);

    tree.SetBranchAddress("chip_bot", &PData.chip_bot);
    tree.SetBranchAddress("chan_bot", &PData.chan_bot);
    tree.SetBranchAddress("Aint_bot", &PData.Aint_bot);
    tree.SetBranchAddress("Bint_bot", &PData.Bint_bot);
    tree.SetBranchAddress("Cint_bot", &PData.Cint_bot);
    tree.SetBranchAddress("Tint_bot", &PData.Tint_bot);
    //tree.SetBranchAddress("TDCchan_bot", &PData.TDCchan_bot);
    //tree.SetBranchAddress("TDCval_bot", &PData.TDCval_bot);

    tree.SetBranchAddress("bar_id", &PData.bar_id);
    tree.SetBranchAddress("coupledhits", &PData.coupledhits);
    tree.SetBranchAddress("barmult", &PData.barmult);
    return;
  }

  void set_ATreeBranches(TTree& tree, treebiz::ATreeReadData& AData){
    tree.SetBranchAddress("timestamp", &AData.timestamp);
    tree.SetBranchAddress("coupledhits", &AData.coupledhits);
    tree.SetBranchAddress("bar_id", &AData.bar_id);
    tree.SetBranchAddress("nmaybe", &AData.nmaybe);
    
    tree.SetBranchAddress("chip_top", &AData.chip_top);
    tree.SetBranchAddress("chip_bot", &AData.chip_bot);
    tree.SetBranchAddress("chan_top", &AData.chan_top);
    tree.SetBranchAddress("chan_bot", &AData.chan_bot);
    tree.SetBranchAddress("Aint_top", &AData.Aint_top);
    tree.SetBranchAddress("Aint_bot", &AData.Aint_bot);
    tree.SetBranchAddress("Bint_top", &AData.Bint_top);
    tree.SetBranchAddress("Bint_bot", &AData.Bint_bot);
    tree.SetBranchAddress("Cint_top", &AData.Cint_top);
    tree.SetBranchAddress("Cint_bot", &AData.Cint_bot);
    tree.SetBranchAddress("Tint_top", &AData.Tint_top);
    tree.SetBranchAddress("Tint_bot", &AData.Tint_bot);

    tree.SetBranchAddress("PSD_top", &AData.PSD_top);
    tree.SetBranchAddress("PSD_bot", &AData.PSD_bot);
    tree.SetBranchAddress("PSD", &AData.PSD);
    tree.SetBranchAddress("xhit", &AData.xhit);
    tree.SetBranchAddress("yhit", &AData.yhit);
    tree.SetBranchAddress("zhit", &AData.zhit);
    tree.SetBranchAddress("rho", &AData.rho);
    tree.SetBranchAddress("theta", &AData.theta);
    tree.SetBranchAddress("phi", &AData.phi);
    tree.SetBranchAddress("E_calc", &AData.E_calc);
    return;
  }
}