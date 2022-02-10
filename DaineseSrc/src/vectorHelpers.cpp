#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <utility>


#include "global.h"
#include "Plotting.h"
#include "timeHelpers.h"
#include "vectorHelpers.h"

using namespace std;

namespace gwDainese
{
    // Various helper functions for working with vectors - mostly just viewing.

    // Display contents of given vector (max first 200 elements), size and min/max values.
    template<typename T> void displayResults(const vector<T>& results, bool plotMe){

        cout << "Vector contents: ";
        size_t maxToDisplay = myConfig.getValueInt("vectorMaxElementsToDisplay");
        size_t i = 0;
        for (T x : results){
            cout << x << " ";
            if (++i >= maxToDisplay) break;
        }
        cout << "\n";
        
        const auto [min, max] = std::minmax_element(begin(results), end(results));
        cout << "Size: " << results.size() << "\n";
        cout << "Min: " << *min << ", Max: " << *max << "\n";
        if (plotMe) plotY(results);
    }

    template<typename T> void displayUniqueValues(const vector<T>& results) {
        vector<T> uniqueVals;
        cout << "Unique values: ";
        unique_copy(results.begin(), results.end(), std::back_inserter(uniqueVals));
        for (T x : uniqueVals) cout << x << " ";
        cout << "\n";
    }


    string getTime(tm myTime) {
        string rtnStr = getMonthNameFromNum(myTime.tm_mon) + " " + to_string(myTime.tm_mday) + ", ";
        rtnStr += to_string(myTime.tm_hour) + ":" + to_string(myTime.tm_min);
        rtnStr += ":" + to_string(myTime.tm_sec);

        return rtnStr;
    }

    void outputPairSideBySide(const vector<double>& lapTimes, const vector<tm>& startTimes) {
        //cout << setprecision(5) << fixed << "\n startTime lapTime";
        for (size_t i = 0; i < lapTimes.size(); i++) {
            cout << getTime(startTimes[i]) << ", " << lapTimes[i] << "\n";
        }
    }

    void outputPairSideBySide(const vector<double>& lapTimes, const vector<double>& plotTimes) {
        //cout << setprecision(5) << fixed << "\n startTime lapTime";
        for (size_t i = 0; i < lapTimes.size(); i++) {
            cout << plotTimes[i] << ", " << lapTimes[i] << "\n";
        }
    }

    template<typename T> void outputVector(const vector<T>& data){
        for (auto item : data)
            cout << item << ", ";
        cout << "\n";
    }

    template<typename T1, typename T2> void savePairToFile(const vector<T1>& data1, 
                                                            const vector<T2>& data2,
                                                            const string fileName){
        ofstream ost(fileName, ios::trunc);
        if (ost.is_open()) {
            ost << setprecision(8);
            for (size_t i = 0; i < data1.size(); i++) {
                ost << i << ", " << data1[i] << ", " << data2[i] << "\n";
            }
            ost.close();
        }
        else {
            cout << "can't open output file " << fileName << "\n";
        }
    }

    template void outputVector(const vector<double>&);
    template void outputVector(const vector<int>&);
    template void outputVector(const vector<short>&);
    template void outputVector(const vector<long>&);
    template void outputVector(const vector<float>&);
    template void outputVector(const vector<string>&);

    template void displayResults(const vector<double>&, bool);
    template void displayResults(const vector<float>&, bool);
    template void displayResults(const vector<unsigned short>&, bool);

    template void savePairToFile(const vector<double>&, const vector<double>&, const string);
}
