#pragma once

#include <string>
#include <fstream>

#include "analysis/detector.hpp"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

namespace rooty{
  int Evt_to_ROOT(std::ifstream&, std::string, detector);
}