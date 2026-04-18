#include "proggy/tformat.hpp"
#include "io/evthandler.hpp"
#include "analysis/eventutils.hpp"
#include "proggy/bitutils.hpp"

#include <fstream>
#include <iostream>
#include <string>

using namespace eventutils;
using namespace tformat;
using namespace bitutils;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

namespace {
  bool UnpackXLM(const unsigned short* XLMBuffer, int& XLMwordcount, raw_event& event, bool printout=false){
    const unsigned short* p = XLMBuffer; //preserve the starting point of the buffer pointer
    int totalhits = XLMwordcount/5; //XLM has multiples of 5 words, so we divide by 5 to get the total number of hits
    if(printout) std::cout<<BLUE<<"Reading XLM data..."<<RESET<<std::endl;
    if(printout) std::cout<<"The chip and channel IDs are both in one word, then A, B, C, and T integrations are the following 4 words.";
    for(int i=0;i<totalhits;i++){
      if(printout) std::cout<<"\nReading hit "<<i<<"... ";
      unsigned short channelID=*p++; //dBuffer[0+(5*i)]
      event.chip.push_back((channelID>>5) &0xff);
      event.chan.push_back(channelID & 0x7);
      if(printout){
        std::cout<<"First word of hit "<<i<<" is: "<<channelID<<std::endl;
        std::cout<<"This gives chip: "<<event.chip[i]<<" and channel: "<<event.chan[i]<<std::endl;
      }
      //integrators A=Total, B=ShorTint, C=Longint
      event.Aint.push_back(*p++); //dBuffer[1+(5*i)];
      event.Bint.push_back(*p++); //dBuffer[2+(5*i)];
      event.Cint.push_back(*p++); //dBuffer[3+(5*i)];
      event.Tint.push_back(*p++); //dBuffer[4+(5*i)]; and so on...
      if(printout){
        std::cout<<"A, B, C, and T integrations: ";
        std::cout<<event.Aint[i]<<" "<<event.Bint[i]<<" "<<event.Cint[i]<<" "<<event.Tint[i];
        std::cout<<std::endl;
      }
    }
    return 1; //successful unpack!
  }

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  bool UnpackTDC(const unsigned short* TDCBuffer, int& TDCwordcount, raw_event& event, bool printout=false){
    if(printout) std::cout<<"Combining the TDC data into 32bit words."<<std::endl;
    const unsigned short* p = TDCBuffer; //preserve the starting point of the buffer pointer
    bool headerfound = false;
    bool endfound = false;

     int ExpectedCount = -1; //count provided by the header
     int decodedDataWords = 0;

    for(int i=0;i<TDCwordcount;i++){
      //Assemble one 32-bit V775 word from two 16-bit halfwords.
      uint16_t low = *p++; // apple
      uint16_t high = *p++; // pen
      uint32_t fullword = (static_cast<uint32_t>(high) << 16) | low; // apple pen
      uint32_t type = (fullword >> 24) & 0x7;   // bits [26:24] tell us the event type

      if (printout) std::cout<<"TDC word["<<i<<"] = 0x"<<std::hex<<fullword<<std::dec<<"  type="<<type<<std::endl;
      if (type == 0x2) { // header indicated by '010'
        headerfound = true;
        uint32_t geo   = (fullword >> 27) & 0x1F;
        uint32_t crate = (fullword >> 16) & 0xFF;
        uint32_t count   = (fullword >> 8)  & 0x3F;
        ExpectedCount = static_cast<int>(count);

        if (printout) std::cout<<"HEADER: geo="<<geo<<" crate="<<crate<<" count="<<count<<std::endl;
      }else if (type == 0x0) { // data indicated by '000'
        uint32_t channel = (fullword >> 16) & 0x1F;
        uint32_t valid   = (fullword >> 14) & 0x1;
        uint32_t under   = (fullword >> 13) & 0x1;
        uint32_t over    = (fullword >> 12) & 0x1;
        uint32_t value   = fullword & 0x0FFF;         // 12-bit converted value

        if (printout) {
          std::cout << "DATA:"
                    << " chan=" << channel
                    << " valid=" << valid
                    << " under=" << under
                    << " over=" << over
                    << " val=" << value
                    << std::endl;
        }

        // Keep only valid, non-underthreshold, non-overflow data.
        if (valid && !under && !over) {
          event.TDCchan.push_back(static_cast<int>(channel));
          event.TDCval.push_back(static_cast<int>(value));
        }
        decodedDataWords++;
      }else if (type == 0x4) { // EOB: 100
        endfound = true;

        uint32_t eventCounter = fullword & 0x00FFFFFF;

        if (printout) std::cout <<"EOB: "<<"eventcountert=" << eventCounter<< std::endl;
        break; //finish reading here
      }else if (type == 0x6) { // invalid data: 110
      if (printout) std::cout<<"INVALID DATA. Skipping..."<<std::endl;
      // Ignore it.
      }else{
        if(printout)std::cout<<YELLOW<<"Unknown TDC type: "<<type<<". Skipping..."<<RESET<<std::endl;
      }
    }

    //final checks
    if (!headerfound || !endfound) {
      if (printout)std::cout<<RED<<"TDC read failed: missing header or EOB."<<RESET<<std::endl;
      event.TDCchan.clear();
      event.TDCval.clear();
      return false;
    }
    if (ExpectedCount >= 0 && printout) {
      std::cout << "Header says " << ExpectedCount
                << " data words; decoded " << decodedDataWords
                << " data words total before filtering." << std::endl;
    }
    if (event.TDCchan.empty() || event.TDCchan.size() != event.TDCval.size()) {
      if (printout) std::cout<<RED<<"TDC read failed: empty vectors or size mismatch."<<RESET<<std::endl;
      return false;
    }

    if (printout)std::cout<<GREEN<<"TDC read completed successfully."<<RESET<<std::endl;
    return true;
  }

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  raw_event UnpackData(std::ifstream& evtfile, bool printout=false){ //0=bad data, 1=good data, 2=fail, 3=done
    //Structure is: header, data header, xlm data, tdc data
    //This function will read the two headers, then unpack the xlm and tdc data separately

    raw_event rawevent; //prepare a raw event struct
    //we'll output a raw event regardless of unpack status
    //the raw event has an unpack status flag

    unsigned short hBuffer[4]={0}; //prepare a header buffer
    //hbuffer = {nbytes, nbytes2, type, type2}
    //nbytes2 and type2 should be EMPTY (0)
    //nbytes gives the total bytes in this event (including the header)
    //type is assigned by the hardware. Type 30 is a physics event. 

    evtfile.read((char*)hBuffer, 8); //read the header into the buffer (4 words over 8 bytes)
    if(!evtfile){
      rawevent.unpackflag = 3; //if the file is empty after this read, break
      return rawevent;
    }
    if(printout) std::cout<<"Header: "<<hBuffer[0]<<" "<<hBuffer[1]<<" "<<hBuffer[2]<<" "<<hBuffer[3]<<std::endl;
    int dBufferBytes = hBuffer[0]-8; //subtract the header size from the total bytes
    int dBufferWords = dBufferBytes/2; //divide by 2 to get the number of words
    if(printout) std::cout<<"Data Buffer Bytes // Words: "<<dBufferBytes<<" // "<<dBufferWords<<std::endl;

    unsigned short dBuffer[4096] = {0}; //prepare a data header buffer
    //The data buffer has different format depending on the event type, so we must read the full block at once
    evtfile.read((char*)dBuffer, dBufferBytes);
    if(!evtfile){
      rawevent.unpackflag = 3; //if the file is empty after this read, break
      return rawevent;
    }
    if((hBuffer[0]>100)&&(hBuffer[2]!=2)){//skip the event if it's overloaded
      if(printout) std::cout<<"Event overloaded. Notice nbytes>100 AND type!=2. Skipping..."<<std::endl;
      rawevent.unpackflag = 0; //continue the read at next entry
      return rawevent;
    }

    if(int(hBuffer[2])==30){ //if it's a physics event, unpack it

      if(printout) std::cout<<"Physics event denoted by type==30. Unpacking..."<<std::endl;
      //The data header for a physics event is 8 words long
      //->{(dH+XLM+TDC) words, (dH+XLM) words, blank, XLM words, timestamp1, timestamp2, timestamp3, timestamp4}
      int XLMwordcount = dBuffer[3]; 
      int TDChalfwordcount = dBuffer[0]-dBuffer[1];
      if (TDChalfwordcount%2!=0){
        if(printout) std::cout<<RED<<"TDC data contains broken words. Skipping..."<<RESET<<std::endl;
        rawevent.unpackflag = 0; //continue the read at next entry
        return rawevent;
      }
      int TDCwordcount = TDChalfwordcount/2;
      
      if(printout){
        std::cout<<"Data Header: "<<dBuffer[0]<<" "<<dBuffer[1]<<" "<<dBuffer[2]<<" "<<dBuffer[3]<<std::endl;
        std::cout<<"XLM words // bytes: "<<XLMwordcount<<" // "<<XLMwordcount*2<<std::endl;
        std::cout<<"TDC words // bytes: "<<TDCwordcount<<" // "<<(TDCwordcount*4)<<std::endl;
      }

      //now, let's load up the timestamp
      long long int timestamp1, timestamp2, timestamp3, timestamp4;
      const long long int factor = 65536; //factor for timestamp conversion (16-bit shift)
      timestamp1 = dBuffer[4];
      timestamp2 = dBuffer[5];
      timestamp3 = dBuffer[6];
      timestamp4 = dBuffer[7];
      rawevent.timestamp = timestamp1 + (timestamp2*factor) + (timestamp3*factor*factor) + (timestamp4*factor*factor*factor);
      if(printout){
        std::cout<<"The following four words are the timestamp (apply bitshift to each following word)... "<<std::endl;
        std::cout<<timestamp1<<" "<<timestamp2<<" "<<timestamp3<<" "<<timestamp4<<std::endl;
        std::cout<<"... so the full timestamp is: "<<BOLD<<rawevent.timestamp<<RESET<<std::endl;
      }

      if(TDCwordcount<=2){ //Let's check to see if the TDC data is usable at all
        if(printout) std::cout<<RED<<"Sadly, there aren't enough TDC words to be useful..."<<RESET<<std::endl;
        rawevent.unpackflag = 0; //continue the read at next entry
        return rawevent;
      }

      int totalhits = XLMwordcount/5; //XLM has multiples of 5 words
      rawevent.hitcount = totalhits;
      if(printout) std::cout<<"The XLM will read out 5 words per hit in the event. The total number of hits is: "<<totalhits<<std::endl;
      if(XLMwordcount%5 != 0 || XLMwordcount > 4095 || totalhits > 32 || totalhits < 1){
        if(printout) std::cout<<RED<<"-> Rejection by wordcount (not a multiple of 5, too many hits, or too few hits)"<<RESET<<std::endl;
        rawevent.unpackflag = 0; //unsucessful unpack - let's move on :(
        return rawevent;
      }

      //okay, NOW we can unpack the full event data
      const unsigned short* XLMBuffer = dBuffer + 8; //create a pointer to the start of the XLM data in dBuffer
      int XLMresult = UnpackXLM(XLMBuffer, XLMwordcount, rawevent, printout); //unpack the XLM data
      if(printout) std::cout<<BLUE<<"XLM Complete... Reading TDC data..."<<RESET<<std::endl;

      const unsigned short* TDCBuffer = XLMBuffer + XLMwordcount; //pointer to the start of the TDC data in dBuffer
      int TDCresult = UnpackTDC(TDCBuffer, TDCwordcount, rawevent, printout); //unpack the TDC data
      bool GoodUnpack = XLMresult && TDCresult; //check to see if the XLM and TDC data were both unpacked
      
      if(GoodUnpack && rawevent.chan.size()>1){
        if(printout) std::cout<<YELLOW<<"Successful unpack AND multiple hits... This data is 'useful'."<<RESET<<std::endl;
        rawevent.unpackflag = 1;
        return rawevent;
      }
    } 
    rawevent.unpackflag = 0; //if it's not overloaded, nor physics event, just move on
    return rawevent;
  }
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int inspect_evt_file(std::ifstream& evtfile){
  //Read the file per event
  bool stillreading = true;
  int eventnumber = 0;
  int pauser = 1;

  int batchsize = 1;
  int goodcount = 0;
  int lastcount = 0;

  std::cout<<"Pause every __ GOOD events: ";std::cin>>batchsize;

  while(stillreading){
    eventnumber++;
    std::cout<<GREEN<<"\nReading... Currently at event " << eventnumber << RESET<<std::endl;

    raw_event rawevent = UnpackData(evtfile, true); //unpack the data and read the event unpack flag
    if(rawevent.unpackflag==1){ goodcount++; 
    }else if(rawevent.unpackflag==2||rawevent.unpackflag==3) stillreading = false;

    if(!stillreading)break;
    if(batchsize==0){ //always pause if the batch size is 0. Otherwise pause in batches 
      std::cout<<"Enter 1 to continue, 0 to stop: ";std::cin>>pauser;
      if(pauser==0)break;
    }else if(goodcount%batchsize==0&&goodcount!=lastcount){
      lastcount=goodcount;
      std::cout<<"Batch size reached. Continue?"<<std::endl;
      std::cout<<"Enter 1 to continue, 0 to stop: ";std::cin>>pauser;
      if(pauser==0)break;
    }
    
  }
  evtfile.close();
  return 1; //successful process
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

raw_event grab_event(std::ifstream& evtfile){
  //Read the next raw event from the file
  raw_event rawevent = UnpackData(evtfile);
  return rawevent;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

std::ifstream grabfile(std::string filename){
  std::cout<<"Looking for "<<filename<<std::endl;
  std::ifstream evtfile(filename);
  if(!evtfile.is_open()){
    std::cout<<RED<<"Error: could not open evt file"<<RESET<<std::endl;
    return evtfile; //Be sure to check for the evt in the parent function
  }else std::cout<<GREEN<<"Found!"<<RESET<<std::endl;
  return evtfile;
}