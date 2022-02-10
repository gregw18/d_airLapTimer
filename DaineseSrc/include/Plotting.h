#include <vector>

namespace gwDainese
{
    void testplot();
    void testplot2();
    void testplotScatter();
    template<typename T1, typename T2> void barChart(const std::vector<T1>& x, const std::vector<T2>& y);
    template<typename T1, typename T2> void plotXY(const std::vector<T1>& x, const std::vector<T2>& y);
    template<typename T1> void plotY(const std::vector<T1>& y);
    template<typename T1, typename T2> void plot2XY(const std::vector<T1>& x1, const std::vector<T2>& y1, 
                                                const std::vector<T1>& x2, const std::vector<T2>& y2);
    template<typename T1, typename T2, typename T3> void plotScatterColor(const std::vector<T1>& x,
                                                                        const std::vector<T2>& y,
                                                                        const std::vector<T3>& extraData);
}