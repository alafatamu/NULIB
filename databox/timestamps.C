#include <TFile.h>
#include <TTree.h>
#include <TString.h>
#include <TBranch.h>

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

void ts(int runnum) {
    TString input_filename = Form("run%d.root",runnum);
    TString output_filename = Form("run%d_times.txt",runnum);
    TFile* f = TFile::Open(input_filename, "READ");

    if (!f || f->IsZombie()) {
        std::cerr << "ERROR: Could not open input file: "
                  << input_filename << std::endl;
        return;
    }

    TTree* t = nullptr;
    t = (TTree*)f->Get("analysed");

    if (!t) {
        std::cerr << "ERROR: Could not find analysed tree." << std::endl;
        std::cerr << "Try opening the file in ROOT and running .ls to check the path."
                  << std::endl;
        f->Close();
        return;
    }

    std::cout << "Using tree: " << t->GetName() << std::endl;
    std::cout << "Entries: " << t->GetEntries() << std::endl;

    if (!t->GetBranch("timestamp")) {
        std::cerr << "ERROR: No branch named timestamp." << std::endl;
        f->Close();
        return;
    }

    if (!t->GetBranch("PIDtag")) {
        std::cerr << "ERROR: No branch named PIDtag." << std::endl;
        f->Close();
        return;
    }

    Long64_t timestamp = 0;
    std::vector<int>* PIDtag = nullptr;

    t->SetBranchStatus("*", 0);
    t->SetBranchStatus("timestamp", 1);
    t->SetBranchStatus("PIDtag", 1);

    t->SetBranchAddress("timestamp", &timestamp);
    t->SetBranchAddress("PIDtag", &PIDtag);

    std::ofstream out(output_filename);

    if (!out.is_open()) {
        std::cerr << "ERROR: Could not create output file: "
                  << output_filename << std::endl;
        f->Close();
        return;
    }

    Long64_t written = 0;
    Long64_t nentries = t->GetEntries();

    for (Long64_t i = 0; i < nentries; i++) {
        t->GetEntry(i);

        if (!PIDtag) continue;

        bool has_pid1 = false;

        for (int tag : *PIDtag) {
            if (tag == 1) {
                has_pid1 = true;
                break;
            }
        }

        if (has_pid1) {
            out << timestamp << "\n";
            written++;
        }
    }


    out.close();
    f->Close();

    std::cout << "Done." << std::endl;
    std::cout << "Wrote " << written
              << " timestamps to " << output_filename << std::endl;
}