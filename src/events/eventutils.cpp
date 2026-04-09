#include "events/eventutils.hpp"

#include <iostream>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

namespace eventdata{
  void display_raw(raw_event event){
    std::cout<<"Timestamp: "<<event.timestamp<<std::endl;
    for(int i=0;i<event.chip.size();i++){
      std::cout<<"Chip: "<<event.chip[i]<<std::endl;
      std::cout<<"Channel: "<<event.chan[i]<<std::endl;
      std::cout<<"Aint: "<<event.Aint[i]<<std::endl;
      std::cout<<"Bint: "<<event.Bint[i]<<std::endl;
      std::cout<<"Cint: "<<event.Cint[i]<<std::endl;
      std::cout<<"Tint: "<<event.Tint[i]<<std::endl;
    }
    for(int i=0;i<event.TDCchannel.size();i++){
      std::cout<<"TDCchannel: "<<event.TDCchannel[i]<<std::endl;
      std::cout<<"TDCvalue: "<<event.TDCvalue[i]<<std::endl;
    }
  }

  void display_hit(processed_hit hit){
    std::cout<<"Chip: "<<hit.chip<<std::endl;
    std::cout<<"Channel: "<<hit.chan<<std::endl;
    std::cout<<"Aint: "<<hit.Aint<<std::endl;
    std::cout<<"Bint: "<<hit.Bint<<std::endl;
    std::cout<<"Cint: "<<hit.Cint<<std::endl;
    std::cout<<"Tint: "<<hit.Tint<<std::endl;
    std::cout<<"TDCchannel: "<<hit.TDCchannel<<std::endl;
    std::cout<<"TDCvalue: "<<hit.TDCvalue<<std::endl;
  }

  void display_event(processed_event event){
    std::cout<<"Good Hits: "<<event.goodhit_amt<<std::endl;
    std::cout<<"Timestamp: "<<event.timestamp<<std::endl;
    for(int i=0;i<event.hits.size();i++){
      std::cout<<"\nHit "<<i<<":\n";
      display_hit(event.hits[i]);
    }
  }
}