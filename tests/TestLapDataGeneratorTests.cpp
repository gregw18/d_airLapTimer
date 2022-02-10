#include <cmath>
#include <vector>

#include "gtest/gtest.h"

#include "analysis.h"
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

double getAvgLapTime(vector<double> lats, vector<double> longs, 
                    vector<unsigned short> secsData, vector<unsigned short> hoursData, int numLaps);
int getElapsedMins(vector<unsigned short> hoursData, size_t startIdx, size_t endIdx);
double getPercentDiff(double targetTime, double calculatedTime);

latLongPoint startFinish (22, -33);
latLongPoint pitOut (23, -33);
latLongPoint exit1 (21, -33);
latLongPoint exit2 (22, -35);
vector<latLongPoint> emptyVec;
vector<latLongPoint> exits ({exit1, exit2});
TrackData track1 (startFinish, pitOut, exits);

// Asking for a too short lap time, so won't get full lap.
TEST(LapGeneratorTests, LessThanOneLap) {
    double targetLapTime = 1;
    short targetNumLaps = 1;
    TestLapDataGenerator myGen;
    auto [lats, longs, daysData, hoursData, secsData] = myGen.getTestLapData(targetLapTime, targetNumLaps, track1);
    size_t sizeDiff = std::abs ((long) (lats.size() - daysData.size() * 2));
    cout << "lats.size: " << lats.size() << ", days.size: " << daysData.size() << "\n";
    //double calcLapTime = secsData[secsData.size()-1] - secsData[0];
    double avgLapTime = getAvgLapTime(lats, longs, secsData, hoursData, targetNumLaps);
    cout << "avgLapTime: " << avgLapTime << "\n";

    ASSERT_TRUE(sizeDiff <= 1);
    ASSERT_EQ(0, avgLapTime);
}

TEST(LapGeneratorTests, LessThanOneLapLonger) {
    double targetLapTime = 3;
    short targetNumLaps = 1;
    //cout << "lessThanOne 1\n";
    TestLapDataGenerator myGen;
    //cout << "lessThanOne 2\n";
    auto [lats, longs, daysData, hoursData, secsData] = myGen.getTestLapData(targetLapTime, targetNumLaps, track1);
    //cout << "lessThanOne 3\n";
    size_t sizeDiff = std::abs ((long) (lats.size() - daysData.size() * 2));
    //cout << "lessThanOne 4\n";
    //double calcLapTime = secsData[secsData.size()-1] - secsData[0];
    double avgLapTime = getAvgLapTime(lats, longs, secsData, hoursData, targetNumLaps);
    //cout << "lessThanOne 5\n";

    ASSERT_TRUE(sizeDiff <= 1);
    //cout << "lessThanOne 6\n";
    ASSERT_EQ(targetLapTime, avgLapTime);
    //cout << "lessThanOne 7\n";
}

TEST(LapGeneratorTests, OneShortLap) {
    double targetLapTime = 10;
    short targetNumLaps = 1;
    TestLapDataGenerator myGen;
    auto [lats, longs, daysData, hoursData, secsData] = myGen.getTestLapData(targetLapTime, targetNumLaps, track1);

    size_t sizeDiff = std::abs ((long) (lats.size() - daysData.size() * 2));
    cout << "One Short Lap, lats-size=" << lats.size() << ", daysData.size=" << daysData.size()
        << ", sizeDiff=" << sizeDiff << "\n";
    //double calcLapTime = secsData[secsData.size()-1] - secsData[0];
    double avgLapTime = getAvgLapTime(lats, longs, secsData, hoursData, targetNumLaps);

    ASSERT_TRUE(sizeDiff <= 1);
    ASSERT_EQ(targetLapTime, avgLapTime);
}


TEST(LapGeneratorTests, TwoShortLaps) {
    double targetLapTime = 10;
    short targetNumLaps = 2;
    TestLapDataGenerator myGen;
    auto [lats, longs, daysData, hoursData, secsData] = myGen.getTestLapData(targetLapTime, targetNumLaps, track1);

    size_t sizeDiff = std::abs ((long) (lats.size() - daysData.size() * 2));
    cout << "One Short Lap, lats-size=" << lats.size() << ", daysData.size=" << daysData.size()
        << ", sizeDiff=" << sizeDiff << "\n";
    //double calcLapTime = secsData[secsData.size()-1] - secsData[0];
    double avgLapTime = getAvgLapTime(lats, longs, secsData, hoursData, targetNumLaps);
    //double lapDiffPercent = abs((targetLapTime - avgLapTime)/targetLapTime) * 100;

    ASSERT_TRUE(sizeDiff <= 1 * targetNumLaps);
    // Verify that calculate lap time is off by less than 1%.
    ASSERT_TRUE(getPercentDiff(targetLapTime, avgLapTime) < 1);
}

