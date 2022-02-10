#pragma once

#include <string>
#include <vector>

#include "FileTypesData.h"

namespace gwDainese
{
    // Note that am currently storing data here in same format as it is in the source files.
    // Thus, seconds is SSHH with two implied decimal places, and HH represents hundredths of a second.
    // Hours is HHMM, 24 hour clock.
    // Days is MMDD.
    class TimeData{
        public:
            TimeData() {
                std::vector<unsigned short> secondsData;
                std::vector<unsigned short> hoursData;
                std::vector<unsigned short> daysData;
            }
            TimeData(const std::vector<unsigned short>& daysVec, const std::vector<unsigned short>& hoursVec, 
                        const std::vector<unsigned short>& secondsVec,
                        FileTypesData filesData):
                secondsData(secondsVec),
                hoursData(hoursVec),
                daysData(daysVec) {
                    timeFrequency = filesData.GetFrequency(DaineseDataTypes::sshh);
                }

            double getSeconds(size_t srcIndex, double srcFrequency);
            std::string getDate(size_t srcIndex, double srcFrequency);
            tm getDateTime(size_t srcIndex, double srcFrequency);
            std::string getDateTimeString(size_t srcIndex, double srcFrequency);

        private:
            std::vector<unsigned short> secondsData;
            std::vector<unsigned short> hoursData;
            std::vector<unsigned short> daysData;
            double timeFrequency;       // Times per second that the time data is saved.

            size_t getTimeIndex(size_t srcIndex, double srcFrequency);
            double getSecsFromBalance(size_t srcIndex, size_t timeIndex, double srcFrequency);
            std::string str(short num);
    };

    TimeData timeDataFactory(std::string baseFilename, std::string dataDir);
}
