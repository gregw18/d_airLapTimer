#include <tuple>
#include <vector>
#include <utility>

namespace gwDainese
{
    template<typename T1, typename T2, typename T3 = double>
        std::tuple<std::vector<T1>, std::vector<T2>, std::vector<T3>> 
            cleanVectors(T1 lowValue, T1 highValue, const std::vector<T1>& vec1,
                        const std::vector<T2>& vec2, const std::vector<T3>& vec3);
    template<typename T1, typename T2>
        std::tuple<std::vector<T1>, std::vector<T2>>
            cleanVectors(T1 lowValue, T1 highValue, const std::vector<T1>& vec1,
                        const std::vector<T2>& vec2);

    template<typename T> std::vector<size_t> GetBadIndexes(const std::vector<T>& data, T lowValue, T highValue);
    size_t guessNumInvalid(const size_t);
    template<typename T> std::vector<T> RemoveBadData(const std::vector<T>&, const std::vector<size_t>&);
    std::pair<std::vector<double>, std::vector<double>> cleanLapsForGraphing(const std::vector<double>& lapTimes, 
                                                                        const std::vector<double>& plotTimes);
    //std::vector<double> cleanLapsForGraphing(const std::vector<double>&);
    double getAverage(const std::vector<double>&);
}