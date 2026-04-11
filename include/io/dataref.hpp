#pragma once

#include "proggy/config.hpp"

#include <string>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

namespace namebuilder{
  std::string get_InputEvtName(const Config& cfg, int runid, std::string modifier = "");
  std::string get_InputRootName(const Config& cfg, int runid, std::string modifier = "");
  std::string get_OutputRootName(const Config& cfg, int runid, std::string modifier = "");
};