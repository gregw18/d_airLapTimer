#include <vector>

#include "latLong.h"


class TestLapDataGenerator {
    public:
        //gwDainese::latLongPoint startFinish = gwDainese::latLongPoint(44.2249, -77.1593);

        void outputPair(std::pair<std::vector<double>, std::vector<double>>);
        void outputPairSideBySide(std::pair<std::vector<double>, std::vector<double>>);
        template<typename T> void outputVector(std::vector<T>);
        std::tuple<std::vector<double>, std::vector<double>, std::vector<unsigned short>, 
                    std::vector<unsigned short>, std::vector<unsigned short>> 
            getTestLapData(const double lapTime, const int numFullLaps, const gwDainese::TrackData myTrack);

    private:
        std::vector<double> getPoints(const double, const int, const double, size_t overPoints, size_t underPoints);
        double getIncrement(const double);
        std::vector<size_t> GetConsecutiveGaps(const std::vector<double>&, double);
        void setNumOverUnderPoints(const double laptime);
        std::tuple<std::vector<unsigned short>, std::vector<unsigned short>, std::vector<unsigned short>>
            getTimes(const double laptime, 
                        const int numFullLaps, 
                        const size_t overPoints, 
                        const size_t underPoints);
        std::tuple<std::vector<unsigned short>, std::vector<unsigned short>, std::vector<unsigned short>> 
            generatePointTimes( const double incTime, 
                                const int numFullLaps, 
                                const size_t pointsPerLap);
        short formatHour(const short hour, const short min);
        short formatSec(const double sec);
        void parseTrackData(gwDainese::TrackData myTrack);
        void setLatLongInc();
        void setTimeInc(double lapTime, int numFullLaps);
        void clearOutputData();
        void addDegenerateLap();
        void addLaps(double lapTime, int numFullLaps);
        void addLapsStartFinishOnly(double lapTime, int numFullLaps);
        void addPitExit();
        void addLapMiddle();
        void addStartFinish();
        void addTrackExit();
        void addLatLongPoints(size_t numPoints, gwDainese::latLongPoint newPoint);
        void addTimes(size_t numPoints);
        void correctNumTimes();

        const double latLongThresh = 0.0001;
        const size_t startFinishJump = 25;
        const double latReadRate = 12.5;
        const double clockReadRate = 6.25;
        const short startHr = 9;  // Start all sessions at 9am, for simplicity.
        const short startMin = 0;
        const double startSec = 0;
        const short startDay = 910; // Hardcoding month/day to sept 10.

        gwDainese::latLongPoint startFinishPoint;
        gwDainese::latLongPoint pitOutPoint;
        std::vector<gwDainese::latLongPoint> trackExitPoints;
        size_t numOverPoints;
        size_t numUnderPoints;
        double latInc;
        double longInc;
        double timeInc;
        size_t timePointsDivisor;       // Ratio of lat/long points to time points - 12.5/6.25.
        unsigned short currHr;
        unsigned short currMin;
        double currSec;
        std::vector<double> oLatitudes;
        std::vector<double> oLongitudes;
        std::vector<unsigned short> oDaysData;
        std::vector<unsigned short> oHoursData;
        std::vector<unsigned short> oSecsData;

        /*
        const double startFinishLat (-78);
        const double startFinishLong (44);
        const double latLongThresh = 1;
        const size_t startFinishJump = 50;
        const double latReadRate = 12.5;
        */
};

double getPercentDiff(double targetTime, double calculatedTime);
