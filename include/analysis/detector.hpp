#pragma once

#include <string>
#include <array>
#include "proggy/config.hpp"

class TCutG;

class detector{
  private:
    // Calibration slope and offsets to convert from (x, y, z) index to millimeters
    double xm = 28.6258, xb = -214.6935, ym = 41.3766, yb = 0, zm = 21, zb = -52.5;

    // Arrays used to map chips/channels to bars and give offsets.
    int map_x[32][8], map_y[32][8], map_bar[32][8], map_chan[32][8], map_tdcchan[32][8],
        map_Aoffset[32][8], map_Boffset[32][8], map_crystalhitID[128][6];

    double map_crystalx[128][6], map_crystaly[128][6], map_crystalz[128][6],
           map_crystaltoptdcoffset[128][6], map_crystalbottomtdcoffset[128][6],
           map_crystaltoppsdTslope[128][6], map_crystaltoppsdToffset[128][6],
           map_crystalbottompsdTslope[128][6], map_crystalbottompsdToffset[128][6],
           map_crystaltoppsdTgammaoffset[128][6], map_crystalbottompsdTgammaoffset[128][6];

    double map_tdc_measured_offset[128][128];
    int map_toppmts[128], map_bottompmts[128];
    double map_gainfactors[128][2];

    int inchip, inchipchan, outchan, outtdcchan, outx, outy, outbarnum, outAoffset, outBoffset;
    int crystalhitID;

    double top_tsigma[6] = { 0.411, 0.766, 0.738, 0.980, 1.320, 2.410 };
    double bottom_tsigma[6] = { 1.930, 1.840, 1.180, 0.927, 0.674, 0.604 };

    // PSD graphical cuts, one per bar.
    std::array<TCutG*, 128> psd_cuts = {nullptr};

    bool psd_cuts_loaded = false;

  public:
    detector();
    ~detector();

    // Disable copying because this class owns TCutG pointers.
    detector(const detector&) = delete;
    detector& operator=(const detector&) = delete;

    // Allow moving only if you later need it. For now, easiest is to disable.
    detector(detector&&) = delete;
    detector& operator=(detector&&) = delete;

    // GETTER FUNCTIONS
    int getbar(int, int);
    int get_pmt_id(int, int);
    double get_xcoord(int, int);
    double get_ycoord(int, int);
    double get_offset(int, int, int);
    double get_gainfactors(int, int);
    int get_tdcchan(int, int);
    int get_top_pmt_id(int);
    int get_bottom_pmt_id(int);

    // PSD CUT FUNCTIONS
    bool load_PSDcuts(std::string);
    bool has_PSDcut(int bar) const;
    int PIDtag(int bar, double AB, double PSD) const;
    bool has_PSDcuts();

    // MAP READERS
    bool ReadMap(std::string);
    bool ReadPositionMap(std::string);
    bool ReadGains(std::string);
  
    bool fill_maps(INFOCON);
};