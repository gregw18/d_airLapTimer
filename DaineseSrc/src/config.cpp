#include <iostream>

#include "config.h"
#include "stringHelpers.h"

using namespace std;

namespace gwDainese
{
    // For all non-blank, non comment (comments start with #) lines, break at = sign,
    // everything before is key, everything after is value.
    bool Config::init(const vector<string>& fileContents) {
        int numKeys = 0;
        for (string line: fileContents){
            if (isLineValid(line)) {
                auto [key, value] = split(line);
                if (key.length() > 0) {
                    configMap[key] = value;
                }
            }
        }

        return (numKeys > 0);
    }

   string Config::getValueStr(const string configKey) {
        string val;
        try {
            val = configMap.at(configKey);
        }
        catch (exception ex) {
            cout << "getValueStr for key: " << configKey << ", value: " << val 
                << ", exception: " << ex.what() << "\n";
            throw;
        }
        return val;
    }

   int Config::getValueInt(const string configKey) {
       string valStr = getValueStr(configKey);
       int val = 0;
        try {
            if (valStr.length() > 0) val = stoi(valStr);
        }
        catch (exception ex) {
            cout << "getValueInt for value: " << valStr << ", exception: " << ex.what() << "\n";
            throw;
        }
       return val;
   }

   double Config::getValueDbl(const string configKey) {
       string valStr = getValueStr(configKey);
       double val = 0;
        try {
            if (valStr.length() > 0) val = stod(valStr);
        }
        catch (exception ex) {
            cout << "getValueDbl for value: " << valStr << ", exception: " << ex.what() << "\n";
            throw;
        }
       return val;
   }

    // Verify that have a valid config line - can't be empty, can't start with #, must contain at least one =.
    bool Config::isLineValid(const string configLine) {
        bool isValid = false;
        if (trim(configLine).length() > 0) {
            if (configLine.substr(0,1) != "#") {
                if (configLine.find_first_of("=") != string::npos) {
                    isValid = true;
                }
            }
        }
        return isValid;
    }

    void Config::logValues() {
        cout << "Config values\n";
        for (auto it = configMap.cbegin(); it != configMap.cend(); ++it) {
            cout << "key: " << it->first << ", val: " << it->second << "\n";
        }
    }
}
