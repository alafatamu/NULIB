#pragma once

#include <string>

struct Config {
    std::string TNDataDir = "/home/usr/TNDATA/";
    std::string ROOTDataDir = "../databox/";
    std::string OutputDir = "../databox/";
    std::string ExpInfoDir = "../expinfo/";
    bool folder_separated_data = true;
    std::string BarMapFile = "barmap.txt";
    std::string PosMapFile = "barpositionmap.txt";
    std::string GainFile = "pmtgains.txt";
    std::string VertexFile = "reactionvertex.txt";
};

Config load_config(const std::string& path);