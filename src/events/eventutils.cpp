#include "events/eventutils.hpp"

#include <iostream>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

namespace eventdata{
  void display_raw(const raw_event& event){
    std::cout<<"Timestamp: "<<event.timestamp<<std::endl;
    for(int i=0;i<event.chip.size();i++){
      std::cout<<"Chip: "<<event.chip[i]<<std::endl;
      std::cout<<"Channel: "<<event.chan[i]<<std::endl;
      std::cout<<"Aint: "<<event.Aint[i]<<std::endl;
      std::cout<<"Bint: "<<event.Bint[i]<<std::endl;
      std::cout<<"Cint: "<<event.Cint[i]<<std::endl;
      std::cout<<"Tint: "<<event.Tint[i]<<std::endl;
    }
    for(int i=0;i<event.TDCchan.size();i++){
      std::cout<<"TDCchannel: "<<event.TDCchan[i]<<std::endl;
      std::cout<<"TDCvalue: "<<event.TDCval[i]<<std::endl;
    }
  }

  void display_event(const processed_event& event){
    std::cout<<"Good Hits: "<<event.hitcount<<std::endl;
    std::cout<<"Timestamp: "<<event.timestamp<<std::endl;
    for(int i=0;i<event.hitcount;i++){
      std::cout<<"Hit "<<i<<": "<<std::endl;
      std::cout<<"Chip: "<<event.chip[i]<<std::endl;
      std::cout<<"Channel: "<<event.chan[i]<<std::endl;
      std::cout<<"Aint: "<<event.Aint[i]<<std::endl;
      std::cout<<"Bint: "<<event.Bint[i]<<std::endl;
      std::cout<<"Cint: "<<event.Cint[i]<<std::endl;
      std::cout<<"Tint: "<<event.Tint[i]<<std::endl;
      std::cout<<"TDCchannel: "<<event.TDCchan[i]<<std::endl;
      std::cout<<"TDCvalue: "<<event.TDCval[i]<<std::endl;
    }
  }

  processed_event process_event(raw_event rawevent){
    processed_event output_event;
    return output_event;
  }
}