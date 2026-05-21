#pragma once

#include "analysis/eventutils.hpp"
#include "TTree.h"

#include <vector>

namespace treebiz{
  //This is some cool shit. Here, we'll define branches for automatic variable generation and binding.
  //If you want to add a new branch, such as an alternative PSD, just add PSD_alt to the list below and the rest is automated.
  #define ADATA_SCALARS(X) \
    X(long long int, timestamp, 0) \
    X(int, coupledhits, 0)

  #define ADATA_VECTORS(X) \
    X(int, bar_id) \
    X(int, chip_top) \
    X(int, chip_bot) \
    X(int, chan_top) \
    X(int, chan_bot) \
    X(int, Aint_top) \
    X(int, Aint_bot) \
    X(int, Bint_top) \
    X(int, Bint_bot) \
    X(int, Cint_top) \
    X(int, Cint_bot) \
    X(int, Tint_top) \
    X(int, Tint_bot) \
    X(double, PSD_top) \
    X(double, PSD_bot) \
    X(double, PSD) \
    X(double, xhit) \
    X(double, yhit) \
    X(double, zhit) \
    X(double, rho) \
    X(double, theta) \
    X(double, phi) \
    X(double, E_calc) \
    X(int, PIDtag)

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
  
  void init_RTree(TTree&, treebiz::RTreeData&);
  void init_PTree(TTree&, treebiz::PTreeData&);

  void fill_RTreeData(treebiz::RTreeData&, eventutils::raw_event&);
  void fill_PTreeData(treebiz::PTreeData&, eventutils::processed_event&);

  void set_RTreeBranches(TTree&, treebiz::RTreeReadData&);
  void set_PTreeBranches(TTree&, treebiz::PTreeReadData&);

  struct AData{
    //Continuation of the cool shit above. This is how we create the members defined above.
    #define DECLARE_SCALAR(type, name, default_value) type name = default_value;
      ADATA_SCALARS(DECLARE_SCALAR)
    #undef DECLARE_SCALAR

    #define DECLARE_VECTOR(type, name) std::vector<type> name;
      ADATA_VECTORS(DECLARE_VECTOR)
    #undef DECLARE_VECTOR

    //And this is how we create the pointer members
    #define DECLARE_VECTOR_PTR(type, name) std::vector<type>* r_##name = &name;
      ADATA_VECTORS(DECLARE_VECTOR_PTR)
    #undef DECLARE_VECTOR_PTR
    //note: we don't need scalar nullptrs because we can just pass their addresses to the tree

    //member functions
    void clear();
    void bindWrite(TTree& tree);
    void bindRead(TTree& tree);
    void fillFrom(const eventutils::analysed_event& event);
  };
}