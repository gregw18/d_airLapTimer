#include <ctime>
#include <iostream>
#include <math.h>
#include <numeric>

#include "timeData.h"
#include "fileRead.h"
#include "timeHelpers.h"

using namespace std;

namespace gwDainese
{
    TimeData timeDataFactory(string baseFilename, string dataDir) {
        FileRead myReader;
        auto secondsData = myReader.readFile<unsigned short, unsigned short>(dataDir, baseFilename, DaineseDataTypes::sshh);
        auto hoursData = myReader.readFile<unsigned short, unsigned short>(dataDir, baseFilename, DaineseDataTypes::hhmm);
        auto daysData = myReader.readFile<unsigned short, unsigned short>(dataDir, baseFilename, DaineseDataTypes::mmdd);

        FileTypesData filesData;
        TimeData myData (daysData, hoursData, secondsData, filesData);
        return myData;
    }

    // Return number of seconds since start of day. Reads hours, minutes, converts to seconds,
    // then adds seconds/hundredths data.
    // Added support for greater precision than 1/6.25 seconds, by adding remainder from provided
    // source index to calculated time.
    double TimeData::getSeconds(size_t srcIndex, double srcFrequency) {
        size_t timeIndex = getTimeIndex(srcIndex, srcFrequency);
        auto secsFromHours = floor(hoursData[timeIndex]/100) * 60 * 60;
        auto secsFromMins = (hoursData[timeIndex] % 100 ) * 60;
        
        // Casting to double because otherwise seems to be assuming int,
        // and so loses hundredths.
        double uSeconds = (double) secondsData[timeIndex] / 100;
        double secsFromBalance = getSecsFromBalance(srcIndex, timeIndex, srcFrequency);
        double seconds = secsFromHours + secsFromMins + uSeconds + secsFromBalance;
        /* cout << "getSeconds, srcIndex=" << srcIndex << ", srcFrequency=" << srcFrequency
            << ", timeIndex=" << timeIndex << ", hoursData=" << hoursData[timeIndex]
            << ", secondsData=" << secondsData[timeIndex] << ", secsFromHours=" << secsFromHours
            << ", secsFromMins=" << secsFromMins << ", uSeconds=" << uSeconds 
            << ", secsFromBalance=" << secsFromBalance << ", seconds=" << seconds << "\n";
        */

        return seconds;
    }

    // Returns MMM DD.
    string TimeData::getDate(size_t srcIndex, double srcFrequency) {
        //cout << "getDate 1\n";
        size_t timeIndex = getTimeIndex(srcIndex, srcFrequency);
        //cout << "getDate 2, timeIndex=" << timeIndex << "\n";
        auto uDate = daysData[timeIndex];
        //cout << "getDate 3, uDate=" << uDate << "\n";
        int monthIndex = (int) floor(uDate/100);
        string thisDate = getMonthNameFromNum(monthIndex) + " " + to_string(uDate % 100);
        //cout << "getDate 4, thisDate=" << thisDate << "\n";

        return thisDate;
    }

    tm TimeData::getDateTime(size_t srcIndex, double srcFrequency) {
        size_t timeIndex = getTimeIndex(srcIndex, srcFrequency);
        //auto uDate = daysData[timeIndex];

        float secs = convertToSeconds(secondsData[timeIndex]);
        auto [hrs, mins] = convertToHHMM(hoursData[timeIndex]);
        auto [month, day] = convertToMMDD(daysData[timeIndex]);
        /* cout << "before getDateTime, secs=" << secondsData[timeIndex] << ", " << secs
            << "  HHMM=" << hoursData[timeIndex] << ", " << hrs << ", " << mins
            << "  MMDD=" << daysData[timeIndex] << ", " << month << ", " << day << "\n";
        */
        struct tm thisTime = {
            .tm_sec = (int) secs,
            .tm_min = (int) mins, 
            .tm_hour = (int) hrs, 
            .tm_mday = (int) day, 
            .tm_mon = (int) month
        };
        /* cout << "after getDateTime, secs=" << thisTime.tm_sec << ", min=" << thisTime.tm_min << ", hour=" << thisTime.tm_hour
            << ", day=" << thisTime.tm_mday << ", mon=" << thisTime.tm_mon << "\n";
        */

        return thisTime;
    }

    string TimeData::getDateTimeString(size_t srcIndex, double srcFrequency) {

        size_t timeIndex = getTimeIndex(srcIndex, srcFrequency);
    
        //float secs = convertToSeconds(secondsData[timeIndex]);
        auto [secs, huns] = convertToSSHH(secondsData[timeIndex]);
        auto [hrs, mins] = convertToHHMM(hoursData[timeIndex]);
        auto [month, day] = convertToMMDD(daysData[timeIndex]);
        /* cout << "before getDateTime, secs=" << secondsData[timeIndex] << ", " << secs
            << "  HHMM=" << hoursData[timeIndex] << ", " << hrs << ", " << mins
            << "  MMDD=" << daysData[timeIndex] << ", " << month << ", " << day << "\n";
        */
        string result = getMonthNameFromNum(month) + " " + str(day) + ", ";
        result += str(hrs) + ":" + str(mins) + ":";
        result += str(secs) + "." + str(huns);

        return result;
    }

    string TimeData::str(short num) {
        string result;
        if (num < 10) {
            result = "0" + to_string(num);
        }
        else result = to_string(num);

        return result;
    }

    // Find the time data closest to the given data point.
    // Time is sampled at lowest frequency, 6.25Hz. Everything else is sampled at a higher frequency,
    // but an integer multiplier of 6.25. To convert an index from a different frequency to the
    // closest index for time data, find the multiplier, banker's rounding, ensuring that
    // not past end of time vector.
    size_t TimeData::getTimeIndex(size_t srcIndex, double srcFrequency) {
        // Seems that if divide by int or size_t, result is truncated before can be rounded,
        // so don't get desired result. i.e. I would like to get 1 from 7/8, but get zero.
        // If divide by float, get desired rounded values.
        auto divider0 = srcFrequency / timeFrequency;
        size_t timeIndex = (size_t) round(srcIndex / divider0);

        // Ensure that don't go past end of the vector
        if (timeIndex >= daysData.size()) timeIndex = daysData.size() - 1;

        return timeIndex;
    }

    // Since most measurements are done at higher frequency than time, can use the number of
    // additional measurements and their rate to increase the precision of the calculated time.
    // I.e. if srcIndex is 13 and frequency is 50 Hz, index for time would be 1 at 6.25 Hz (8 at 50Hz)
    // leaving 13-8/50 = 5/50 = 1/10s that should be added to the time calculated from the time measurement.
    double TimeData::getSecsFromBalance(size_t srcIndex, size_t timeIndex, double srcFrequency) {

        double addSecs;

        int freqMultiplier = (int) (srcFrequency / timeFrequency);
        long numAdditionalTicks = (long) srcIndex - (long) (timeIndex * freqMultiplier);
        addSecs = numAdditionalTicks / srcFrequency;
        /* cout << "getSecsFromBlance, srcIndex=" << srcIndex << ", timeIndex=" << timeIndex 
            << ", freqMultiplier=" << freqMultiplier << ", subtractor=" << (timeIndex * freqMultiplier)
            << ", numAdditionalTicks="
            << numAdditionalTicks << ", addSecs=" << addSecs << "\n";
        */

        return addSecs;
    }
}
