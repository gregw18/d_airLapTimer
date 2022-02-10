#include <string>
#include <vector>

#include "analysis.h"
#include "timeData.h"

namespace gwDainese
{
    void lapTimes(std::string fileName, std::string sampleDir);
    LapAnalysis createAnalyzer(std::vector<double>& latitudes, 
                                std::vector<double>& longitudes, 
                                TimeData timeData);
    void graphTrackPOIs(std::string baseFileName, std::string sampleDir);
    std::vector<double> getPlotTimes(const std::vector<tm>& startTimes);
}
