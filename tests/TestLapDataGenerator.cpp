#include <algorithm>
#include <iostream>
#include <iomanip>
#include <math.h>
#include <vector>

#include "TestLapDataGenerator.h"

using namespace std;
using namespace gwDainese;


void TestLapDataGenerator::outputPair(pair<vector<double>, vector<double>> latLongs) {
    cout << "\n lats ";
    outputVector(latLongs.first);

    cout << "\n longs ";
    outputVector(latLongs.second);
}

void TestLapDataGenerator::outputPairSideBySide(pair<vector<double>, vector<double>> latLongs) {
    cout << setprecision(5) << fixed << "\n lats  longs";
    for (size_t i = 0; i < latLongs.first.size(); i++) {
        cout << latLongs.first[i] << ", " << latLongs.second[i] << "\n";
    }
}

template<typename T> void TestLapDataGenerator::outputVector(vector<T> myNums) {
    cout << "Vector: " << setprecision(8);
    for (auto num : myNums) cout << num << ", ";
    cout << "\n";
    
    const auto [min, max] = minmax_element(begin(myNums), end(myNums));
    cout << "Size: " << myNums.size() << "\n";
    cout << "Min: " << *min << ", Max: " << *max << "\n";
}


// Generate requested number of laps at target lap time.
// Note that numFullLaps refers to laps that start and finish at start/finish, not
// in and out laps. For testing, can pass in 0 for numFullLaps and will get in lap and out
// lap but no full laps. -1 will generate out lap only.
// Expects laptime in seconds.
//pair<vector<double>, vector<double>> getTestLapData(const double laptime, const int numLaps) {
tuple<vector<double>, vector<double>, vector<unsigned short>, vector<unsigned short>, vector<unsigned short>> 
TestLapDataGenerator::getTestLapData(const double lapTime, const int numFullLaps, TrackData myTrack) {
    parseTrackData(myTrack);
    setNumOverUnderPoints(lapTime);
    setLatLongInc();
    setTimeInc(lapTime, numFullLaps);
    clearOutputData();

    if (numUnderPoints == 0) {
        // If target lap time is too short for a full lap, add bunch of points in one location.
        addDegenerateLap();
    }
    else {
        addLaps(lapTime, numFullLaps);
    }
    //vector<double> lats = getPoints(laptime, numLaps, startFinish.latitude, numOverPoints, underPoints);
    //vector<double> longs = getPoints(laptime, numLaps, startFinish.longitude, overPoints, underPoints);
    
    //auto [daysData, hoursData, secsData] = getTimes(laptime, numLaps, overPoints, underPoints);
    // cout << "secsData ";
    // outputVector(secsData);
    // cout << "hoursData ";
    // outputVector(hoursData);

    return make_tuple(oLatitudes, oLongitudes, oDaysData, oHoursData, oSecsData);
}

// Calculate how many points (entries in the lat/long vectors) will be after start/finish
// and how many will be before, bearing in mind that will have two points at start/finish
// and startFinishJump points directly after.
void TestLapDataGenerator::setNumOverUnderPoints(const double laptime) {
    //size_t overPoints, underPoints;

    size_t pointsPerLap = (size_t) ceil(laptime * latReadRate);
    if (pointsPerLap <= startFinishJump) {
        numOverPoints = pointsPerLap;
        numUnderPoints = 0;
    }
    else {
        numOverPoints = (size_t) ceil((pointsPerLap - startFinishJump) / 2);
        
        // Correct for any rounding errors - want exact lap time.
        numUnderPoints = (size_t) (laptime * latReadRate - 2 - startFinishJump - numOverPoints);
        cout << "\nmore than jump, overPoints=" << numOverPoints 
            << ", underPoints=" << numUnderPoints << "\n";
    }

    //return make_pair(overPoints, underPoints);
}

