#pragma once

#include <string>
#include <fstream>

#include "TTree.h"
#include "TFile.h"
#include "analysis/detector.hpp"
#include "analysis/eventutils.hpp"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int Evt_to_ROOT(std::ifstream&, TFile&, detector&, int runid = -10);