TEST(LapGeneratorTests, FourShortLaps) {
    double targetLapTime = 10;
    short targetNumLaps = 4;
    TestLapDataGenerator myGen;
    auto [lats, longs, daysData, hoursData, secsData] = myGen.getTestLapData(targetLapTime, targetNumLaps, track1);

    size_t sizeDiff = std::abs ((long) (lats.size() - daysData.size() * 2));
    cout << "One Short Lap, lats-size=" << lats.size() << ", daysData.size=" << daysData.size()
        << ", sizeDiff=" << sizeDiff << "\n";
    //double calcLapTime = secsData[secsData.size()-1] - secsData[0];
    double avgLapTime = getAvgLapTime(lats, longs, secsData, hoursData, targetNumLaps);
    //double lapDiffPercent = abs((targetLapTime - avgLapTime)/targetLapTime) * 100;

    ASSERT_TRUE(sizeDiff <= 1 * targetNumLaps);
    // Verify that calculate lap time is off by less than 1%.
    ASSERT_TRUE(getPercentDiff(targetLapTime, avgLapTime) < 1);
}

TEST(LapGeneratorTests, OneLongLap) {
    double targetLapTime = 80;
    short targetNumLaps = 1;
    TestLapDataGenerator myGen;
    auto [lats, longs, daysData, hoursData, secsData] = myGen.getTestLapData(targetLapTime, targetNumLaps, track1);

    size_t sizeDiff = std::abs ((long) (lats.size() - daysData.size() * 2));
    cout << "One Short Lap, lats-size=" << lats.size() << ", daysData.size=" << daysData.size()
        << ", sizeDiff=" << sizeDiff << "\n";
    //double calcLapTime = secsData[secsData.size()-1] - secsData[0];
    double avgLapTime = getAvgLapTime(lats, longs, secsData, hoursData, targetNumLaps);
    // double lapDiffPercent = abs((targetLapTime - avgLapTime)/targetLapTime) * 100;

    ASSERT_TRUE(sizeDiff <= 1 * targetNumLaps);
    ASSERT_TRUE(getPercentDiff(targetLapTime, avgLapTime) < 1);
}

TEST(LapGeneratorTests, TenLongLaps) {
    double targetLapTime = 182;
    short targetNumLaps = 10;
    TestLapDataGenerator myGen;
    auto [lats, longs, daysData, hoursData, secsData] = myGen.getTestLapData(targetLapTime, targetNumLaps, track1);

    size_t sizeDiff = std::abs ((long) (lats.size() - daysData.size() * 2));
    cout << "One Short Lap, lats-size=" << lats.size() << ", daysData.size=" << daysData.size()
        << ", sizeDiff=" << sizeDiff << "\n";
    //double calcLapTime = secsData[secsData.size()-1] - secsData[0];
    double avgLapTime = getAvgLapTime(lats, longs, secsData, hoursData, targetNumLaps);
    // double lapDiffPercent = abs((targetLapTime - avgLapTime)/targetLapTime) * 100;

    ASSERT_TRUE(sizeDiff <= 1 * targetNumLaps);
    ASSERT_TRUE(getPercentDiff(targetLapTime, avgLapTime) < 1);
}

TEST(LapGeneratorTests, OneReallyLongLap) {
    double targetLapTime = 8000;
    short targetNumLaps = 1;
    TestLapDataGenerator myGen;
    auto [lats, longs, daysData, hoursData, secsData] = myGen.getTestLapData(targetLapTime, targetNumLaps, track1);

    size_t sizeDiff = std::abs ((long) (lats.size() - daysData.size() * 2));
    cout << "One Short Lap, lats-size=" << lats.size() << ", daysData.size=" << daysData.size()
        << ", sizeDiff=" << sizeDiff << "\n";
    //double calcLapTime = secsData[secsData.size()-1] - secsData[0];
    double avgLapTime = getAvgLapTime(lats, longs, secsData, hoursData, targetNumLaps);
    // double lapDiffPercent = abs((targetLapTime - avgLapTime)/targetLapTime) * 100;

    ASSERT_TRUE(sizeDiff <= 1 * targetNumLaps);
    ASSERT_TRUE(getPercentDiff(targetLapTime, avgLapTime) < 1);
}


