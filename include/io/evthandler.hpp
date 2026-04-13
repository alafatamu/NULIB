#pragma once 

#include <string>
#include <fstream>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int inspect_evt_file(std::ifstream& evtfile);
int convert_evt_file(std::ifstream& evtfile);
std::ifstream grabfile(std::string filename);