#pragma once

#include "analysis/eventutils.hpp"
#include "TTree.h"

#include <vector>

namespace treebiz{
  //This is some cool shit. Here, we'll define branches for automatic variable generation and binding.
  //If you want to add a new branch, such as an alternative PSD, just add PSD_alt to the list below and the rest is automated.

  //Raw data defined here (read from evt files or manually entered)
  #define RDATA_SCALARS(R) \
    R(long long int, timestamp, 0) \
    R(int, hitcount, 0) \

  #define RDATA_VECTORS(R) \
    R(int, chip) \
    R(int, chan) \
    R(int, Aint) \
    R(int, Bint) \
    R(int, Cint) \
    R(int, Tint) \
    R(int, TDCchan) \
    R(int, TDCval)

  //Processed data defined here (processed form the raw data previously stored)
  #define PDATA_SCALARS(P) \
    P(long long int, timestamp, 0) \
    P(int, hitcount, 0) \
    P(int, coupledhits, 0) \
    P(int, barmult, 0) \

  #define PDATA_VECTORS(P) \
    P(int, bar_id) \
    P(int, chip) \
    P(int, chan) \
    P(int, Aint) \
    P(int, Bint) \
    P(int, Cint) \
    P(int, Tint) \
    P(int, TDCchan) \
    P(int, TDCval) \
    P(int, chip_top) \
    P(int, chan_top) \
    P(int, Aint_top) \
    P(int, Bint_top) \
    P(int, Cint_top) \
    P(int, Tint_top) \
    P(int, chip_bot) \
    P(int, chan_bot) \
    P(int, Aint_bot) \
    P(int, Bint_bot) \
    P(int, Cint_bot) \
    P(int, Tint_bot) \

  //Analysis data defined here (calculated from the processed data previously stored)
  #define ADATA_SCALARS(A) \
    A(long long int, timestamp, 0) \
    A(int, coupledhits, 0)

  #define ADATA_VECTORS(A) \
    A(int, bar_id) \
    A(int, chip_top) \
    A(int, chip_bot) \
    A(int, chan_top) \
    A(int, chan_bot) \
    A(int, Aint_top) \
    A(int, Aint_bot) \
    A(int, Bint_top) \
    A(int, Bint_bot) \
    A(int, Cint_top) \
    A(int, Cint_bot) \
    A(int, Tint_top) \
    A(int, Tint_bot) \
    A(double, PSD_top) \
    A(double, PSD_bot) \
    A(double, PSD) \
    A(double, xhit) \
    A(double, yhit) \
    A(double, zhit) \
    A(double, rho) \
    A(double, theta) \
    A(double, phi) \
    A(double, E_calc) \
    A(int, PIDtag)

  
  struct RData{ //Raw data
    //member variable generation
    #define DECLARE_SCALAR(type, name, default_value) type name = default_value;
      RDATA_SCALARS(DECLARE_SCALAR)
    #undef DECLARE_SCALAR

    #define DECLARE_VECTOR(type, name) std::vector<type> name;
      RDATA_VECTORS(DECLARE_VECTOR)
    #undef DECLARE_VECTOR

    #define DECLARE_VECTOR_PTR(type, name) std::vector<type>* r_##name = &name;
      RDATA_VECTORS(DECLARE_VECTOR_PTR)
    #undef DECLARE_VECTOR_PTR

    //member functions
    void clear();
    void bindWrite(TTree& tree);
    void bindRead(TTree& tree);
    void fillFrom(const eventutils::raw_event& event);
  };

  struct PData{ //Processed data
    //member variable generation
    #define DECLARE_SCALAR(type, name, default_value) type name = default_value;
      PDATA_SCALARS(DECLARE_SCALAR)
    #undef DECLARE_SCALAR

    #define DECLARE_VECTOR(type, name) std::vector<type> name;
      PDATA_VECTORS(DECLARE_VECTOR)
    #undef DECLARE_VECTOR

    #define DECLARE_VECTOR_PTR(type, name) std::vector<type>* r_##name = &name;
      PDATA_VECTORS(DECLARE_VECTOR_PTR)
    #undef DECLARE_VECTOR_PTR
    
    //member functions
    void clear();
    void bindWrite(TTree& tree);
    void bindRead(TTree& tree);
    void fillFrom(const eventutils::processed_event& event);
  };

  struct AData{ //Analysis data
    //member variable generation
    #define DECLARE_SCALAR(type, name, default_value) type name = default_value;
      ADATA_SCALARS(DECLARE_SCALAR)
    #undef DECLARE_SCALAR

    #define DECLARE_VECTOR(type, name) std::vector<type> name;
      ADATA_VECTORS(DECLARE_VECTOR)
    #undef DECLARE_VECTOR

    #define DECLARE_VECTOR_PTR(type, name) std::vector<type>* r_##name = &name;
      ADATA_VECTORS(DECLARE_VECTOR_PTR)
    #undef DECLARE_VECTOR_PTR

    //member functions
    void clear();
    void bindWrite(TTree& tree);
    void bindRead(TTree& tree);
    void fillFrom(const eventutils::analysed_event& event);
  };
}