// Need to get total time from first start/finish to last start/finish, then divide by expected
// number of laps. Note that start/finish point is known, accurate and precies, because the same
// value is used by the generator.
double getAvgLapTime(vector<double> lats, vector<double> longs,
                    vector<unsigned short> secsData, vector<unsigned short> hoursData, int numLaps) {
    /* size_t startIdx = 27/2;
    size_t endIdx = secsData.size() - startIdx;
    cout << "getAvgLapTime 1, secsData.size=" << secsData.size() << "startIdx=" << startIdx
        << ", endIdx=" << endIdx << "\n";
    short startTime = secsData[startIdx];
    cout << "getAvgLapTime 2, secs[0] = " << secsData[startIdx] << "-" << secsData[endIdx] << " \n";
    short endTime = secsData[endIdx];
    cout << "getAvgLapTime 3\n";
    int mins = getElapsedMins(hoursData, startIdx, endIdx);
    cout << "getAvgLapTime 4\n";
    double totalTime = mins * 60 + (double) (endTime - startTime) / 100;
    */

   size_t startIdx = 0, endIdx = 0;
   for (size_t i = 0; i < secsData.size(); i++){
       if (lats[i * 2] == startFinish.latitude && longs[i * 2] == startFinish.longitude) {
           // Note that start lat/long is emitted twice, so need to skip twice when hit it.
           if (startIdx == 0) {
               startIdx = i;
               i++;
           }
           else {
               endIdx = i;
               i++;
           }
       }
   }

    cout << "getAvgLapTime 1, secsData.size=" << secsData.size() << ", startIdx=" << startIdx
        << ", endIdx=" << endIdx << "\n";
    double avgTime = 0;
    if ( (startIdx > 0) && (endIdx > 0)) {
        short startTime = secsData[startIdx];
        cout << "getAvgLapTime 2, secs[0] = " << secsData[startIdx] << "-" << secsData[endIdx] << " \n";
        short endTime = secsData[endIdx];
        cout << "getAvgLapTime 3\n";
        int mins = getElapsedMins(hoursData, startIdx, endIdx);
        cout << "getAvgLapTime 4\n";
        double totalTime = mins * 60 + (double) (endTime - startTime) / 100;

        avgTime = totalTime / numLaps;
        cout << "getAvgLapTime, startTime=" << startTime << ", endTime=" << endTime
            << ", totalTime=" << totalTime << ", avgTime=" << avgTime << "\n";
    }
    cout << "getAvgLapTime, avgTime=" << avgTime << "\n";

    return avgTime;
}

/*double getAvgLapTime(vector<unsigned short> secsData, vector<unsigned short> hoursData, int numLaps) {
    cout << "getAvgLapTime 1, secsData.size=" << secsData.size() << "\n";
    short startTime = secsData[0];
    cout << "getAvgLapTime 2, secs[0] = " << secsData[0] << "-" << secsData[secsData.size()-1] << " \n";
    short endTime = secsData[secsData.size()-1];
    cout << "getAvgLapTime 3\n";
    int mins = getElapsedMins(hoursData);
    cout << "getAvgLapTime 4\n";
    double totalTime = mins * 60 + (double) (endTime - startTime) / 100;
    double avgTime = totalTime / numLaps;
    cout << "getAvgLapTime, startTime=" << startTime << ", endTime=" << endTime
        << ", totalTime=" << totalTime << ", avgTime=" << avgTime << "\n";

    return avgTime;
}
*/

int getMinutes(unsigned short hhmm) {
    int hrs = (int) floor(hhmm/100);
    int mins = hhmm % 100;
    
    return (hrs * 60 + mins);
}

int getElapsedMins(vector<unsigned short> hoursData, size_t startIdx, size_t endIdx) {
    // Calculate difference between minutes at end of vector and minutes at beginning.
    // Remember that hours, minutes are stored in one short as HHMM. Thus, converting each
    // measured to minutes, subtracting those to get difference.
    int startMins = getMinutes(hoursData[startIdx]);
    int endMins = getMinutes(hoursData[endIdx]);
    cout << "getElapsedMins [start]=" << hoursData[startIdx] << ", [end]=" << hoursData[endIdx]
        << ", startMins=" << startMins << ", endMins=" << endMins
        << ", elapsed=" << endMins - startMins << "\n";

    return (endMins - startMins);
}
