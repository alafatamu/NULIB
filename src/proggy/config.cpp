#include "proggy/config.hpp"

#include <cctype>
#include <fstream>
#include <functional>
#include <stdexcept>
#include <string>
#include <unordered_map>


//This file was entirely written by chatgpt
namespace {

std::string trim(const std::string& s) {
    const auto first = s.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) {
        return "";
    }

    const auto last = s.find_last_not_of(" \t\r\n");
    return s.substr(first, last - first + 1);
}

bool parse_bool(const std::string& value, const std::string& key) {
    if (value == "true") {
        return true;
    }
    if (value == "false") {
        return false;
    }
    throw std::invalid_argument(key + " must be either true or false");
}

std::string apply_default(const std::string& value, const std::string& fallback) {
    return value == "---" ? fallback : value;
}

} // namespace

Config load_config(const std::string& path) {
    Config cfg;  // starts with defaults from the struct

    if (path.empty()) {
        return cfg;
    }

    std::ifstream file(path);
    if (!file) {
        throw std::invalid_argument("Config file '" + path + "' failed to open");
    }

    using Handler = std::function<void(const std::string&)>;
    std::unordered_map<std::string, Handler> handlers = {
        { "TNDataDir", [&](const std::string& v) {
            cfg.TNDataDir = apply_default(v, "/home/usr/TNDATA/");
        }},
        { "ROOTDataDir", [&](const std::string& v) {
            cfg.ROOTDataDir = apply_default(v, "../databox/");
        }},
        { "OutputDir", [&](const std::string& v) {
            cfg.OutputDir = apply_default(v, "../databox/");
        }},
        { "ExpInfoDir", [&](const std::string& v) {
            cfg.ExpInfoDir = apply_default(v, "../expinfo/");
        }},
        { "folder_separated_data", [&](const std::string& v) {
            const std::string actual = apply_default(v, "true");
            cfg.folder_separated_data = parse_bool(actual, "folder_separated_data");
        }},
        { "BarMapFile", [&](const std::string& v) {
            cfg.BarMapFile = apply_default(v, "barmap.txt");
        }},
        { "PosMapFile", [&](const std::string& v) {
            cfg.PosMapFile = apply_default(v, "barpositionmap.txt");
        }},
        { "GainFile", [&](const std::string& v) {
            cfg.GainFile = apply_default(v, "pmtgains.txt");
        }},
        { "VertexFile", [&](const std::string& v) {
            cfg.VertexFile = apply_default(v, "reactionvertex.txt");
        }},
    };

    std::string line;
    std::size_t line_number = 0;

    while (std::getline(file, line)) {
        ++line_number;
        line = trim(line);

        if (line.empty() || line[0] == '#') {
            continue;
        }

        const auto eq = line.find('=');
        if (eq == std::string::npos) {
            throw std::invalid_argument(
                "Invalid config line " + std::to_string(line_number) + ": missing '='"
            );
        }

        const std::string key = trim(line.substr(0, eq));
        const std::string value = trim(line.substr(eq + 1));

        const auto it = handlers.find(key);
        if (it == handlers.end()) {
            throw std::invalid_argument(
                "Unknown config key on line " + std::to_string(line_number) + ": " + key
            );
        }

        it->second(value);
    }

    return cfg;
}