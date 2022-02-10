#include <iostream>

#include "latLongHelpers.h"
#include "dataCleaning.h"
#include "fileRead.h"
#include "global.h"
#include "vectorHelpers.h"
#include "vectorSampling.h"
#include "Plotting.h"


using namespace std;

namespace gwDainese
{
    void plotLatLong(const string fileName, const string sampleDir) {
        string baseFilename = fileName;
        auto latLong = readLatLong(baseFilename, sampleDir);
        auto latitude = latLong.first;
        auto longitude = latLong.second;

        auto [goodLong, goodLat] = cleanVectors(myConfig.getValueDbl("longFilterLow"), 
                                                myConfig.getValueDbl("longFilterHigh"), 
                                                longitude, latitude);
        cout << "\nLongitude size before: " << longitude.size() << ", after: " << goodLong.size() << "\n";
        cout << "Latitude size before: " << latitude.size() << ", after: " << goodLat.size() << "\n";

        savePairToFile(goodLong, goodLat, "pairs.txt");

        cout << "\n goodlat: ";
        displayResults(goodLat);
        cout << "\n goodlong: ";
        displayResults(goodLong);

        plotXY(goodLong, goodLat);
        plotY(goodLat);
    }

    pair<vector<double>, vector<double>> readLatLong(const string baseFileName, const string sampleDir) {
        FileRead myReader;
        auto latitude = myReader.readFile<double, double>(sampleDir, baseFileName, DaineseDataTypes::latitude);
        auto longitude = myReader.readFile<double, double>(sampleDir, baseFileName, DaineseDataTypes::longitude);

        // Used to test doing analysis based in lat/long_dez instead of lat/long. Had no effect.
        //auto latitude = myReader.readFile<unsigned long, double>(sampleDir, baseFileName, DaineseDataTypes::lat_dez);
        //auto longitude = myReader.readFile<unsigned long, double>(sampleDir, baseFileName, DaineseDataTypes::lon_dez);

        return make_pair(latitude, longitude);
    }

    // Plot the given data type, as 3rd dimension (color), against lat/long.
    // Automatically resamples type to match frequency of lat/long data.
    template<typename TfileContents, typename treturnData>
    void graphFileAgainstLatLong(const string baseFileName, const string sampleDir, DaineseDataTypes myType) {

        FileRead myReader;
        auto extraData = myReader.readFile<TfileContents, treturnData>(sampleDir, baseFileName, myType);

        FileTypesData myFileData;
        auto origFreq = myFileData.GetFrequency(myType);
        auto targetFreq = myFileData.GetFrequency(DaineseDataTypes::latitude);
        //auto targetFreq = myFileData.GetFrequency(DaineseDataTypes::lat_dez);
        auto sampledExtraData = resampleData<treturnData>(extraData, origFreq, targetFreq);

        cout << "graphAgainstLatLong origFreq=" << origFreq << ", targetFreq=" << targetFreq
            << ", origType=" << static_cast<int> (myType) << ", targetType=" 
            << static_cast<int> (DaineseDataTypes::latitude) << "\n\n";

        graphAgainstLatLong(sampledExtraData, baseFileName, sampleDir);
    }

    // Graph subset of lat/long data, with subset of that highlighted. Used to figure out
    // where am losing data, for cases where not hitting exit at end of session.
    void graphSubsetHighlighted(const string baseFileName, const string sampleDir, 
                                size_t subsetLow, size_t subsetHigh, 
                                size_t highlightLow, size_t highlightHigh) {
        auto [latitudes, longitudes] = readLatLong(baseFileName, sampleDir);

        // Create 3rd data channel, all zero except for second range, containing 100
        const int highlightVal = 100;
        vector<unsigned short> extraData (latitudes.size(), 0);
        for (size_t i = highlightLow; i < highlightHigh; i++) {
            extraData[i] = highlightVal;
        }

        // Copy requested subset from each of 3 vectors.
        auto latitudesSubset = subset(latitudes, subsetLow, subsetHigh);
        auto longitudesSubset = subset(longitudes, subsetLow, subsetHigh);
        auto extraSubset = subset(extraData, subsetLow, subsetHigh);
        //cout << "graphSubsetHighlighted, latitudes.size=" << latitudes.size()
        //    << ", latitudesSubset.size=" << latitudesSubset.size() << "\n";
        //displayResults(latitudesSubset);
        //cout << "extraSubSet ";
        //displayResults(extraSubset);

        graphAgainstLatLong(extraSubset, latitudesSubset, longitudesSubset);
    }


    // Copy requested subset to new vector.
    template<typename T> vector<T> subset(const vector<T>& sourceData, size_t subsetLow, size_t subsetHigh) {
        auto startIt = sourceData.begin();
        advance (startIt, subsetLow);
        auto endIt = sourceData.begin();
        advance (endIt, subsetHigh);
        vector<T> subsetData (startIt, endIt);

        return subsetData;
    }

    // Plot the given data, as 3rd dimension (color), against lat/long.
    // Automatically resamples type to match lat/long data.
    // Requires the 3rd channel data and filename to retrieve lat/long from.
    template<typename TvectorContents>
    void graphAgainstLatLong(const vector<TvectorContents>& extraData, const string& baseFileName, 
                            const string& sampleDir) {

        auto latLong = readLatLong(baseFileName, sampleDir);

        cout << "latitudes ";
        displayResults(latLong.first);
        cout << "longitudes ";
        displayResults(latLong.second);
        cout << "\nsampled ";
        displayResults(extraData);

        graphAgainstLatLong(extraData, latLong.first, latLong.second);
    }

    // Plot the given data, as 3rd dimension (color), against lat/long.
    // Overload for when lat/long already in memory, appropriately sampled.
    template<typename TvectorContents>
    void graphAgainstLatLong(const vector<TvectorContents>& extraData, 
                                vector<double>& latitudes, vector<double>& longitudes) {

        auto cleaned = cleanVectors(myConfig.getValueDbl("longFilterLow"), 
                                    myConfig.getValueDbl("longFilterHigh"), 
                                    longitudes, latitudes, extraData);

        auto [goodLong, goodLat, goodExtra] = cleaned;
        cout << "\ncleaned latitudes ";
        displayResults(goodLat);
        cout << "\ncleaned longitudes ";
        displayResults(goodLong);

        // Filtering longitudinal acceleration to remove unusually high values, to give
        // more detail of track time.
        //auto [goodExtra2, goodLat2, goodLong2] = cleanVectors((double) 0, (double) 9, 
        //                            goodExtra, goodLat, goodLong);
        // Attempted ranges for riskAccInt - none seemed to give intelligible data.
        //auto [goodExtra2, goodLat2, goodLong2] = cleanVectors((unsigned short) 0, (unsigned short) 5, 
        //                            goodExtra, goodLat, goodLong);

        cout << "\ngood sampled ";
        displayResults(goodExtra, true);
        //displayResults(goodExtra2, true);

        plotScatterColor(goodLong, goodLat, goodExtra);
        //plotScatterColor(goodLong2, goodLat2, goodExtra2);
    }

    template void graphAgainstLatLong(const vector<unsigned short>&, const string&, const string&);
    template void graphAgainstLatLong(const vector<double>&, const string&, const string&);

    template void graphFileAgainstLatLong <unsigned short, unsigned short> (const string, const string, DaineseDataTypes);
    template void graphFileAgainstLatLong <unsigned short, double> (const string, const string, DaineseDataTypes);
    template void graphFileAgainstLatLong <float, float> (const string, const string, DaineseDataTypes);
}
