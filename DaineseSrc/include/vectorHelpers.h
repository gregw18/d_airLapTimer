#include <string>
#include <vector>

namespace gwDainese
{
    template<typename T> void displayResults(const std::vector<T>&, bool plotMe = false);
    template<typename T> void displayUniqueValues(const std::vector<T>&);
    template<typename T> void outputVector(const std::vector<T>&);
    void outputPairSideBySide(const std::vector<double>& lapTimes, const std::vector<tm>& startTimes);
    void outputPairSideBySide(const std::vector<double>& lapTimes, const std::vector<double>& plotTimes);
    template<typename T1, typename T2> void savePairToFile(const std::vector<T1>& data1, 
                                                            const std::vector<T2>& data2,
                                                            const std::string fileName);
}
