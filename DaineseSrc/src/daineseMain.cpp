#include <exception>
#include <iomanip>
#include <iostream>
#include <utility>

#include "global.h"
#include "fileRead.h"
#include "lapTimes.h"
#include "vectorHelpers.h"
#include "latLongHelpers.h"

using namespace std;
using namespace gwDainese;

void parseArgs(int, char*[]);
void populateConfig(const string cfgFile);
template<typename TfileContents, typename TreturnData> 
    void displayFileContents(string baseFileName, DaineseDataTypes myType);
void displayTimeLatLong(string baseFileName);
void testTextRead(const string);


string cfgFile = "config.txt";
string sampleDir = "";
string fileName;
Config gwDainese::myConfig;

int main(int argc, char* argv[]) {
    try {
        parseArgs(argc, argv);
        populateConfig(cfgFile);
        fileName = myConfig.getValueStr("baseFileName");

        //displayFileContents <unsigned short, double> (fileName, DaineseDataTypes::a_lon_gps );
        //displayFileContents <unsigned short, double> (fileName, DaineseDataTypes::a_lat_gps );
        //displayFileContents <unsigned short, double> (fileName, DaineseDataTypes::acc_x);
        //displayFileContents <unsigned short, double> (fileName, DaineseDataTypes::acc_y);
        //displayFileContents <unsigned short, double> (fileName, DaineseDataTypes::acc_z);
        //displayFileContents <unsigned long, double> (fileName, DaineseDataTypes::altitude_meas);
        //displayFileContents <float, float> (fileName, DaineseDataTypes::altitude_calc);
        //displayFileContents <float, float> (fileName, DaineseDataTypes::banking_gps );
        //displayFileContents <unsigned short, double> (fileName, DaineseDataTypes::course);
        //displayFileContents <unsigned short, double> (fileName, DaineseDataTypes::gps_yaw);
        //displayFileContents <unsigned short, unsigned short> (fileName, DaineseDataTypes::hhmm);
        //displayFileContents <unsigned long, double> (fileName, DaineseDataTypes::lat_dez);
        //displayFileContents <double, double> (fileName, DaineseDataTypes::latitude);
        //displayFileContents <unsigned long, double> (fileName, DaineseDataTypes::lon_dez);
        //displayFileContents <double, double> (fileName, DaineseDataTypes::longitude);
        //displayFileContents <unsigned short, unsigned short> (fileName, DaineseDataTypes::mmdd);
        //displayFileContents <unsigned short, unsigned short> (fileName, DaineseDataTypes::riskAccInt);
        //displayFileContents <unsigned short, unsigned short> (fileName, DaineseDataTypes::sshh);
        //displayFileContents <unsigned short, double> (fileName, DaineseDataTypes::v_bat);
        //displayFileContents <unsigned short, double> (fileName, DaineseDataTypes::v_gps);
        //displayFileContents <float, float> (fileName, DaineseDataTypes::v_sat_calc);
        //testTextRead("test.txt");

        //graphFileAgainstLatLong <unsigned short, unsigned short> (fileName, sampleDir, DaineseDataTypes::riskAccInt );
        //graphFileAgainstLatLong <unsigned short, double> (fileName, sampleDir, DaineseDataTypes::a_lon_gps );
        //graphFileAgainstLatLong <float, float> (fileName, sampleDir, DaineseDataTypes::banking_gps );
        //graphFileAgainstLatLong <float, float> (fileName, sampleDir, DaineseDataTypes::v_sat_calc);
        //graphFileAgainstLatLong <unsigned short, double> (fileName, sampleDir, DaineseDataTypes::acc_x);
        //graphFileAgainstLatLong <unsigned short, double> (fileName, sampleDir, DaineseDataTypes::acc_y);
        //graphFileAgainstLatLong <unsigned short, double> (fileName, sampleDir, DaineseDataTypes::acc_z);
        //graphFileAgainstLatLong <unsigned short, double> (fileName, sampleDir, DaineseDataTypes::gps_yaw);


        //testplot();
        //testplot2();
        //testplotScatter();
        //plotLatLong(fileName, sampleDir);
        lapTimes(fileName, sampleDir);
        //graphTrackPOIs(fileName, sampleDir);
        //displayTimeLatLong(fileName);

        // First dropout.
        //size_t offset = 12;  // Number lines at top of reference file that don't contain valid data.
        //graphSubsetHighlighted(fileName, sampleDir, 161215 - offset, 167910 - offset, 167377 - offset, 167910 - offset);
        // Second dropout.
        //graphSubsetHighlighted(fileName, sampleDir, 185579 - offset, 200542 - offset, 198171 - offset, 200542 - offset);
        // Missed out lap.
        //graphSubsetHighlighted(fileName, sampleDir, 208524 - offset, 210710 - offset, 208524 - offset, 208896 - offset);

    }
    catch (exception ex) {
        cout << "exception: " << ex.what() << "\n";
    }
}

void parseArgs(int argc, char* argv[]) {
    cout << "Provided arguments\n";
    cout << "argc: " << argc << "\n";
    for (int i = 0; i < argc; i++) {
        cout << "i:" << i << ", " << argv[i] << "\n";
    }
    cout << "\n";
    if (argc == 2) {
        sampleDir = argv[1];
    }
}

void populateConfig(const string cfgFile) {
    vector<string> cfgStrings = readTextFileToVector(cfgFile);
    myConfig.init(cfgStrings);
    //myConfig.logValues();
}

// Read data from requested file name and type, display results.
template<typename TfileContents, typename TreturnData>
 void displayFileContents(string baseFileName, DaineseDataTypes myType) {

    FileRead myReader;
    auto fileContents = myReader.readFile<TfileContents, TreturnData>(sampleDir, baseFileName, myType);
    displayResults(fileContents, true);
}

// Output all time and lat/long data points, from given base file.
void displayTimeLatLong(string baseFileName) {
    FileTypesData myFileData;
    auto latFreq = myFileData.GetFrequency(DaineseDataTypes::latitude);

    TimeData timeData = timeDataFactory(baseFileName, sampleDir);
    auto latLong = readLatLong(baseFileName, sampleDir);

    cout << setprecision(9);
    for (size_t i = 0; i < latLong.first.size(); i++) {
        if ( i % 2 == 0) {
            cout << timeData.getDateTimeString(i, latFreq) << "  ";
        }
        else cout << "                     ";
        cout << latLong.first[i] << "  " << latLong.second[i] << "\n";
    }
}

void testTextRead(const string fileName) {
    auto results = readTextFileToVector(fileName);
    //outputVector(results);
}
