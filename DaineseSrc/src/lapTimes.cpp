#include <iostream>

#include "lapTimes.h"
#include "dataCleaning.h"
#include "latLongHelpers.h"
#include "Plotting.h"
#include "vectorHelpers.h"
#include "timeHelpers.h"

using namespace std;

namespace gwDainese
{

    // Entry point for calculating lap times from given base file name.
    void lapTimes(string fileName, string sampleDir) {
        string baseFilename = fileName;
        TimeData timeData = timeDataFactory(baseFilename, sampleDir);
        auto latLong = readLatLong(baseFilename, sampleDir);
        LapAnalysis myAnalyzer = createAnalyzer(latLong.first, latLong.second, timeData);

        auto [lapTimes, startTimes] = myAnalyzer.calcLapTimes(false);

        cout << "\nCalculated lap times:\n";
        outputPairSideBySide(lapTimes, startTimes);
        vector<double> plotTimes = getPlotTimes(startTimes);
        auto [cleanLapTimes, cleanPlotTimes] = cleanLapsForGraphing(lapTimes, plotTimes);
        //outputPairSideBySide(cleanLapTimes, cleanPlotTimes);
        //plotXY(cleanPlotTimes, cleanLapTimes);
        barChart(cleanPlotTimes, cleanLapTimes);
    }

    LapAnalysis createAnalyzer(vector<double>& latitudes, 
                                vector<double>& longitudes, 
                                TimeData timeData) {
        
        latLongPoint startFinish (myConfig.getValueDbl("startFinishLat"), myConfig.getValueDbl("startFinishLong"));
        latLongPoint pitOut (myConfig.getValueDbl("trackEntranceLat"), myConfig.getValueDbl("trackEntranceLong"));
        latLongPoint exit1 (myConfig.getValueDbl("trackExit1Lat"), myConfig.getValueDbl("trackExit1Long"));
        latLongPoint exit2 (myConfig.getValueDbl("trackExit2Lat"), myConfig.getValueDbl("trackExit2Long"));

        vector<latLongPoint> emptyVec;
        vector<latLongPoint> exits;
        if ( exit2.latitude != 0 && exit2.longitude != 0) {
            exits = {exit1, exit2};
        }
        else {
            exits = {exit1};
        }
        TrackData myTrack (startFinish, pitOut, exits);
        FileTypesData myFileData;
        auto latFreq = myFileData.GetFrequency(DaineseDataTypes::latitude);
        LapAnalysis myAnalyzer (latitudes, longitudes, myTrack, timeData, latFreq);

        return myAnalyzer;
    }

    // Graph points of interest on track - the pit exit, pit entrance and start/finish line hits.
    // Hopefully useful for figuring out why sometimes not detecting them.
    void graphTrackPOIs(string baseFileName, string sampleDir) {

        TimeData timeData = timeDataFactory(baseFileName, sampleDir);
        auto latLong = readLatLong(baseFileName, sampleDir);
        LapAnalysis myAnalyzer = createAnalyzer(latLong.first, latLong.second, timeData);
        auto myPois = myAnalyzer.getTrackPOIs();
        graphAgainstLatLong(myPois, baseFileName, sampleDir);
    }

    // Create alternative x-values for plotting lap times, to better separate sessions, graphically
    // indicate gaps between laps. If data includes multiple days, plot each day's data consecutively.
    vector<double> getPlotTimes(const vector<tm>& startTimes) {
        // Calculate seconds since start of day for each element of startTimes.
        // Add number of seconds in day times counter for day
        vector<double> plotTimes (startTimes.size(), 0);
        double plotTime;
        double dayCntr = 0;
        double betweenDayGap = 60 * 60 * 8;
        tm lastDay = startTimes[0];
        size_t i = 0;

        for (tm thisTm: startTimes) {
            plotTime = getTotalSecsFromTime(thisTm);
            if (not areDaysSame(lastDay, thisTm)) {
                dayCntr++;
                lastDay = thisTm;
            }
            plotTime += dayCntr * betweenDayGap;
            //cout << "dayCntr: " << dayCntr << ", plotTime=" << plotTime << "\n";
            plotTimes[i] = plotTime;
            i++;
        }

        return plotTimes;
    }
}
