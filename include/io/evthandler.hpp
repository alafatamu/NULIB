#pragma once 
#include "analysis/eventutils.hpp"
#include "analysis/detector.hpp"

#include <string>
#include <fstream>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int inspect_evt_file(std::ifstream&);
int convert_evt_file(std::ifstream&);
eventdata::raw_event grab_event(std::ifstream&);
std::ifstream grabfile(std::string);