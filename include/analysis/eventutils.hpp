#pragma once

#include <vector>
#include "analysis/detector.hpp"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

namespace eventutils{
  //Useful Structs for passing data around
  struct raw_event{
    long long int timestamp;
    int hitcount;
    std::vector<int> chip;
    std::vector<int> chan;
    std::vector<int> Aint;
    std::vector<int> Bint;
    std::vector<int> Cint;
    std::vector<int> Tint;
    std::vector<int> TDCchan;
    std::vector<int> TDCval;
    int unpackflag; //used by the unpacker to determine unpacking status
  };

  struct processed_event{
    long long int timestamp;
    int hitcount;
    std::vector<int> chip; //need to fill
    std::vector<int> chan; //need to fill
    std::vector<int> Aint; //need to fill
    std::vector<int> Bint; //need to fill
    std::vector<int> Cint; //need to fill
    std::vector<int> Tint; //need to fill
    std::vector<int> TDCchan; //need to fill
    std::vector<int> TDCval; //need to fill

    int coupledhits, barmult; 
    //coupledhits requires a multiplicity of 2.
    // barmult is the amount of DIFFERENT bars hit in the event.
    std::vector<int> barshit; //a list of the bars hit in the event
    std::vector<int> chip_top, chip_bot;
    std::vector<int> chan_top, chan_bot;
    std::vector<int> Aint_top, Aint_bot;
    std::vector<int> Bint_top, Bint_bot;
    std::vector<int> Cint_top, Cint_bot;
    std::vector<int> Tint_top, Tint_bot;
    //std::vector<int> TDCchan_top, TDCchan_bot;
    //std::vector<int> TDCval_top, TDCval_bot;

    bool keep; //keep or toss depending on the processing
  };

  //Functions for handling the event data (and structs)
  void display_raw(raw_event);
  void display_event(processed_event);
  processed_event process_event(raw_event, detector);
}