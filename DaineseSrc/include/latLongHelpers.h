#include <string>
#include <utility>
#include <vector>

#include "DaineseDataTypes.h"


namespace gwDainese
{
    void plotLatLong(const std::string fileName, const std::string sampleDir);
    std::pair<std::vector<double>, std::vector<double>> readLatLong(std::string baseFileName, std::string sampleDir);
    template<typename TfileContents, typename treturnData>
        void graphFileAgainstLatLong(const std::string baseFileName, const std::string sampleDir, DaineseDataTypes myType);
    void graphSubsetHighlighted(const std::string baseFileName, const std::string sampleDir,
                                size_t subsetLow, size_t subsetHigh, 
                                size_t highlightLow, size_t highlightHigh);
    template<typename T> std::vector<T> subset(const std::vector<T>& sourceData, size_t subsetLow, size_t subsetHigh);
    template<typename TvectorContents>
        void graphAgainstLatLong(const std::vector<TvectorContents>& extraData, const std::string& baseFileName, 
                                const std::string& sampleDir);
    template<typename TvectorContents>
    void graphAgainstLatLong(const std::vector<TvectorContents>& extraData, 
                                std::vector<double>& latitudes, std::vector<double>& longitudes);
}
