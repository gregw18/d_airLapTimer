// Getting some warnings about converting size_t to double could result in loss of precision.
// Since only used for graphing, not important. (Could manually convert my vectors from size_t
// to double to eliminate, but seems unuseful.)
#pragma warning( push )
#pragma warning(disable: 4244 )

#include <fstream>
#include <iostream>

#include "matplotlibcpp.h"

#include "Plotting.h"


namespace plt = matplotlibcpp;

using namespace std;

namespace gwDainese
{
    void testplot() {
        plt::plot({1,3,2,4});
        plt::show();
    }

    void testplot2() {
        std::vector<double> x = {1, 2, 3, 4, 1, 6, 3};
        std::vector<double> y = {1, 4, 9, 16, 10, 2, 20};
        plt::plot(x, y);
        plt::show();

    }

    void testplotScatter() {
        //std::vector<double> x = {1, 2, 3, 4, 1, 6, 3};
        //std::vector<double> y = {1, 4, 9, 16, 10, 2, 20};
        //std::vector<double> z = {1, 3, 5, 3, 3, 1, 5};

        std::vector<double> x = {1.99, 2.55, 3.001, 4.324, 1.234, 6, 3};
        std::vector<double> y = {1.0001, 4.999, 9.432, 16, 10, 2, 20};
        std::vector<double> z = {1.443, 3.111, 5.222, 3.001, 3.999, 1, 5};
        plt::scatter_colored(x, y, z, 20);
        plt::show();
    }

    // Bar chart of given vectors against each other, first as x, second as y.
    template<typename T1, typename T2> void barChart(const vector<T1>& x, const vector<T2>& y) {
        plt::bar(x, y);
        plt::show();
    }

    // Plot given vectors against each other, first as x, second as y.
    template<typename T1, typename T2> void plotXY(const vector<T1>& x, const vector<T2>& y) {
        plt::plot(x, y, "ro");
        plt::show();
    }

    // Plot given two sets of vectors against each other, first as x, second as y.
    template<typename T1, typename T2> void plot2XY(const vector<T1>& x1, const vector<T2>& y1, 
                                                const vector<T1>& x2, const vector<T2>& y2) {
        plt::plot(x1, y1, "ro");
        plt::plot(x2, y2, "bo");
        plt::show();
    }

    // Plot given vector against uniformly increasing series - i.e. first point against 0, second
    // against 1, etc.
    template<typename T1> void plotY(const vector<T1>& series1) {
        std::vector<size_t> xvector(series1.size());
        for (size_t i = 0; i < xvector.size(); i++) xvector[i] = i;

        plotXY(xvector, series1);
    }

    // Plot scatter of x/y, with z as color.
    template<typename T1, typename T2, typename T3> void plotScatterColor(const vector<T1>& x,
                                                                        const vector<T2>& y,
                                                                        const vector<T3>& extraData) {
        PyObject * res = plt::scatter_colored(x, y, extraData, 2, {{"cmap", "inferno"}});
        plt::colorbar(res);
        plt::show();
    }


    template void barChart(const vector<double>&, const vector<double>&);

    template void plotXY(const vector<double>&, const vector<double>&);
    template void plotXY(const vector<double>&, const vector<size_t>&);
 
    template void plotY(const vector<double>&);
    template void plotY(const vector<float>&);
    template void plotY(const vector<short>&);
    template void plotY(const vector<unsigned short>&);
 
    template void plot2XY(const vector<double>&, const vector<double>&, 
                                                const vector<double>&, const vector<double>&);
 
    template void plotScatterColor(const vector<double>&, const vector<double>&,
                                    const vector<float>& );
    template void plotScatterColor(const vector<double>&, const vector<double>&,
                                    const vector<double>& );
    template void plotScatterColor(const vector<double>&, const vector<double>&,
                                    const vector<unsigned short>& );
}
#pragma warning( pop )