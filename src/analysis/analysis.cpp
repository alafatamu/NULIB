//Run analysis on a single processed event here

#include <vector>

#include "analysis/analysis.hpp"
#include "analysis/eventutils.hpp"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

analysis_feed Analyse(eventdata::processed_event& event, detector& texneut){
  analysis_feed AFeed;
  AFeed.timestamp = event.timestamp;
  AFeed.coupledhits = event.coupledhits;
  //ENERGY AND PSD CALCULATIONS
  for(int i=0;i<event.coupledhits;i++){
    //do the calculations before pushing them into AFeed
    double Etop = event.Aint_top[i]+event.Bint_top[i];
    double Ebot = event.Aint_bot[i]+event.Bint_bot[i];
    double Etot = event.Aint_top[i]+event.Bint_top[i]+event.Aint_bot[i]+event.Bint_bot[i];
    double PSDtop = 1.-(double)event.Bint_top[i]/(double)event.Aint_top[i];
    double PSDbot = 1.-(double)event.Bint_bot[i]/(double)event.Aint_bot[i];
    double PSD = 1.-((double)event.Bint_top[i]+(double)event.Bint_bot[i])/((double)event.Aint_top[i]+(double)event.Aint_bot[i]);

    AFeed.E_top.push_back(Etop);
    AFeed.E_bot.push_back(Ebot);
    AFeed.E_tot.push_back(Etot);
    AFeed.PSD_top.push_back(PSDtop);
    AFeed.PSD_bot.push_back(PSDbot);
    AFeed.PSD.push_back(PSD);
  }
  //HIT LOCATION CALCULATIONS
  return AFeed;
}