// Return vector of points for either lat or long - whichever startLine point we're given,
// calculated based on requested lap time, number of laps and position of start/finish line.
// Each lap starts just past start/finish, ends with "jump" after it.
vector<double> TestLapDataGenerator::getPoints(const double laptime, const int numFullLaps, 
                        const double startLine, const size_t overPoints,
                        const size_t underPoints) {
    vector<double> points;

    double increment = getIncrement(startLine);
    if (underPoints == 0) {
        // If less than start/finish, just output a bunch of points just past the start/finish.
        cout << "\nless than jump, overPoints=" << overPoints << "\n";
        points.insert(points.begin(), overPoints, startLine + increment);
    }
    else {
        if (startLine < 0) increment = 0 - increment;

        for (int i = 0; i < numFullLaps; i++) {
            points.insert(points.end(), overPoints, startLine + increment);
            points.insert(points.end(), underPoints - 2, startLine - increment);
            points.insert(points.end(), 2, startLine);
            points.insert(points.end(), startFinishJump, startLine + (increment / 4));
        }
    }
    
    return points;
}

// Calculate value to add to start/finish, for non-start/finish points.
// Want magnitude of this value + start/finish to be greater than start/finish,
// so making it negative if start/finish is negative.
double TestLapDataGenerator::getIncrement(const double startLine){
    double increment = 6 * latLongThresh;
    if (startLine < 0) increment = 0 - increment;
    return increment;
}

// Create vectors with HHMM and SSHH data, for requested parameters.
// Also returns vector for day - but currently just hardcoded.
tuple<vector<unsigned short>, vector<unsigned short>, vector<unsigned short>> 
    TestLapDataGenerator::getTimes(
                                        const double laptime, 
                                        const int numFullLaps, 
                                        const size_t overPoints, 
                                        const size_t underPoints) {
    // Convert given number of data points to correct frequency for clock measurements.
    size_t divisor = (size_t) (latReadRate / clockReadRate);
    size_t tmOverPoints = overPoints / divisor;
    size_t tmUnderPoints = underPoints / divisor;

    size_t pointsPerLap = tmOverPoints + tmUnderPoints + (2 + startFinishJump) / divisor;
    if (tmUnderPoints == 0) {
        // If no under points, just output overpoints.
        pointsPerLap = tmOverPoints;
    }

    // Calculate what time between measurements should be. Am assuming constant speed around
    // entire lap, which is entirely unrealistic, but should give accurate times.
    double inc = laptime / pointsPerLap;
    
    // If only one lap, if lap is short, can get lap difference because start at zero at time zero.
    if (numFullLaps < 2) inc = laptime / (pointsPerLap - 1);
    cout << "getTimes, inc=" << inc << ", tmOverPoints=" << tmOverPoints
        << ", tmUnderPoints=" << tmUnderPoints << ", pointsPerLap=" << pointsPerLap << "\n";
    tuple timeData = generatePointTimes(inc, numFullLaps, pointsPerLap);

    //return make_tuple(daysData, hoursData, secsData);
    return timeData;
}

// Create vectors with HHMM and SSHH data, for requested parameters.
// Also returns vector for day - but currently just hardcoded.
tuple<vector<unsigned short>, vector<unsigned short>, vector<unsigned short>> 
    TestLapDataGenerator::generatePointTimes(
                                        const double incTime, 
                                        const int numFullLaps, 
                                        const size_t pointsPerLap) {
    vector<unsigned short> daysData;
    vector<unsigned short> hoursData;
    vector<unsigned short> secsData;

    unsigned short currHr = startHr;
    unsigned short currMin = startMin;
    double currSec = startSec;
    unsigned short fmtHr = formatHour(currHr, currMin);
    // Must have at least one complete lap.
    for (int lap = 0; lap < numFullLaps; lap++) {
        for (int pt = 0; pt < pointsPerLap; pt++) {
            daysData.push_back(startDay);
            hoursData.push_back(fmtHr);
            secsData.push_back(formatSec(currSec));
            currSec += incTime;
            if (currSec >= 60) {
                currMin ++;
                if (currMin > 59) {
                    currMin = 0;
                    currHr++;
                }
                fmtHr = formatHour(currHr, currMin);
                currSec -= 60;
            }

        }
    }

    return make_tuple(daysData, hoursData, secsData);
}

// Takes hour (24 hr format) and minute, converts to HHMM.
short TestLapDataGenerator::formatHour(const short hour, const short min) {
    short formatted = hour * 100 + min;
    return formatted;
}

