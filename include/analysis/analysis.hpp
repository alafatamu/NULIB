#pragma once

#include "analysis/detector.hpp"
#include "analysis/eventutils.hpp"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

struct analysis_feed{
  long long int timestamp = 0;
  int coupledhits = 0;
  std::vector<double> E_top, E_bot; //energy in top, bottom PMT
  std::vector<double> E_tot; //Total energy (top+bottom PMT)
  std::vector<double> PSD_top, PSD_bot, PSD;
  std::vector<int> PSDflag;
  std::vector<double> xhit, yhit, zhit; //hit location 
  std::vector<double> rho, theta, phi;
};

analysis_feed Analyse(eventdata::processed_event&, detector&);