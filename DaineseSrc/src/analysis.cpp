#include <iomanip>
#include <iostream>
#include <math.h>

#include "analysis.h"
#include "Plotting.h"

using namespace std;

namespace gwDainese
{
    // Calculate lap times, based on trigger points. Calculates time based on times saved with each
    // lat/long measurement, which is done at 6.25 Hz.
    // Note that, if startOnly is false, function will calculate times for out and in laps - i.e. from
    // leaving pit lane to first time pass start/finish, and from last time across start/finish until pass
    // track exit.
    pair<vector<double>, vector<tm>> LapAnalysis::calcLapTimes(bool startOnly) {

        vector<double> laptimes;
        vector<tm> startTimes;
        size_t lastIndex = 0;
        vector<double> startLats;
        vector<double> startLongs;

        cout << "calcLapTimes, latitude.size=" << latitudes.size() << "\n";
        cout << "targets, latitude=" << startFinish.latitude << ", longitude=" << startFinish.longitude << "\n";
        bool passedEntrance = false;
        for (size_t i = 0; i < latitudes.size(); i++){
            // If using track entrance and exit, start by finding next time we go on track.
            if (not startOnly && not passedEntrance) {
                if (atTrackEntrance(i) ) {
                    passedEntrance = true;
                    startLats.push_back(latitudes[i]);
                    startLongs.push_back(longitudes[i]);
                    cout << "atTrackEntrance, lat=" << latitudes[i] << ", long=" << longitudes[i] << "\n";
                    lastIndex = i;
                    i += lapIndexJump;
                }
            }

            if (( startOnly || passedEntrance ) && atStartFinish(i)) {
                double thisTime = GetTimeDifference(i, lastIndex, latFrequency, timeData);

                // When calculating lap times based on measured time, go negative when
                // cross days. No point saving such times.
                if (thisTime > 0) {
                    laptimes.push_back(thisTime);
                    cout << lastIndex << ", " << i << ", " << latitudes[i] << ", " 
                        << longitudes[i] << ", " << thisTime << "\n";
                    startLats.push_back(latitudes[i]);
                    startLongs.push_back(longitudes[i]);
                    startTimes.push_back(timeData.getDateTime(lastIndex, latFrequency));
                }
                lastIndex = i;
                i += lapIndexJump;
            }
            if ( (not startOnly) && passedEntrance && atTrackExit(i)) {
                // We've hit a track exit. End last lap.
                double thisTime = GetTimeDifference(i, lastIndex, latFrequency, timeData);
                laptimes.push_back(thisTime);
                startTimes.push_back(timeData.getDateTime(lastIndex, latFrequency));
                cout << "atTrackExit, lat=" << latitudes[i] << ", long=" << longitudes[i] << "\n";
                lastIndex = i;
                i += lapIndexJump;
                passedEntrance = false;
            }
        }
        //plot2XY(longitudes, latitudes, startLongs, startLats);

        return make_pair(laptimes, startTimes);
    }

    // Return a vector, parallel to lat/long, containing 400 if point is at pit exit,
    // 500 if at start/finish, 600 if at pit entrance, 300 if hit both pit exit and start/finish,
    // 200 if hit pit entrance and start/finish and 0 elsewhere. Intended to be used
    // to view where these points are on lat/long graph, to troubleshoot when the points
    // aren't being detected as desired.
    vector<unsigned short> LapAnalysis::getTrackPOIs() {
        vector<unsigned short> pois (latitudes.size(), 0);
        unsigned short poiValue = 0;
        bool atPitOut = false, atStart = false, atPitIn = false;
        int numPitOut = 0, numStart = 0, numPitIn = 0;
        for (size_t i = 0; i < latitudes.size(); i++){
            atPitOut = atTrackEntrance(i);
            atStart = atStartFinish(i);
            atPitIn = atTrackExit(i);
            
            if (atPitOut && atStart) {
                poiValue = 300;
                numPitOut++;
                numStart++;
            }
            else if (atPitOut) {
                poiValue = 400;
                numPitOut++;
            }
            else if (atPitIn && atStart) {
                poiValue = 200;
                numStart++;
                numPitIn++;
            }
            else if (atPitIn) {
                poiValue = 600;
                numPitIn++;
            }
            else if (atStart) {
                poiValue = 500;
                numStart++;
            }
            else poiValue = 0;
            
            pois[i] = poiValue;
            
            // If we found something, skip ahead, so don't count it twice.
            if (poiValue > 0) {
                i += lapIndexJump;
            }
        }
        cout << "\ngetTrackPOIs, numPitOut=" << numPitOut << ", numStart=" << numStart << ", numPitIn=" << numPitIn << "\n";
        return pois;
    }

    bool LapAnalysis::atStartFinish(size_t index) {
        if ( isInRange(latitudes[index], startFinish.latitude, latLongRange) && 
            isInRange(longitudes[index], startFinish.longitude, latLongRange)) {
                return true;
            }
        else return false;
    }

    bool LapAnalysis::atTrackEntrance(size_t index) {
        if ( isInRange(latitudes[index], trackEntrance.latitude, latLongRange) && 
            isInRange(longitudes[index], trackEntrance.longitude, latLongRange)) {
                return true;
            }
        else return false;
    }

    bool LapAnalysis::atTrackExit(size_t latsIndex) {
        for (size_t exitIndex = 0; exitIndex < trackExits.size(); exitIndex++) {
            if ( isInRange(latitudes[latsIndex], trackExits[exitIndex].latitude, latLongRange) && 
                isInRange(longitudes[latsIndex], trackExits[exitIndex].longitude, latLongRange)) {
                    return true;
                }
        }
        
        return false;
    }

    // Function to check if given point is within range of given target.
    bool LapAnalysis::isInRange(double val, double target, double tolerance) {
        if ( val > (target - tolerance) && val < (target + tolerance)) return true;
        else return false;
    }

    double LapAnalysis::GetTimeDifference(size_t currIndex, size_t lastIndex, double latFrequency) {
                    
        size_t index_diff = currIndex - lastIndex;
        double thisTime = index_diff / latFrequency;
        
        return thisTime;
    }


    double LapAnalysis::GetTimeDifference(size_t currIndex, size_t lastIndex, 
                            double latFrequency, TimeData timeData) {
                    
        double prevSecs = timeData.getSeconds(lastIndex, latFrequency);
        double currSecs = timeData.getSeconds(currIndex, latFrequency);
        double thisTime = currSecs - prevSecs;
        cout << "GetTimeDifference, lastIndex=" << lastIndex << ", currIndex=" << currIndex
            << ", prevSecs=" << prevSecs << ", currSecs=" << currSecs << ", thisTime=" 
            << thisTime << "\n";
        
        return thisTime;
    }
}