// Takes seconds, including number of decimal places, returns SSHH, where HH is hundredths
// of a second.
short TestLapDataGenerator::formatSec(const double sec) {
    double rounded = round(sec * 100);
    short formatted = (short) rounded;

    return formatted;
}

// Testing that point generator is giving what I want. Counts number of items from first instance
// of given target to next instance, skipping startFinishJump points after find first.
vector<size_t> TestLapDataGenerator::GetConsecutiveGaps(const vector<double>& points, double target) {
    vector<size_t> gaps;
    size_t lastIndex = 0;
    for (size_t i = 0; i < points.size(); i++) {
        if (points[i] == target){
            gaps.push_back(i - lastIndex);
            lastIndex = i;
            i += startFinishJump;
        }
        else {
            i++;
        }
    }
    return gaps;
}

void TestLapDataGenerator::clearOutputData() {
    currHr = startHr;
    currMin = startMin;
    currSec = startSec;
    oLatitudes.clear();
    oLongitudes.clear();
    oDaysData.clear();
    oHoursData.clear();
    oSecsData.clear();
}

void TestLapDataGenerator::parseTrackData(TrackData myTrack) {
    startFinishPoint = myTrack.startFinishPoint;
    pitOutPoint = myTrack.trackEntrancePoint;
    trackExitPoints = myTrack.trackExitPoints;
}

void TestLapDataGenerator::setLatLongInc() {
    latInc = getIncrement(startFinishPoint.latitude);
    longInc = getIncrement(startFinishPoint.longitude);
}

void TestLapDataGenerator::setTimeInc(double lapTime, int numFullLaps) {
    // Convert given number of data points to correct frequency for clock measurements.
    timePointsDivisor = (size_t) (latReadRate / clockReadRate);
    //size_t tmOverPoints = numOverPoints / timePointsDivisor;
    //size_t tmUnderPoints = numUnderPoints / timePointsDivisor;

    // Adding 2 because emit start/finish point twice. Adding 1 because current startFinishJump of 25 
    // results in always rounding down number of points, creating larger timeInc, and thus giving a laptime
    // that is always over the target, never under. This causes the tests with shorter lap times to fail.
    size_t pointsPerLap = (size_t) round((numOverPoints + numUnderPoints + 2 + 1 + startFinishJump) / timePointsDivisor);
    if (numUnderPoints == 0) {
        // If no under points, just output overpoints.
        pointsPerLap = numOverPoints / timePointsDivisor;
    }

    // Calculate what time between measurements should be. Am assuming constant speed around
    // entire lap, which is entirely unrealistic, but should give accurate times.
    timeInc = lapTime / pointsPerLap;
    
    // If only one lap, if lap is short, can get lap difference because start at zero at time zero.
    if (numFullLaps < 2) { 
        timeInc = lapTime / (pointsPerLap - 1);
    }
    cout << "pointsPerLap= " << pointsPerLap << ", timeInc=" << timeInc << "\n";
}

// Have been asked for fewer points than make up an entire lap, so just doing required number
// of points, at a fixed point.
void TestLapDataGenerator::addDegenerateLap() {
        //points.insert(points.begin(), overPoints, startLine + increment);
    latLongPoint newPoint (pitOutPoint.latitude + latInc, pitOutPoint.longitude + longInc);
    addLatLongPoints(numOverPoints, newPoint);
    addTimes(numOverPoints);
}

void TestLapDataGenerator::addLapsStartFinishOnly(double lapTime, int numFullLaps) {
    for (int i = 0; i < numFullLaps; i++) {
        addLapMiddle();
        addStartFinish();
        correctNumTimes();
    }
}


// Controls generating the requested number of laps. Note that if ask for one lap,
// get one out lap, one full lap and one in lap. If request 0 laps, get one out and one inlap
// neither of which has a representative lap time, which matches reality.
void TestLapDataGenerator::addLaps(double lapTime, int numFullLaps) {
    addPitExit();
    for (int i = 0; i <= numFullLaps; i++) {
        addLapMiddle();
        addStartFinish();
        correctNumTimes();
    }
    //addStartFinish();
    addLapMiddle();
    addTrackExit();
    correctNumTimes();
}

