#include "TFile.h"
#include "TDirectory.h"
#include "TKey.h"
#include "TObject.h"
#include "TH2.h"
#include "TCutG.h"
#include "TCanvas.h"
#include "TROOT.h"
#include "TSystem.h"

#include <iostream>
#include <string>
#include <cstdio>

void wait_for_enter()
{
    std::cout << "\nPress ENTER when you are ready to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

TCutG* find_latest_cut()
{
    // ROOT usually names the graphical cut "CUTG"
    TCutG* cut = dynamic_cast<TCutG*>(gROOT->FindObject("CUTG"));

    if (cut) return cut;

    // Fallback: search through ROOT's list of special objects
    TIter next(gROOT->GetListOfSpecials());
    TObject* obj = nullptr;

    while ((obj = next())) {
        if (obj->InheritsFrom(TCutG::Class())) {
            return dynamic_cast<TCutG*>(obj);
        }
    }

    return nullptr;
}

void make_PSD_cuts(
    const char* histfile_name = "6hepn_histos.root",
    const char* cutfile_name  = "PSD_cuts.root",
    const char* hist_name     = "PSDvQ"
)
{
    TFile* histfile = TFile::Open(histfile_name, "READ");

    if (!histfile || histfile->IsZombie()) {
        std::cerr << "ERROR: Could not open histogram file: "
                  << histfile_name << "\n";
        return;
    }

    TFile* cutfile = TFile::Open(cutfile_name, "UPDATE");

    if (!cutfile || cutfile->IsZombie()) {
        std::cerr << "ERROR: Could not open cut file: "
                  << cutfile_name << "\n";
        histfile->Close();
        return;
    }

    TCanvas* c = new TCanvas("c_make_PSD_cuts", "Draw PSD Cuts", 1000, 800);
    c->SetLogz(1);

    TIter nextkey(histfile->GetListOfKeys());
    TKey* key = nullptr;

    while ((key = dynamic_cast<TKey*>(nextkey()))) {

        TObject* obj = key->ReadObj();

        if (!obj) continue;

        if (!obj->InheritsFrom(TDirectory::Class())) {
            delete obj;
            continue;
        }

        TDirectory* bardir = dynamic_cast<TDirectory*>(obj);

        if (!bardir) {
            delete obj;
            continue;
        }

        std::string dirname = bardir->GetName();

        TH2* hPSD = nullptr;
        bardir->GetObject(hist_name, hPSD);

        if (!hPSD) {
            std::cout << "Skipping directory " << dirname
                      << " because histogram " << hist_name
                      << " was not found.\n";
            delete obj;
            continue;
        }

        c->cd();
        c->Clear();

        hPSD->SetTitle(Form("%s/%s", dirname.c_str(), hist_name));
        hPSD->Draw("COLZ");
        c->Update();

        std::cout << "\n====================================================\n";
        std::cout << "Directory: " << dirname << "\n";
        std::cout << "Histogram: " << hist_name << "\n";
        std::cout << "Draw a graphical cut around the accepted PSD region.\n";
        std::cout << "\n";
        std::cout << "Typical ROOT interaction:\n";
        std::cout << "  - Click the canvas.\n";
        std::cout << "  - Use the graphical cut tool / TCutG tool.\n";
        std::cout << "  - Close the polygon.\n";
        std::cout << "  - Then return here and press ENTER.\n";
        std::cout << "====================================================\n";

        // Remove any old CUTG before drawing the new one
        TObject* oldcut = gROOT->FindObject("CUTG");
        if (oldcut) {
            oldcut->Delete();
        }

        std::cout << "\nDraw the cut now.\n";
        std::cout << "Left-click points around the region.\n";
        std::cout << "Double-click to finish the cut.\n";

        TCutG* cut = dynamic_cast<TCutG*>(gPad->WaitPrimitive("CUTG", "CutG"));

        if (!cut) {
            std::cout << "No cut was drawn for " << dirname << ". Skipping.\n";
            delete obj;
            continue;
        }

        std::string cutname = dirname + "_PSD_cut";
        cut->SetName(cutname.c_str());
        cut->SetTitle(Form("PSD cut for %s", dirname.c_str()));

        cutfile->cd();
        cut->Write(cutname.c_str(), TObject::kOverwrite);

        std::cout << "Saved cut: " << cutname
                  << " to " << cutfile_name << "\n";

        // Rename/remove the in-memory object so the next cut starts cleanly
        cut->SetName(Form("%s_saved", cutname.c_str()));

        delete obj;
    }

    cutfile->Write();
    cutfile->Close();

    histfile->Close();

    std::cout << "\nDone. Cuts saved in " << cutfile_name << "\n";
}

//WRITTEN ENTIRELY BY CHATGPT

//HOW TO USE: 
//After generating 6hepn_histos.root, run this code via...

//root -l
//.L cutter.C
//make_PSD_cuts("6hepn_histos.root", "PSD_cuts.root", "PSD")

