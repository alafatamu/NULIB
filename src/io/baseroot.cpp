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
  int Evt_to_ROOT(std::ifstream& InputEvtFile, std::string outputfilename, detector texneut){
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
        processed_event processedevent = eventdata::process_event(rawevent, texneut);
        if(processedevent.keep){goodcount++;
        }else continue;
      }else if(rawevent.unpackflag==2||rawevent.unpackflag==3) stillreading = false;
      if(!stillreading)break;
    }

    std::cout<<std::endl;
    std::cout<<"Done!"<<std::endl;
    std::cout<<"Total 'events': "<<eventnumber<<std::endl;
    std::cout<<"Total events saved: "<<goodcount<<std::endl;

    return 1; //successful conversion and storage!
  }
}