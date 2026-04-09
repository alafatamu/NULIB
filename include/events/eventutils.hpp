#pragma once

#include <vector>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

namespace eventdata{
  //Useful Structs for passing data around
  struct raw_event{
    long long int timestamp;
    std::vector<int> chip;
    std::vector<int> chan;
    std::vector<int> Aint;
    std::vector<int> Bint;
    std::vector<int> Cint;
    std::vector<int> Tint;
    std::vector<int> TDCchannel;
    std::vector<int> TDCvalue;
  };

  struct processed_hit{
    int chip;
    int chan;
    int Aint;
    int Bint;
    int Cint;
    int Tint;
    int TDCchannel;
    int TDCvalue;
  };

  struct processed_event{
    int goodhit_amt;
    long long int timestamp;
    std::vector<processed_hit> hits;
  };

  //Functions for handling the event data (and structs)
  void display_raw(raw_event);
  void display_hit(processed_hit);
  void display_event(processed_event);
  processed_event process_raw_event(raw_event);
}