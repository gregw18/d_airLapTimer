#include <vector>

#include "global.h"
#include "latLong.h"
#include "timeData.h"

namespace gwDainese
{   class LapAnalysis {
    public:
        std::vector<double> latitudes;
        std::vector<double> longitudes;
        latLongPoint startFinish;
        latLongPoint trackEntrance;
        std::vector<latLongPoint> trackExits;
        TimeData timeData;
        bool addTimes;
        double latFrequency;    // Sampling frequency for latitude.
        bool startFinishOnly;   // Flag for whether want to calculate times based on start/finish line only,
                                // or whether want to also include pit entrance and exit.

        LapAnalysis (const std::vector<double>& lats, const std::vector<double>& longs, 
                        const TrackData myTrack, const TimeData myTimeData, 
                        double myFrequency, bool myAddTimes = false) :
                        latitudes(lats),
                        longitudes(longs),
                        startFinish(myTrack.startFinishPoint),
                        trackEntrance(myTrack.trackEntrancePoint),
                        trackExits(myTrack.trackExitPoints),
                        timeData(myTimeData),
                        latFrequency(myFrequency),
                        addTimes(myAddTimes)
                        {}

        std::pair<std::vector<double>, std::vector<tm>> 
            calcLapTimes(bool startOnly = false);
        std::vector<unsigned short> getTrackPOIs();
    
    private:
        const double latLongRange = myConfig.getValueDbl("latLongWindow");
        const size_t lapIndexJump = myConfig.getValueInt("startFinishJump");    // Number of items to skip once find start/finish, to avoid
                                                                                // accidentally thinking we've immediately hit it again.

        bool isInRange(double, double, double);
        double GetTimeDifference(size_t currIndex, size_t lastIndex, double latFrequency);
        double GetTimeDifference(size_t currIndex, size_t lastIndex, double latFrequency, TimeData);
        bool atStartFinish(size_t index);
        bool atTrackEntrance(size_t index);
        bool atTrackExit(size_t index);
    };
}
