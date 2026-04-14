#pragma once

#include "proggy/config.hpp"

#include <string>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

namespace namebuilder{
  std::string get_InputEvtName(const INFOCON&, int);
  std::string get_InputROOTName(const INFOCON&, int, std::string modifier = "");
  std::string get_OutputROOTName(const INFOCON&, int, std::string modifier = "");
};