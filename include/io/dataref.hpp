#pragma once

#include "proggy/config.hpp"

#include <string>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

namespace namebuilder{
  std::string get_InputEvtName(const Config&, int);
  std::string get_InputROOTName(const Config&, int, std::string modifier = "");
  std::string get_OutputROOTName(const Config&, int, std::string modifier = "");
};