#include <iostream>
#include <math.h>
#include <numeric>

#include "global.h"
#include "dataCleaning.h"

using namespace std;

namespace gwDainese
{
    // Function for finding bad elements in a vector, then removing corresponding elements
    // in all the provided vectors.


    // Accepts low/high range of acceptable values, and 3 vectors.
    // Finds indexes of unacceptable values in first vector, then removes those
    // elements from it, and the elements from the same indexes in the other vectors.
    // May also be called with just two vectors.
    template<typename T1, typename T2, typename T3>
        tuple<vector<T1>, vector<T2>, vector<T3>> cleanVectors(T1 lowValue, T1 highValue, const vector<T1>& vec1,
                                        const vector<T2>& vec2, const vector<T3>& vec3) {

        auto badIndexes = GetBadIndexes(vec1, lowValue, highValue);
        auto cleanedVec1 = RemoveBadData(vec1, badIndexes);
        auto cleanedVec2 = RemoveBadData(vec2, badIndexes);
        vector<T3> cleanedVec3;
        if ( vec3.size() > 0) {
            cleanedVec3 = RemoveBadData(vec3, badIndexes);
            cout << "cleanVectors, vec3.size=" << vec3.size() << ", badIndexes.size=" << badIndexes.size()
                    << ", cleanedVec3.size=" << cleanedVec3.size() << "\n";
        }
        
        return make_tuple(cleanedVec1, cleanedVec2, cleanedVec3);
    }

    template<typename T1, typename T2>
        tuple<vector<T1>, vector<T2>> cleanVectors(T1 lowValue, T1 highValue, const vector<T1>& vec1,
                                        const vector<T2>& vec2) {
        vector<T1> vec3;
        auto tup = cleanVectors(lowValue, highValue, vec1, vec2, vec3);

        vector<T1> cleanedVec1;
        vector<T2> cleanedVec2;
        tie(cleanedVec1, cleanedVec2, ignore) = tup;

        return make_tuple(cleanedVec1, cleanedVec2);
    }

    // Accepts low/high range of acceptable values, to filter out other points.
    // I.e. returns indexes of values that are outside the given range.
    // Note that range is exclusive - if value is equal to low or high, it is 
    // considered a good value, and so not added to bad indexes.
    template<typename T> vector<size_t> GetBadIndexes(const vector<T>& inputData, T lowValue, T highValue){
        // Allocate a vector to store the indexes of the invalid points.
        size_t numInvalid = guessNumInvalid(inputData.size());
        vector<size_t> badIndexes;
        badIndexes.reserve(numInvalid);

        size_t i = 0;
        for (auto value : inputData) {
            if (value < lowValue || value > highValue) {
                badIndexes.push_back(i);
                //cout << "Decided that the following value is bad: " << value << "\n";
            }
            i++;
        }
        badIndexes.shrink_to_fit();
        cout << "numBadIndexes: " << badIndexes.size() << "\n";

        return badIndexes;
    }

    // Guesses that around 1 of 4 data points are invalid.
    size_t guessNumInvalid(const size_t numValid) {
        size_t numInvalid = (size_t) floor((double) numValid / 4);
        return numInvalid;
    }

    template<typename T> vector<T> RemoveBadData(const vector<T>& srcData, const vector<size_t>& badIndexes){
        vector<T> goodData(srcData.size() - badIndexes.size());
        size_t badIdx = 0;
        size_t srcIdx = 0;
        size_t goodIdx = 0;
        for (auto item : srcData) {
            // If this item is one that we should skip, skip it, otherwise copy it to the good data.
            if (badIdx < badIndexes.size() && srcIdx++ == badIndexes[badIdx]) {
                badIdx++;
            }
            else {
                goodData[goodIdx++] = item;
            }
        }

        return goodData;
    }

    // Want to clean up raw lap times before graphing, by removing invalid times.
    // Will start by removing numbers more than given multiple the average of all times.
    // First element (out lap) is usually way different, so not even including it when calculating average.
    pair<vector<double>, vector<double>> cleanLapsForGraphing(const vector<double>& lapTimes, 
                                                                const vector<double>& plotTimes) {

        // double avg = getAverage(lapTimes);
        
        vector<double> notFirst(lapTimes.begin() + 1, lapTimes.end());
        double avg2 = getAverage(notFirst);
        //cout << "average: " << avg << ", not including first: " << avg2 << "\n";

        double thresholdMultiple = myConfig.getValueDbl("lapTimeGraphCutoffMultiple");  // Multiple of average, over which to exclude times.
        double thresh = thresholdMultiple * avg2;
        vector<double> cleanLapTimes;
        vector<double> cleanPlotTimes;
        int i = 0;
        for (auto laptime: lapTimes) {
            if (laptime < thresh) {
                cleanLapTimes.push_back(laptime);
                cleanPlotTimes.push_back(plotTimes[i]);
            }
            i++;
        }

        return make_pair(cleanLapTimes, cleanPlotTimes);
    }

    double getAverage(const vector<double>& lapTimes) {
        auto count = lapTimes.size();
        return std::reduce(lapTimes.begin(), lapTimes.end())/count;
    }

    template vector<size_t> GetBadIndexes(const vector<double>&, double, double);
    template vector<size_t> GetBadIndexes(const vector<int>&, int, int);
    template vector<size_t> GetBadIndexes(const vector<unsigned short>&, unsigned short, unsigned short);

    template vector<unsigned short> RemoveBadData(const vector<unsigned short>&, const vector<size_t>&);
    template vector<int> RemoveBadData(const vector<int>&, const vector<size_t>&);
    template vector<double> RemoveBadData(const vector<double>&, const vector<size_t>&);
    template vector<float> RemoveBadData(const vector<float>&, const vector<size_t>&);


    template tuple<vector<double>, vector<double>, vector<double>> 
        cleanVectors(double, double, const vector<double>&, const vector<double>&, const vector<double>&);
    template tuple<vector<double>, vector<double>, vector<float>> 
        cleanVectors(double, double, const vector<double>&, const vector<double>&, const vector<float>&);
    template tuple<vector<double>, vector<double>, vector<unsigned short>> 
        cleanVectors(double, double, const vector<double>&, const vector<double>&, const vector<unsigned short>&);
    template tuple<vector<unsigned short>, vector<double>, vector<double>> 
        cleanVectors(unsigned short, unsigned short, const vector<unsigned short>&, const vector<double>&, 
                    const vector<double>&);
    template tuple<vector<double>, vector<double>> 
        cleanVectors(double, double, const vector<double>&, const vector<double>&);
}
