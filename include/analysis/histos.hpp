#pragma once
#include "TFile.h"
#include "TDirectory.h"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void base_histos(TFile&, TFile&, int runreq = -10);
void proc_histos(TFile&, TFile&, int runreq = -10);
void calc_histos(TFile&, TFile&, int runruq = -10, int barreq = 1234);
void set_histos(TFile&, TFile&, int barreq = 1234);

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......