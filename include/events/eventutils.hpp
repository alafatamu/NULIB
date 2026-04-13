#pragma once

#include <vector>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

namespace eventdata{
  //Useful Structs for passing data around
  struct raw_event{
    bool worthy;
    long long int timestamp;
    std::vector<int> chip;
    std::vector<int> chan;
    std::vector<int> Aint;
    std::vector<int> Bint;
    std::vector<int> Cint;
    std::vector<int> Tint;
    std::vector<int> TDCchan;
    std::vector<int> TDCval;
  };

  struct processed_event{
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
  };

  //Functions for handling the event data (and structs)
  void display_raw(raw_event);
  void display_event(processed_event);
  processed_event process_raw_event(raw_event);
}