#pragma once

#include "analysis/eventutils.hpp"
#include "TTree.h"

#include <vector>

struct AData{
  //data for the tree
  long long int timestamp = 0;
  int coupledhits = 0;
  std::vector<int> bar_id;
  std::vector<int> chip_top,chip_bot;
  std::vector<int> chan_top,chan_bot;
  std::vector<int> Aint_top,Aint_bot;
  std::vector<int> Bint_top,Bint_bot;
  std::vector<int> Cint_top,Cint_bot;
  std::vector<int> Tint_top,Tint_bot;

  std::vector<double> PSD_top, PSD_bot, PSD;
  std::vector<double> xhit, yhit, zhit; //hit location 
  std::vector<double> rho, theta, phi;
  std::vector<double> E_calc; //Total energy (top+bottom PMT)
  std::vector<int> PIDtag; //Particle ID tag - 1 for neutrons, 0 otherwise

  //member functions
  void clear();
  void bindWrite(TTree& tree);
  void bindRead(TTree& tree);
  void fillFrom(const eventutils::analysed_event& event);
};

namespace treebiz{
  //TREE DATA WRITE STRUCTS
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
    //std::vector<int> TDCchan_top,TDCchan_bot;
    //std::vector<int> TDCval_top,TDCval_bot;
    std::vector<int> bar_id;
    int coupledhits = 0;
    int barmult = 0;
  };

  struct ATreeData{ //analysis data tree
    long long int timestamp = 0;
    int coupledhits = 0;
    std::vector<int> bar_id;
    std::vector<int> nmaybe;
    std::vector<int> chip_top,chip_bot;
    std::vector<int> chan_top,chan_bot;
    std::vector<int> Aint_top,Aint_bot;
    std::vector<int> Bint_top,Bint_bot;
    std::vector<int> Cint_top,Cint_bot;
    std::vector<int> Tint_top,Tint_bot;

    std::vector<double> PSD_top, PSD_bot, PSD;
    std::vector<double> xhit, yhit, zhit; //hit location 
    std::vector<double> rho, theta, phi;
    std::vector<double> E_calc; //Total energy (top+bottom PMT)
  };

  //TREE DATA READ STRUCTS
  struct RTreeReadData{
    long long int timestamp = 0;
    int hitcount = 0;
    std::vector<int>* chip = nullptr;
    std::vector<int>* chan = nullptr;
    std::vector<int>* Aint = nullptr;
    std::vector<int>* Bint = nullptr;
    std::vector<int>* Cint = nullptr;
    std::vector<int>* Tint = nullptr;
    std::vector<int>* TDCchan = nullptr;
    std::vector<int>* TDCval = nullptr;
  };

  struct PTreeReadData{
    long long int timestamp = 0;
    int hitcount = 0;
    std::vector<int>* chip = nullptr;
    std::vector<int>* chan = nullptr;
    std::vector<int>* Aint = nullptr;
    std::vector<int>* Bint = nullptr;
    std::vector<int>* Cint = nullptr;
    std::vector<int>* Tint = nullptr;
    std::vector<int>* TDCchan = nullptr;
    std::vector<int>* TDCval = nullptr;

    std::vector<int>* chip_top = nullptr;
    std::vector<int>* chip_bot = nullptr;
    std::vector<int>* chan_top = nullptr;
    std::vector<int>* chan_bot = nullptr;
    std::vector<int>* Aint_top = nullptr;
    std::vector<int>* Aint_bot = nullptr;
    std::vector<int>* Bint_top = nullptr;
    std::vector<int>* Bint_bot = nullptr;
    std::vector<int>* Cint_top = nullptr;
    std::vector<int>* Cint_bot = nullptr;
    std::vector<int>* Tint_top = nullptr;
    std::vector<int>* Tint_bot = nullptr;
    //std::vector<int>* TDCchan_top = nullptr;
    //std::vector<int>* TDCchan_bot = nullptr;
    //std::vector<int>* TDCval_top = nullptr; 
    //std::vector<int>* TDCval_bot = nullptr;

    std::vector<int>* bar_id = nullptr;
    int coupledhits = 0;
    int barmult = 0;
  };

  struct ATreeReadData{
    long long int timestamp = 0;
    int coupledhits = 0;
    std::vector<int>* bar_id = nullptr;
    std::vector<int>* nmaybe = nullptr;
    std::vector<int>* chip_top = nullptr;
    std::vector<int>* chip_bot = nullptr;
    std::vector<int>* chan_top = nullptr;
    std::vector<int>* chan_bot = nullptr;
    std::vector<int>* Aint_top = nullptr;
    std::vector<int>* Aint_bot = nullptr;
    std::vector<int>* Bint_top = nullptr;
    std::vector<int>* Bint_bot = nullptr;
    std::vector<int>* Cint_top = nullptr;
    std::vector<int>* Cint_bot = nullptr;
    std::vector<int>* Tint_top = nullptr;
    std::vector<int>* Tint_bot = nullptr;

    std::vector<double>* PSD_top = nullptr;
    std::vector<double>* PSD_bot = nullptr;
    std::vector<double>* PSD = nullptr;
    
    std::vector<double>* xhit = nullptr;
    std::vector<double>* yhit = nullptr;
    std::vector<double>* zhit = nullptr;
    std::vector<double>* rho = nullptr;
    std::vector<double>* theta = nullptr;
    std::vector<double>* phi = nullptr;
    std::vector<double>* E_calc = nullptr;
  };
  
  void init_RTree(TTree&, treebiz::RTreeData&);
  void init_PTree(TTree&, treebiz::PTreeData&);
  void init_ATree(TTree&, treebiz::ATreeData&);

  void fill_RTreeData(treebiz::RTreeData&, eventutils::raw_event&);
  void fill_PTreeData(treebiz::PTreeData&, eventutils::processed_event&);
  void fill_ATreeData(treebiz::ATreeData&, eventutils::analysed_event&);

  void set_RTreeBranches(TTree&, treebiz::RTreeReadData&);
  void set_PTreeBranches(TTree&, treebiz::PTreeReadData&);
  void set_ATreeBranches(TTree&, treebiz::ATreeReadData&);
}