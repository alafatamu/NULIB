#include "io/baseroot.hpp"
#include "io/evthandler.hpp"
#include "analysis/eventutils.hpp"
#include "proggy/tformat.hpp"
#include "TTree.h"

#include <iostream>
#include <fstream>
#include <string>

using namespace eventdata;
using namespace tformat;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

namespace rooty{
  int Evt_to_ROOT(std::ifstream& InputEvtFile, std::string outputfilename){
    //make evthandler spit out events -- CURRENT WORK
    bool stillreading = true;
    int eventnumber = 0;
    int goodcount = 0;

    while(stillreading){
      eventnumber++;
      std::cout<<"                                                              \r";
      std::cout<<YELLOW<<"Reading... Currently at event " << eventnumber << RESET;
      std::flush(std::cout);
      raw_event rawevent = grab_event(InputEvtFile);
      if(rawevent.unpackflag==1){ goodcount++; 
      }else if(rawevent.unpackflag==2||rawevent.unpackflag==3) stillreading = false;
      if(!stillreading)break;
    }
    //need the evt file to be opened outside of the evthandler (new function for global evt use?)
    //need unpacker to return raw events (with a quality flag)
    //then we can save the raw event data based on their quality (worthy or not)

    std::cout<<std::endl;
    std::cout<<"Done!"<<std::endl;

    return 1; //successful conversion and storage!
  }
}