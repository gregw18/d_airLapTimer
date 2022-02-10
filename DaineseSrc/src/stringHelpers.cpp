#include "stringHelpers.h"

using namespace std;

namespace gwDainese
{
    pair<string, string> split(const string configStr) {
        string key = "";
        string val = "";

        size_t splitPos = configStr.find_first_of("=");
        if (splitPos != string::npos) {
            key = trim(configStr.substr(0, splitPos));
            val = trim(configStr.substr(splitPos + 1));
        }


        return make_pair(key, val);
    }

    // Remove leading and trailing spaces from given string.
    string trim(const string origStr) {
        string rtnStr = "";
        const auto strStart = origStr.find_first_not_of(" ");
        if (strStart != string::npos) {
            const auto strEnd = origStr.find_last_not_of(" ");
            rtnStr = origStr.substr(strStart, strEnd - strStart + 1);
        }

        return rtnStr;
    }
}
