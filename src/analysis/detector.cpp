//load detector/experiment mapping into detector object
#include "analysis/detector.hpp"
#include "proggy/tformat.hpp"
#include "proggy/config.hpp"

#include "TFile.h"
#include "TCutG.h"

#include <cmath>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>

using namespace tformat;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// DESTRUCTOR

detector::~detector(){
  for (TCutG* cut : psd_cuts) {
    delete cut;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// GETTER FUNCTIONS

int detector::getbar(int chip, int chan){return map_bar[chip][chan];}
int detector::get_pmt_id(int chip, int chan){return map_chan[chip][chan];}

double detector::get_gainfactors(int bar, int spot){return map_gainfactors[bar][spot];} // spot is top (0) or bottom (1)

int detector::get_tdcchan(int chip, int chan){return map_tdcchan[chip][chan];}

int detector::get_top_pmt_id(int bar){return map_toppmts[bar];}

int detector::get_bottom_pmt_id(int bar){return map_bottompmts[bar];}

double detector::get_coord(int chip, int chan, int coordreq){
  // 0 is x, 1 is y
  if (coordreq == 0) return static_cast<double>(map_x[chip][chan]);
  if (coordreq == 1) return static_cast<double>(map_y[chip][chan]);
  return 0.0; // TODO: default behavior here should maybe be NAN or throw an error
}

double detector::get_offset(int chip, int chan, int osreq){
  // osreq = offset request, 0 is A and 1 is B
  double offsets[2];
  offsets[0] = map_Aoffset[chip][chan];
  offsets[1] = map_Boffset[chip][chan];
  return offsets[osreq];
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// PSD CUT FUNCTIONS

bool detector::load_PSDcuts(std::string PSDCutFileName){
  TFile cutfile(PSDCutFileName.c_str(), "READ");
  if (!cutfile.IsOpen() || cutfile.IsZombie()) {
    std::cout << BOLDRED << "PSD cut file " << PSDCutFileName
              << " does not exist or failed to open" << RESET << std::endl;
    psd_cuts_loaded = false;
    return false;
  }
  std::cout << GREEN << "PSD cut file " << PSDCutFileName
            << " opened" << RESET << std::endl;
  int nloaded = 0;

  for (int bar = 0; bar < static_cast<int>(psd_cuts.size()); ++bar) {
    // If cuts were already loaded, delete old copies before replacing them.
    if (psd_cuts[bar]) {
      delete psd_cuts[bar];
      psd_cuts[bar] = nullptr;
    }

    char cutname[128];
    std::snprintf(cutname, sizeof(cutname), "bar%d_PSD_cut", bar);
    TCutG* filecut = nullptr;
    cutfile.GetObject(cutname, filecut);
    // Optional fallback in case your saved cut names use capital "Bar".
    if (!filecut) {
      char altcutname[128];
      std::snprintf(altcutname, sizeof(altcutname), "Bar%d_PSD_cut", bar);
      cutfile.GetObject(altcutname, filecut);
    }

    if (!filecut) continue;

    // Clone the cut so the detector owns it after cutfile closes.
    psd_cuts[bar] = dynamic_cast<TCutG*>(filecut->Clone(cutname));

    if (!psd_cuts[bar]) {
      std::cout << BOLDRED << "Failed to clone PSD cut for bar "
                << bar << RESET << std::endl;
      continue;
    }

    ++nloaded;
  }

  cutfile.Close();
  psd_cuts_loaded = (nloaded > 0);

  if (psd_cuts_loaded) {
    std::cout << GREEN << "Loaded " << nloaded << " PSD cuts" << RESET << std::endl;
  }else{ 
    std::cout << BOLDRED << "No PSD cuts were loaded from " << PSDCutFileName << RESET << std::endl;
  }

  return psd_cuts_loaded;
}

bool detector::has_PSDcut(int bar) const{
  if (bar < 0 || bar >= static_cast<int>(psd_cuts.size()))return false;
  return psd_cuts[bar] != nullptr;
}

int detector::PIDtag(int bar, double AB, double PSD) const{
  int PIDtag = 0;
  if (bar < 0 || bar >= static_cast<int>(psd_cuts.size())) return PIDtag;
  if (!std::isfinite(AB) || !std::isfinite(PSD)) return PIDtag;
  TCutG* cut = psd_cuts[bar];
  if (!cut) PIDtag = -1; //-1 output for filtering the multitude of no-cut bars
  if (cut->IsInside(AB, PSD)) PIDtag = 1; //1 indicates a positive cut (neutron)
  return PIDtag;
}

bool detector::has_PSDcuts(){
  if (psd_cuts_loaded) return true;
  return false;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// MAP READERS

bool detector::ReadMap(std::string barmapfile)
{
  //////////////////////////Read PMT channel map///////////////////////////////
  // This map had chip+channel information from the PSD chips. It
  // also gives the offsets for A-gate and B-gate integrations
  // from the PSD chips
  /////////////////////////////////////////////////////////////////////////////

  std::ifstream inmap(barmapfile);

  if (!inmap.is_open()) {
    std::cout << BOLDRED << "Bar map file " << barmapfile
              << " does not exist or failed to open" << RESET << std::endl;
    return false;
  } else {
    std::cout << GREEN << "Bar mapping file " << barmapfile
              << " opened\n" << RESET;
  }

  int inchip, inchipchan, outchan, outtdcchan, outx, outy, outbarnum;
  double outAoffset, outBoffset;

  while (inmap >> inchip >> inchipchan >> outchan >> outtdcchan >> outx
               >> outy >> outbarnum >> outAoffset >> outBoffset) {

    map_x[inchip][inchipchan] = outx;
    map_y[inchip][inchipchan] = outy;
    map_bar[inchip][inchipchan] = outbarnum;
    map_chan[inchip][inchipchan] = outchan;
    map_tdcchan[inchip][inchipchan] = outtdcchan;
    map_Aoffset[inchip][inchipchan] = outAoffset;
    map_Boffset[inchip][inchipchan] = outBoffset;
  }

  return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

bool detector::ReadPositionMap(std::string posmapfile)
{
  /////////////////////////Read PMT channel map////////////////////////////////
  // This map gives the X,Y,Z position for each crystal. It also gives the TDC
  // offset for the top and bottom PMTs measured for each crystal. The
  // coordinate system is the typical one used for TexAT, which IS NOT the
  // standard beam physics experiment coordinate system. Z is up/down.
  /////////////////////////////////////////////////////////////////////////////

  std::ifstream inposmap(posmapfile);

  if (!inposmap.is_open()) {
    std::cout << BOLDRED << "Bar pos map file " << posmapfile
              << " does not exist or failed to open" << RESET << std::endl;
    return false;
  } else {
    std::cout << GREEN << "BAR position mapping file " << posmapfile
              << " opened\n" << RESET;
  }

  int inbar, incrystal;
  double outx, outy, outz;
  double crystal_top_tdc_offset, crystal_bot_tdc_offset;
  double crystal_top_psdT_slope, crystal_top_psdT_offset;
  double crystal_bot_psdT_slope, crystal_bot_psdT_offset;
  double crystal_top_psdTgamma_offset, crystal_bot_psdTgamma_offset;

  while (inposmap >> inbar >> incrystal >> crystalhitID >> outx >> outy >> outz
                  >> crystal_top_tdc_offset >> crystal_bot_tdc_offset
                  >> crystal_top_psdT_slope >> crystal_top_psdT_offset
                  >> crystal_bot_psdT_slope >> crystal_bot_psdT_offset
                  >> crystal_top_psdTgamma_offset >> crystal_bot_psdTgamma_offset) {

    map_crystalhitID[inbar][incrystal] = crystalhitID;

    map_crystalx[inbar][incrystal] = outx;
    map_crystaly[inbar][incrystal] = outy;
    map_crystalz[inbar][incrystal] = outz;

    map_crystaltoptdcoffset[inbar][incrystal] = crystal_top_tdc_offset;
    map_crystalbottomtdcoffset[inbar][incrystal] = crystal_bot_tdc_offset;

    map_crystaltoppsdTslope[inbar][incrystal] = crystal_top_psdT_slope;
    map_crystaltoppsdToffset[inbar][incrystal] = crystal_top_psdT_offset;

    map_crystalbottompsdTslope[inbar][incrystal] = crystal_bot_psdT_slope;
    map_crystalbottompsdToffset[inbar][incrystal] = crystal_bot_psdT_offset;

    map_crystaltoppsdTgammaoffset[inbar][incrystal] = crystal_top_psdTgamma_offset;
    map_crystalbottompsdTgammaoffset[inbar][incrystal] = crystal_bot_psdTgamma_offset;
  }

  return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

bool detector::ReadGains(std::string gainfile)
{
  // Read the PMT gain map.
  // These should be relative PMT gains.

  std::ifstream ingain(gainfile);

  if (!ingain.is_open()) {
    std::cout << BOLDRED << "PMT gains file " << gainfile
              << " does not exist or failed to open" << RESET << std::endl;
    return false;
  } else {
    std::cout << GREEN << "PMT gains file " << gainfile
              << " opened\n" << RESET;
  }

  int bar, pmttop, pmtbot;
  double topgain, botgain;

  while (ingain >> bar >> pmttop >> topgain >> pmtbot >> botgain) {
    map_toppmts[bar] = pmttop;
    map_bottompmts[bar] = pmtbot;
    map_gainfactors[bar][0] = topgain;
    map_gainfactors[bar][1] = botgain;
  }

  return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

bool detector::fill_maps(INFOCON cfg){
  std::string BarMapFileName = cfg.ExpInfoDir + cfg.BarMapFile;
  std::string PosMapFileName = cfg.ExpInfoDir + cfg.PosMapFile;
  std::string GainFileName = cfg.ExpInfoDir + cfg.GainFile;
  std::string PSDCutFileName = cfg.OutputDir + cfg.PSDCutFile;

  bool mapread = ReadMap(BarMapFileName);
  bool posmapread = ReadPositionMap(PosMapFileName);
  bool gainread = ReadGains(GainFileName);
  bool psdcutread = load_PSDcuts(PSDCutFileName);

  if (!mapread || !posmapread || !gainread) return false;
  return true;
}