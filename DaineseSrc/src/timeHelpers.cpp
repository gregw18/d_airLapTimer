#include <iostream>
#include <math.h>
#include <numeric>
#include <vector>

#include "timeHelpers.h"

using namespace std;

namespace gwDainese
{
    std::vector<string> months {"JAN", "FEB", "MAR", "APR", "MAY", "JUN",
                                    "JUL", "AUG", "SEP", "OCT", "NOV", "DEC"};

    float convertToSeconds(short secs) {
        return ((float) secs / 100);
    }

    pair<short, short> convertToSSHH(short sshh) {
        short secs = (short) floor(sshh/100);
        short hundredths = (short) (sshh % 100);
        
        return make_pair(secs, hundredths);
    }

    pair<short, short> convertToHHMM(short hhmm) {
        short hrs = (short) floor(hhmm/100);
        short mins = (short) (hhmm % 100);
        
        return make_pair(hrs, mins);
    }

    pair<short, short> convertToMMDD(short MMDD) {
        short month = (short) floor(MMDD/100);
        short day = (short) (MMDD % 100);

        return make_pair(month, day);
    }

    // Convert from number of month, 1-12, to three character abbreviation for name.
    // i.e. 2=FEB.
    string getMonthNameFromNum(short i) {
        if ( i > 0)
            return months[i-1];
        else
            return "ZZZ";
    }

    // Convert time of day to number of seconds since start of day. I.e. 1:13:44am would be 
    // 1 * 60 * 60 + 13 * 60 + 44.
    double getTotalSecsFromTime(tm thisTime) {
        double numSecs = thisTime.tm_hour * 60 * 60;
        numSecs += thisTime.tm_min * 60;
        numSecs += thisTime.tm_sec;

        return numSecs;
    }

    // Check if days are the same - i.e. year, month and day of month.
    bool areDaysSame(tm day1, tm day2) {
        bool areSame = false;

        if (day1.tm_year == day2.tm_year) {
            if (day1.tm_mon == day2.tm_mon) {
                if (day1.tm_mday == day2.tm_mday) {
                    areSame = true;
                }
            }
        }

        return areSame;
    }

}
