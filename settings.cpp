#include "settings.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>

using namespace std;

void Settings::getSettings(const std::string& filename){

        ifstream file(filename);
        if (!file.is_open()) {
            cout << "Could not open settings file!" << endl;
            return;
        }

        unordered_map<string, string> keyValuePairs;

        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string key, value;
            if (getline(ss, key, '=') && getline(ss, value)) {
                keyValuePairs[key] = value;
            }
        }

        // Parse settings from the map
        if (keyValuePairs.find("N") != keyValuePairs.end()) {
            N = stoi(keyValuePairs["N"]);
        }
        if (keyValuePairs.find("TargetFPS") != keyValuePairs.end()) {
            TargetFPS = stoi(keyValuePairs["TargetFPS"]);
        }
        if (keyValuePairs.find("screenWidth") != keyValuePairs.end()) {
            screenWidth = stoi(keyValuePairs["screenWidth"]);
        }
        if (keyValuePairs.find("screenHeight") != keyValuePairs.end()) {
            screenHeight = stoi(keyValuePairs["screenHeight"]);
        }
        if (keyValuePairs.find("interfaceWidth") != keyValuePairs.end()) {
            interfaceWidth = stof(keyValuePairs["interfaceWidth"]);
        }
        if (keyValuePairs.find("diffusion") != keyValuePairs.end()) {
            diffusion = stof(keyValuePairs["diffusion"]);
        }
        if (keyValuePairs.find("viscosity") != keyValuePairs.end()) {
            viscosity = stof(keyValuePairs["viscosity"]);
        }
        if (keyValuePairs.find("minDiffusion") != keyValuePairs.end()) {
            minDiffusion = stof(keyValuePairs["minDiffusion"]);
        }
        if (keyValuePairs.find("maxDiffusion") != keyValuePairs.end()) {
            maxDiffusion = stof(keyValuePairs["maxDiffusion"]);
        }
        if (keyValuePairs.find("minViscosity") != keyValuePairs.end()) {
            minViscosity = stof(keyValuePairs["minViscosity"]);
        }
        if (keyValuePairs.find("maxViscosity") != keyValuePairs.end()) {
            maxViscosity = stof(keyValuePairs["maxViscosity"]);
        }

        file.close();
}