void TestLapDataGenerator::addPitExit() {
    cout << "addPitExit\n";
    latLongPoint afterPitExit (pitOutPoint.latitude + latInc/4, pitOutPoint.longitude + longInc/4);
    addLatLongPoints(2, pitOutPoint);
    addLatLongPoints(startFinishJump, afterPitExit);
    addTimes(startFinishJump + 2);
}

void TestLapDataGenerator::addLapMiddle() {
    cout << "addLapMiddle\n";
    latLongPoint afterStartFinish (startFinishPoint.latitude + latInc, startFinishPoint.longitude + longInc);
    latLongPoint beforeStartFinish (startFinishPoint.latitude - latInc, startFinishPoint.longitude - longInc);

    addLatLongPoints(numOverPoints, afterStartFinish);
    addLatLongPoints(numUnderPoints, beforeStartFinish);
    addTimes(numOverPoints + numUnderPoints);
}

void TestLapDataGenerator::addStartFinish() {
    cout << "addStartFinish\n";
    latLongPoint afterStartFinish (startFinishPoint.latitude + latInc/4, startFinishPoint.longitude + longInc/4);
    addLatLongPoints(2, startFinishPoint);
    addLatLongPoints(startFinishJump, afterStartFinish);
    addTimes(startFinishJump + 2);
}

void TestLapDataGenerator::addTrackExit() {
    cout << "addTrackExit\n";
    latLongPoint afterTrackExit (trackExitPoints[0].latitude + latInc/4, trackExitPoints[0].longitude + longInc/4);
    addLatLongPoints(2, trackExitPoints[0]);
    addLatLongPoints(startFinishJump, afterTrackExit);
    addTimes(startFinishJump + 2);
}

void TestLapDataGenerator::addLatLongPoints(size_t numPoints, latLongPoint newPoint) {
    cout << "addLatLongPoints, adding " << numPoints << " points.\n";
    oLatitudes.insert(oLatitudes.end(), numPoints, newPoint.latitude);
    oLongitudes.insert(oLongitudes.end(), numPoints, newPoint.longitude);
}

void TestLapDataGenerator::addTimes(size_t numLatPoints) {
    //cout << "addTimes, adding " << numLatPoints << " points with timeInc=" << timeInc << ".\n";
    size_t numTimePoints = numLatPoints / timePointsDivisor;
    cout << "addTimes, latPoints=" << numLatPoints << ", adding " << numTimePoints << " points with timeInc=" << timeInc << ".\n";
    unsigned short fmtHr = formatHour(currHr, currMin);
    for (int pt = 0; pt < numTimePoints; pt++) {
        oDaysData.push_back(startDay);
        oHoursData.push_back(fmtHr);
        oSecsData.push_back(formatSec(currSec));
        //cout << "currSec=" << currSec << ", added " << oSecsData[oSecsData.size() - 1] << "\n";

        // When increment seconds, may also need to increment minutes and hours.
        // Don't support laps/sessions that extend across days.
        currSec += timeInc;
        if (currSec >= 60) {
            currMin ++;
            if (currMin > 59) {
                currMin = 0;
                currHr++;
            }
            fmtHr = formatHour(currHr, currMin);
            currSec -= 60;
        }
    }
}

// Since sampling rates of lat/long and time are different, can get rounding errors when trying to add the same
// number of points to each. This function adds extra points to times to compensate.
void TestLapDataGenerator::correctNumTimes() {
    size_t missingTimePoints = oLatitudes.size() - (oSecsData.size() * 2);
    if ( missingTimePoints > 0) {
        addTimes(missingTimePoints);
        cout << "correctNumTimes, added " << missingTimePoints << " points.\n";
    }

}


double getPercentDiff(double targetTime, double calculatedTime) {
    double lapDiffPercent = abs((targetTime - calculatedTime)/targetTime) * 100;
    return lapDiffPercent;
}

template void TestLapDataGenerator::outputVector(vector<double>);
template void TestLapDataGenerator::outputVector(vector<size_t>);

