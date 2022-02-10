#include <iostream>
#include <vector>

namespace gwDainese
{
    template<typename T> std::vector<T> upSample(const std::vector<T>& origData, int upSampleRatio) {
        std::vector<T> newData (origData.size() * upSampleRatio, 0);

        std::cout << "upSample, ratio=" << upSampleRatio << ", newData.size=" << newData.size() << "\n";
        for (size_t i = 0; i < origData.size() - 1; i++) {
            for (size_t j = 1; j <= upSampleRatio; j++) {
                newData[(upSampleRatio * i) + j] = origData[i];
            }
        }

        return newData;
    }

    // Need to downsample data - only output every downsampleth item.
    // i.e. if ratio is 8, return items 0, 8, 16, ...
    // Note that am taking absolute value. For lean angle, don't really care if leaning left or right,
    // for acceleration, clear from track whether positive or negative, and easier to tell where zero is.
    template<typename T> std::vector<T> downSample(const std::vector<T>& origData, int downSampleRatio) {
        std::vector<T> newData ((size_t) origData.size() / downSampleRatio, 0);
        std::cout << "downSample, ratio=" << downSampleRatio << ", newData.size=" << newData.size() << "\n";
        for (size_t i = 0; i < origData.size(); i += downSampleRatio) {
            newData[i/downSampleRatio] = abs(origData[i]);
        }

        return newData;
    }

    // Either need to downsample or upsample given data, to match desired sampling rate.
    // I.e. if have 6.25, but want 12.5, need to duplicate every point in given data, i.e. AABBCC...
    // If have 25, but want 12.5, need to take every other point in given data, i.e. ACEG.
    template<typename T> std::vector<T> resampleData(const std::vector<T>& origData, double origSampleRate, 
                                                double targetSampleRate) {
        std::vector<T> newData;
        if (origSampleRate == targetSampleRate) newData = origData;
        else if (origSampleRate < targetSampleRate) newData = upSample(origData, (int) (targetSampleRate/origSampleRate));
        else newData = downSample(origData, (int) (origSampleRate/targetSampleRate));

        std::cout << "resampleData, origRate=" << origSampleRate << ", targetRate=" << targetSampleRate
            << ", origSize=" << origData.size() << ", newSize=" << newData.size() << "\n";

        return newData;
    }

}
