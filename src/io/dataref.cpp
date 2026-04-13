#include "io/dataref.hpp"

#include "proggy/config.hpp"
#include "proggy/tformat.hpp"

#include <string>

using namespace tformat;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

namespace namebuilder{

  std::string get_InputEvtName(const Config& cfg, int runid){
    std::string evtfilename = cfg.TNDataDir; //get the data directory
    if(cfg.folder_separated_data)evtfilename += "run" + std::to_string(runid) + "/"; //add the run folder to the name (conditional)
    evtfilename += "run-" + std::to_string(runid) + "-00.evt"; //add the run file to the name
    return evtfilename;
  }

  std::string get_InputRootName(const Config& cfg, int runid, std::string modifier){
    std::string rootfilename = cfg.ROOTDataDir; //get the data directory
    rootfilename += "run" + std::to_string(runid) + modifier +".root"; //add the runID to the filename
    return rootfilename;
  }

  std::string get_OutputROOTName(const Config& cfg, int runid, std::string modifier){
    std::string outputfilename = cfg.OutputDir; //get the data directory
    outputfilename += "run" + std::to_string(runid) + modifier + ".root"; //add the runID to the filename
    return outputfilename;
  }

};