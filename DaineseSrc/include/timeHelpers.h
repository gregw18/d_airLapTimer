#include <string>
#include <ctime>

namespace gwDainese
{
    float convertToSeconds(short secs);
    std::pair<short, short> convertToSSHH(short sshh);
    std::pair<short, short> convertToHHMM(short hhmm);
    std::pair<short, short> convertToMMDD(short MMDD);
    std::string getMonthNameFromNum(short i);
    double getTotalSecsFromTime(tm thisTime);
    bool areDaysSame(tm day1, tm day2);
}