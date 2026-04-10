#pragma once

#include "proggy/config.hpp"

#include <string>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

std::string get_InputEvtName(Config cfg, int runid);
std::string get_InputRootName(Config cfg, int runid);
std::string get_OutputRootName(Config cfg, int runid);