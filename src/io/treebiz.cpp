#include "io/treebiz.hpp"
#include "analysis/eventutils.hpp"
#include "TTree.h"

#include <vector>

namespace treebiz{
  //RAW STUFF 
  void RData::clear(){
    #define CLEAR_SCALAR(type, name, default_value) name = default_value;
      RDATA_SCALARS(CLEAR_SCALAR)
    #undef CLEAR_SCALAR

    #define CLEAR_VECTOR(type, name) name.clear();
      RDATA_VECTORS(CLEAR_VECTOR)
    #undef CLEAR_VECTOR

    return;
  }

  void RData::bindWrite(TTree& tree){
    #define BIND_SCALAR(type, name, default_value) tree.Branch(#name, &name);
      RDATA_SCALARS(BIND_SCALAR)
    #undef BIND_SCALAR

    #define BIND_VECTOR(type, name) tree.Branch(#name, &name);
      RDATA_VECTORS(BIND_VECTOR)
    #undef BIND_VECTOR

    return;
  }

  void RData::bindRead(TTree& tree){
    #define BIND_SCALAR(type, name, default_value) tree.SetBranchAddress(#name, &name);
      RDATA_SCALARS(BIND_SCALAR)
    #undef BIND_SCALAR

    #define BIND_VECTOR(type, name) tree.SetBranchAddress(#name, &r_##name);
      RDATA_VECTORS(BIND_VECTOR)
    #undef BIND_VECTOR

    return;
  }

  void RData::fillFrom(const eventutils::raw_event& event){
    //For this auto fill, the names in the event MUST match the names listed in treebiz.hpp

    #define FILL_SCALAR(type, name, default_value) name = event.name;
      RDATA_SCALARS(FILL_SCALAR)
    #undef FILL_SCALAR

    #define FILL_VECTOR(type, name) name = event.name;
      RDATA_VECTORS(FILL_VECTOR)
    #undef FILL_VECTOR

    return;
  }

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  //PROCESSED STUFF
  void PData::clear(){
    #define CLEAR_SCALAR(type, name, default_value) name = default_value;
      PDATA_SCALARS(CLEAR_SCALAR)
    #undef CLEAR_SCALAR

    #define CLEAR_VECTOR(type, name) name.clear();
      PDATA_VECTORS(CLEAR_VECTOR)
    #undef CLEAR_VECTOR

    return;
  }

  void PData::bindWrite(TTree& tree){
    #define BIND_SCALAR(type, name, default_value) tree.Branch(#name, &name);
      PDATA_SCALARS(BIND_SCALAR)
    #undef BIND_SCALAR

    #define BIND_VECTOR(type, name) tree.Branch(#name, &name);
      PDATA_VECTORS(BIND_VECTOR)
    #undef BIND_VECTOR

    return;
  }

  void PData::bindRead(TTree& tree){
    #define BIND_SCALAR(type, name, default_value) tree.SetBranchAddress(#name, &name);
      PDATA_SCALARS(BIND_SCALAR)
    #undef BIND_SCALAR

    #define BIND_VECTOR(type, name) tree.SetBranchAddress(#name, &r_##name);
      PDATA_VECTORS(BIND_VECTOR)
    #undef BIND_VECTOR

    return;
  }

  void PData::fillFrom(const eventutils::processed_event& event){
    //For this auto fill, the names in the event MUST match the names listed in treebiz.hpp

    #define FILL_SCALAR(type, name, default_value) name = event.name;
      PDATA_SCALARS(FILL_SCALAR)
    #undef FILL_SCALAR

    #define FILL_VECTOR(type, name) name = event.name;
      PDATA_VECTORS(FILL_VECTOR)
    #undef FILL_VECTOR

    return;
  }

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  //ANALYSIS STUFF

  void AData::clear(){
    #define CLEAR_SCALAR(type, name, default_value) name = default_value;
      ADATA_SCALARS(CLEAR_SCALAR)
    #undef CLEAR_SCALAR

    #define CLEAR_VECTOR(type, name) name.clear();
      ADATA_VECTORS(CLEAR_VECTOR)
    #undef CLEAR_VECTOR

    return;
  }

  void AData::bindWrite(TTree& tree){
    #define BIND_SCALAR(type, name, default_value) tree.Branch(#name, &name);
      ADATA_SCALARS(BIND_SCALAR)
    #undef BIND_SCALAR

    #define BIND_VECTOR(type, name) tree.Branch(#name, &name);
      ADATA_VECTORS(BIND_VECTOR)
    #undef BIND_VECTOR

    return;
  }

  void AData::bindRead(TTree& tree){
    #define BIND_SCALAR(type, name, default_value) tree.SetBranchAddress(#name, &name);
      ADATA_SCALARS(BIND_SCALAR)
    #undef BIND_SCALAR

    #define BIND_VECTOR(type, name) tree.SetBranchAddress(#name, &r_##name);
      ADATA_VECTORS(BIND_VECTOR)
    #undef BIND_VECTOR

    return;
  }

  void AData::fillFrom(const eventutils::analysed_event& event){
    //For this auto fill, the names in the event MUST match the names listed in treebiz.hpp

    #define FILL_SCALAR(type, name, default_value) name = event.name;
      ADATA_SCALARS(FILL_SCALAR)
    #undef FILL_SCALAR

    #define FILL_VECTOR(type, name) name = event.name;
      ADATA_VECTORS(FILL_VECTOR)
    #undef FILL_VECTOR

    return;
  }
}