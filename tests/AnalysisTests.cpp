#include <cmath>
#include <vector>

#include "gtest/gtest.h"

#include "analysis.h"
#include "fileRead.h"
#include "TestLapDataGenerator.h"

using namespace std;
using namespace gwDainese;

// To test (bearing in mind my target lat/long are, threshold is 0.0001 and skip 50 points after
// finding start/finish)
// 1. 5 elements, expect nothing back.
// 2. Hit start/finish (exact) once, expect nothing back.
// 3. Hit start/finish (exact) twice, get a lap time.
// 4. Twice, approximate, with several other approximates, get a lap time.
// 5. Four times, approximate, expect three lap times.
// 6. Three times, with bunch of zeroes in between - simulated session gap.
// 7. Three times, < 60 s lap.
// 8. Four times, > 120 s lap.

double getPercentDiff(double targetTime, double calculatedTime);

string cfgFile = "config.txt";
Config gwDainese::myConfig;

// Helper to generate test data with desired lap time, number of laps, call function
// to calculate lap times from that data, and return them for checking.
std::pair<std::vector<double>, std::vector<tm>> 
    SetUpData(const double laptime, const int numLaps) {
        vector<string> cfgStrings = readTextFileToVector(cfgFile);
        myConfig.init(cfgStrings);

        latLongPoint startFinish (22, -33);
        latLongPoint pitOut (23, -33);
        latLongPoint exit1 (21, -33);
        latLongPoint exit2 (22, -35);
        vector<latLongPoint> exits ({exit1, exit2});
        TrackData track1 (startFinish, pitOut, exits);

        TestLapDataGenerator myGen;
        auto [lats, longs, daysData, hoursData, secsData] = myGen.getTestLapData(laptime, numLaps, track1);

        FileTypesData filesData;
        TimeData timeData (daysData, hoursData, secsData, filesData);

        double latFreq = filesData.GetFrequency(DaineseDataTypes::latitude);
        vector<latLongPoint> emptyVec;
        //TrackData myTrack (myGen.startFinish, latLongPoint(), emptyVec);
        LapAnalysis myAnalyzer(lats, longs, track1, timeData, latFreq);

        auto res = myAnalyzer.calcLapTimes(false);

        return res;
}


// Asking for one lap, but with a time that is less than the startFinish jump. Generator gives
// bunch of times at one location, and so no complete lap.
TEST(AnalysisTest, SitInPitLaneNoTimes){
    double lapTime = 1;
    int numLaps = 1;
    auto [lapTimes, startTimes] = SetUpData(lapTime, numLaps);
    ASSERT_EQ(0, lapTimes.size());
}

// Zero full laps. Get just out lap, no valid lap times.
TEST(AnalysisTests, OutLapNoTimes) {
    double lapTime = 6;
    int numFullLaps = -1;
    auto [lapTimes, startTimes] = SetUpData(lapTime, numFullLaps);
    ASSERT_EQ(1, lapTimes.size());
}

// Zero full laps. Get just out and in laps, no valid lap times.
TEST(AnalysisTests, OutAndInLapsNoTimes) {
    double lapTime = 6;
    int numFullLaps = 0;
    auto [lapTimes, startTimes] = SetUpData(lapTime, numFullLaps);
    ASSERT_EQ(2, lapTimes.size());
}

// One full lap - one out lap, one flying lap, one in lap.
TEST(AnalysisTests, OneLapOneTime) {
    double lapTime = 66;
    int numLaps = 1;
    auto [lapTimes, startTimes] = SetUpData(lapTime, numLaps);

    ASSERT_EQ(3, lapTimes.size());
    ASSERT_TRUE(getPercentDiff(66, lapTimes[1]) < 1);
}


// Two full laps - one out lap, two flying laps, one in lap.
TEST(AnalysisTests, TwoLapsTwoTimes) {
    double lapTime = 66;
    int numLaps = 2;
    auto [lapTimes, startTimes] = SetUpData(lapTime, numLaps);

    ASSERT_EQ(4, lapTimes.size());
    ASSERT_TRUE(getPercentDiff(66, lapTimes[1]) < 1);
}

// Three full laps, three valid lap times.
TEST(AnalysisTests, threeLapsThreeTimes) {
    double lapTime = 120;
    int numLaps = 3;
    auto [lapTimes, startTimes] = SetUpData(lapTime, numLaps);

    ASSERT_EQ(5, lapTimes.size());
    ASSERT_EQ(120, lapTimes[1]);
    ASSERT_EQ(120, lapTimes[2]);
    ASSERT_EQ(120, lapTimes[3]);
}

