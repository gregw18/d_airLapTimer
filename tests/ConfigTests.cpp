#include <vector>

#include "gtest/gtest.h"

#include "global.h"

using namespace std;
using namespace gwDainese;


/*
// Helper to generate test data with desired lap time, number of laps, call function
// to calculate lap times from that data, and return them for checking.
std::pair<std::vector<double>, std::vector<tm>> 
    SetUpData(const double laptime, const int numLaps) {
        latLongPoint startFinish (22, -33);
        latLongPoint pitOut (23, -33);
        latLongPoint exit1 (21, -33);
        latLongPoint exit2 (22, -35);
        vector<latLongPoint> exits ({exit1, exit2});
        TrackData track1 (startFinish, pitOut, exits);

        TestLapDataGenerator myGen;
        auto [lats, longs, daysData, hoursData, secsData] = myGen.getTestLapData(laptime, numLaps, track1);

        FileTypesData filesData;
        TimeData timeData (daysData, hoursData, secsData, filesData);

        double latFreq = filesData.GetFrequency(DaineseDataTypes::latitude);
        vector<latLongPoint> emptyVec;
        //TrackData myTrack (myGen.startFinish, latLongPoint(), emptyVec);
        LapAnalysis myAnalyzer(lats, longs, track1, timeData, latFreq);

        auto res = myAnalyzer.calcLapTimes(false);

        return res;
}
*/

const vector<string> noConfigData;
const vector<string> someConfigData = {"key1=val1", "key2=val2"};
const vector<string> multiTypeConfigData = {"key1=val1", "dbl1=77.12345", "int1=5432"};
const vector<string> spacedConfigData = {" key1=val1 ", " dbl1 = 77.12345 ", "int1 =5432"};
const vector<string> noEqualsConfigData = {" key1=val1 ", "badKey vadVal", " dbl1 = 77.12345 ", "int1 =5432", "key2="};

// Asking for one lap, but with a time that is less than the startFinish jump. Generator gives
// bunch of times at one location, and so no complete lap.
TEST(ConfigTest, NoDataNoValueReturned){
    Config myConfig;
    myConfig.init(noConfigData);

    EXPECT_THROW(string myVal = myConfig.getValueStr ("badkey"), out_of_range);
}

TEST(ConfigTest, SomeDataBadKeyNoValueReturned){
    Config myConfig;
    myConfig.init(someConfigData);

    EXPECT_THROW(string myVal = myConfig.getValueStr ("badkey"), out_of_range);
}

TEST(ConfigTest, SomeDataGoodKeyValueReturned){
    Config myConfig;
    myConfig.init(someConfigData);

    string myVal = myConfig.getValueStr ("key1");

    ASSERT_EQ("val1", myVal);
}

TEST(ConfigTest, SomeDataGoodKeyDoubleValueReturned){
    Config myConfig;
    myConfig.init(multiTypeConfigData);

    double myVal = myConfig.getValueDbl ("dbl1");

    ASSERT_EQ(77.12345, myVal);
}

TEST(ConfigTest, SomeDataGoodKeyIntValueReturned){
    Config myConfig;
    myConfig.init(multiTypeConfigData);

    int myVal = myConfig.getValueInt ("int1");

    ASSERT_EQ(5432, myVal);
}


TEST(ConfigTest, SomeDataBadValueZeroValueReturned){
    Config myConfig;
    myConfig.init(multiTypeConfigData);

    EXPECT_THROW(int myVal = myConfig.getValueInt ("key1"), invalid_argument);
}

TEST(ConfigTest, SpacedDataGoodKeyValueReturned){
    Config myConfig;
    myConfig.init(spacedConfigData);

    string myVal = myConfig.getValueStr ("key1");

    ASSERT_EQ("val1", myVal);
}

TEST(ConfigTest, SpacedDataIntKeyValueReturned){
    Config myConfig;
    myConfig.init(spacedConfigData);

    int myVal = myConfig.getValueInt ("int1");

    ASSERT_EQ(5432, myVal);
}

TEST(ConfigTest, SpacedDataDblKeyValueReturned){
    Config myConfig;
    myConfig.init(spacedConfigData);

    double myVal = myConfig.getValueDbl ("dbl1");

    ASSERT_EQ(77.12345, myVal);
}

// Ask for key that doesn't have equals sign
TEST(ConfigTest, NoEqualsDataBadKeyValueReturned){
    Config myConfig;
    myConfig.init(noEqualsConfigData);

    EXPECT_THROW(string myVal = myConfig.getValueStr ("badKey"), out_of_range);
}

// Ask for before and after keys in set that is missing an equals sign.
TEST(ConfigTest, NoEqualsDataGoodKeysValuesReturned){
    Config myConfig;
    myConfig.init(noEqualsConfigData);

    string myVal1 = myConfig.getValueStr ("key1");
    int myVal2 = myConfig.getValueInt ("int1");
    ASSERT_EQ("val1", myVal1);
    ASSERT_EQ(5432, myVal2);
}

// Ask for key that exists but has an empty value.
TEST(ConfigTest, NoEqualsDataNoValNoValueReturned){
    Config myConfig;
    myConfig.init(noEqualsConfigData);

    string myVal = myConfig.getValueStr ("key2");

    ASSERT_EQ(0, myVal.length());
}
