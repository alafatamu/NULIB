#include "proggy/tformat.hpp"
#include "io/evthandler.hpp"
#include "events/eventutils.hpp"
#include "proggy/bitutils.hpp"

#include <fstream>
#include <iostream>
#include <string>

using namespace eventdata;
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
    if(printout) std::cout<<"NOTE: the TDC data comes in 32bit words, but we read it in 16bit segments.";
    //HERE THERE BE DRAGONS
    //THIS IS FOR THE CAEN V775 TDC -> 32 BIT READOUT, NOT 16 -> SEE BELOW INFO TO UNDERSTAND THE WORKAROUND
    unsigned short TDCword; //TDCword is 16 bits even though an actual word is 32 bits 
    bool TDCendfound = false; //self explanatory
    //Each ID word will hold channel info, then the following 16 bits will give the value
    //Header: 01010 010 00000000 followed by 16 other bits we don't need
    //Data:   01010 000 000 00000 where the first 000 indicates data and the last 5 bits are the channel
    //        0_VUO 00000000000 where v is validity, u is underflow, o is overflow. Last 11 bits are the value
    //END:    01010 100 00000000 where 100 indicates end of data. This is followed by 16 bits we don't need

    //New TDC loop
    const unsigned short* p = TDCBuffer; //preserve the starting point of the buffer pointer
    int stickprevention = 0;
    int wordsread = 0;
    while(!TDCendfound){
      if(wordsread>TDCwordcount)break;
      stickprevention++;
      if(stickprevention>100){
        if(printout) std::cout<<RED<<"TDC read got stuck. Stopping..."<<RESET<<std::endl;
        return 0; //unsuccessful unpack :(
      }
      TDCword = *p++; //read a 16 bit word (not a full 32 bit statement)
      if(printout) std::cout<<std::endl<<"The first 16bits of the TDC word are: "<<TDCword<<std::endl;
      uint8_t opener = firstbyte(TDCword); //check the first byte for data info
      if(TDCword==0){
        if(printout) std::cout<<"Seeing as this value is 0 (useless), we'll read the following 16 bits and move on..."<<std::endl;
        TDCword=*p++;
        if(printout) std::cout<<"The following 16bits of the TDC word are: "<<TDCword<<std::endl;
        wordsread++;
        continue;
      }else if(opener==82){ //be sure to read the next 16 bits for the full statement before moving on
        if(printout) std::cout<<"The first byte of this word is 82. Shouldn't be anything of use here..."<<std::endl;
        TDCword=*p++; //no need to record anything here as far as I'm aware
        if(printout) std::cout<<"The following 16bits of the TDC word are: "<<TDCword<<std::endl;
        wordsread++;
      }else if(opener==80){ //be sure to read the next 16 bits for the full statement before moving on
        if(printout){
          std::cout<<"The first byte of this word is 80. This means we have possible data here..."<<std::endl;
          std::cout<<"Our channel ID should be "<<(TDCword&0x001F)<<std::endl;
        }
        event.TDCchannel.push_back((TDCword&0x001F));
        TDCword=*p++;
        if(printout){
          std::cout<<"The following 16bits of the TDC word are: "<<TDCword<<std::endl;
          std::cout<<"Here, we are given the validity of the data according to the TDC itself: "<<(TDCword&0x2000)<<std::endl;
        }
        wordsread++;
        if((TDCword&0x4000)==0){ //if validity is 0 (not valid), remove the last channel entry and continue
          if(printout) std::cout<<"Seeing as the validity is 0, we'll remove the last channel entry and continue..."<<std::endl;
          event.TDCchannel.pop_back(); //can't reach this point without filling the channel vector with SOMETHING
          continue;
        }
        if(printout){
          std::cout<<"Looks like this data is valid, so we'll store the data from this word..."<<std::endl;
          std::cout<<"The value of the data may be TDCword&0x3800: "<<(TDCword&0x3800)<<std::endl;
          std::cout<<"ChatGPT claims the value is actually TDCword&0x07FF (11-bit): ";
          std::cout<<(TDCword&0x07FF)<<" or TDCword&0x0FFF (12-bit)"<<(TDCword&0x0FFF)<<std::endl;
        }
        event.TDCvalue.push_back((TDCword&0x3800));
      }else if(opener==84){
        if(printout) std::cout<<"The first byte of this word is 84. This means we have reached the end of the data..."<<std::endl;
        TDCword=*p++;
        if(printout) std::cout<<"The following 16bits of the TDC word are: "<<TDCword<<std::endl;
        wordsread++;
        TDCendfound = true;
      }else{
        if(printout) std::cout<<"This tells us nothing of value. Continuing..."<<std::endl;
        wordsread++;
      }
    }
    if(printout) std::cout<<BLUE<<"TDC read Complete... Checking for errors..."<<RESET<<std::endl;

    //Checks to flag a bad event as false
    if(!TDCendfound){
      event.TDCchannel.clear();
      event.TDCvalue.clear();
      if(printout) std::cout<<RED<<"TDC read failed. No end found..."<<RESET<<std::endl;
       return 0; //unsuccessful unpack :(
    }
    if( (event.TDCchannel.size()==0)  || (event.TDCchannel.size()!=event.TDCvalue.size())){
      if(printout){
        std::cout<<RED;
        std::cout<<"TDC read failed. Channel and value vectors are not the same size OR the channel vector is empty...";
        std::cout<<RESET<<std::endl;
      }
      return 0; //unsuccessful unpack :(
    }
    if(printout) std::cout<<GREEN<<"TDC read completed successfully..."<<RESET<<std::endl;

    return 1; //successful unpack!
  }

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  int UnpackData(std::ifstream* evtfile, bool printout=false){ //0=bad data, 1=good data, 2=fail, 3=done
    //Structure is: header, data header, xlm data, tdc data
    //This function will read the two headers, then unpack the xlm and tdc data separately

    unsigned short hBuffer[4]={0}; //prepare a header buffer
    //hbuffer = {nbytes, nbytes2, type, type2}
    //nbytes2 and type2 should be EMPTY (0)
    //nbytes gives the total bytes in this event (including the header)
    //type is assigned by the hardware. Type 30 is a physics event. 

    evtfile->read((char*)hBuffer, 8); //read the header into the buffer (4 words over 8 bytes)
    if(!(*evtfile)) return 3; //if the file is empty after this read, break
    if(printout) std::cout<<"Header: "<<hBuffer[0]<<" "<<hBuffer[1]<<" "<<hBuffer[2]<<" "<<hBuffer[3]<<std::endl;
    int dBufferBytes = hBuffer[0]-8; //subtract the header size from the total bytes
    int dBufferWords = dBufferBytes/2; //divide by 2 to get the number of words
    if(printout) std::cout<<"Data Buffer Bytes // Words: "<<dBufferBytes<<" // "<<dBufferWords<<std::endl;

    unsigned short dBuffer[4096] = {0}; //prepare a data header buffer
    //The data buffer has different format depending on the event type, so we must read the full block at once
    evtfile->read((char*)dBuffer, dBufferBytes);
    if(!(*evtfile)) return 3; //if the file is empty after this read, break

    if((hBuffer[0]>100)&&(hBuffer[2]!=2)){//skip the event if it's overloaded
      if(printout) std::cout<<"Event overloaded. Notice nbytes>100 AND type!=2. Skipping..."<<std::endl;
      return 0; //continue the read at next entry
    }

    if(int(hBuffer[2])==30){ //if it's a physics event, unpack it

      raw_event rawevent; //prepare a raw event struct
      if(printout) std::cout<<"Physics event denoted by type==30. Unpacking..."<<std::endl;
      //The data header for a physics event is 8 words long
      //->{(dH+XLM+TDC) words, (dH+XLM) words, blank, XLM words, timestamp1, timestamp2, timestamp3, timestamp4}
      int XLMwordcount = dBuffer[3]; 
      int TDCwordcount = dBuffer[0]-dBuffer[1];
      
      if(printout){
        std::cout<<"Data Header: "<<dBuffer[0]<<" "<<dBuffer[1]<<" "<<dBuffer[2]<<" "<<dBuffer[3]<<std::endl;
        std::cout<<"XLM words // bytes: "<<XLMwordcount<<" // "<<XLMwordcount*2<<std::endl;
        std::cout<<"TDC words // bytes: "<<TDCwordcount<<" // "<<(TDCwordcount*2)<<std::endl;
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
        return 0; //continue the read at next entry
      }

      int totalhits = XLMwordcount/5; //XLM has multiples of 5 words
      if(printout) std::cout<<"The XLM will read out 5 words per hit in the event. The total number of hits is: "<<totalhits<<std::endl;
      if(XLMwordcount%5 != 0 || XLMwordcount > 4095 || totalhits > 32 || totalhits < 1){
        if(printout) std::cout<<RED<<"-> Rejection by wordcount (not a multiple of 5, too many hits, or too few hits)"<<RESET<<std::endl;
        return 0; //unsucessful unpack - let's move on :(
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
        return 1;
      }
    } 
    return 0; //if it's not overloaded, nor physics event, just move on
  }
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int inspect_evt_file(std::string filename){
  std::ifstream evtfile(filename);
  if(!evtfile.is_open()){
    std::cout<<RED<<"Error: could not open evt file"<<RESET<<std::endl;
    return 0; //process failed somewhere
  }

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

    int UD_result = UnpackData(&evtfile, true); //unpack the data and get a result value
    if(UD_result==1){ goodcount++; 
    }else if(UD_result==2||UD_result==3) stillreading = false;

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

int convert_evt_file(std::string filename){
  std::cout<<"Looking for "<<filename<<std::endl;
  std::ifstream evtfile(filename);
  if(!evtfile.is_open()){
    std::cout<<RED<<"Error: could not open evt file"<<RESET<<std::endl;
    return 0; //process failed somewhere
  }else std::cout<<GREEN<<"Found!"<<RESET<<std::endl;

  //Read the file per event
  bool stillreading = true;
  int eventnumber = 0;

  while(stillreading){
    eventnumber++;

    std::cout<<"                                                              \r";
    std::cout<<YELLOW<<"Reading... Currently at event " << eventnumber << RESET;
    std::flush(std::cout);

    int UD_result = UnpackData(&evtfile, false); //unpack the data and get a result value
    if(UD_result==2||UD_result==3) stillreading = false;

    if(!stillreading)break;
  }
  std::cout<<std::endl;
  evtfile.close();
  std::cout<<"Done!"<<std::endl;

  return 1; //successful process
}