#include "gtest/gtest.h"

#include "timeData.h"

using namespace std;
using namespace gwDainese;


class TimeDataTests : public ::testing::Test {
    protected:
        const vector<unsigned short> seconds {0000, 5997, 3075, 5999};
        const vector<unsigned short> hours {759, 1301, 1755, 2359};
        const vector<unsigned short> days {909, 928, 731, 1231};
        const double timeFrequency = 6.25;
        TimeData timeData;

        TimeDataTests() {
            FileTypesData filesData;
            timeData = TimeData (days, hours, seconds, filesData);
        }

        double GetSecondsFromIndex(size_t timeIndex, size_t srcIndex, double srcFreq) {
            double calcHours = (double) floor(hours[timeIndex]/100);
            double calcMins = (double) (hours[timeIndex] % 100);
            double calcSecs = (double) seconds[timeIndex];
            //cout << "GetSecondsFromIndex, calcHours=" << calcHours << ", calcMins=" << calcMins
            //    << ", calcSecs=" << calcSecs << "\n";
            double calculatedHundredths = (double) (calcHours * 3600 * 100 + calcMins * 60 * 100 + calcSecs);
            double calculatedSeconds = calculatedHundredths / 100;

            // Add time for additional, higher-frequency measurements from source, if any.
            int freqMultiplier = (int) (srcFreq / timeFrequency);
            double extraSecs = ((long) srcIndex - (long) (timeIndex * freqMultiplier)) / srcFreq;
            calculatedSeconds += extraSecs;
            //cout << "hundredths: " << calculatedHundredths << ", seconds: " << calculatedSeconds << "\n";

            return calculatedSeconds;
        }

        /* tm GettmFromString(string myDateTime) {
            struct tm myTm;

            strptime(myDateTime, "%d %b %Y %H:%M:%S", &myTm);
            myTm.tm_isdst = -1;

            return myTm;
        } */

        bool CompareDates(tm calculatedTm, int month, int day, int hour, int min, int sec) {

            bool areSame = false;
            cout << "CompareDates calculatedTm, month=" << calculatedTm.tm_mon
                << ", day=" << calculatedTm.tm_mday
                << ", hour=" << calculatedTm.tm_hour
                << ", min=" << calculatedTm.tm_min
                << ", sec=" << calculatedTm.tm_sec << "\n";
            if (calculatedTm.tm_mon == month) {
                if (calculatedTm.tm_mday == day) {
                    if (calculatedTm.tm_hour == hour) {
                        if (calculatedTm.tm_min == min) {
                            if (calculatedTm.tm_sec == sec) areSame = true;
                        }
                    }
                }
            }

            return areSame;
        }
};


// Tests that various frequencies map to the correct index
TEST_F(TimeDataTests, DateIdx0){
    string results = timeData.getDate((size_t)0, 6.25);
    EXPECT_EQ("SEP 9", results);
    string results2 = timeData.getDate((size_t)0, 1000);
    EXPECT_EQ("SEP 9", results2);
    string results3 = timeData.getDate((size_t)1, 12.5);
    EXPECT_EQ("SEP 28", results3);
}

// Test that am rounding to desired index correctly.
TEST_F(TimeDataTests, DateIdx1){
    string results = timeData.getDate((size_t)3, 50);
    EXPECT_EQ("SEP 9", results);
    results = timeData.getDate((size_t)4, 50);
    EXPECT_EQ("SEP 28", results);
    results = timeData.getDate((size_t)8, 50);
    EXPECT_EQ("SEP 28", results);
    results = timeData.getDate((size_t)11, 50);
    EXPECT_EQ("SEP 28", results);
    results = timeData.getDate((size_t)12, 50);
    EXPECT_EQ("JUL 31", results);
    string results2 = timeData.getDate((size_t)7, 12.5);
    EXPECT_EQ("DEC 31", results2);
}

// Test that if go past end of data, get last item.
TEST_F(TimeDataTests, RequestPastEnd) {
    string results = timeData.getDate((size_t) 8, 12.5);
    EXPECT_EQ("DEC 31", results);
}

TEST_F(TimeDataTests, SimpleDiffFreqGood) {
    string results = timeData.getDate(0, 1000);
    EXPECT_EQ("SEP 9", results);
}

// Test calculating seconds, for requested indexes.
TEST_F(TimeDataTests, TimeIdx0) {
    size_t timeIdx = 0;
    size_t srcIdx = timeIdx;
    double frequency = 6.25;
    double calculatedSeconds = timeData.getSeconds(timeIdx, frequency);
    EXPECT_EQ(GetSecondsFromIndex(timeIdx, srcIdx, frequency), calculatedSeconds);
}

TEST_F(TimeDataTests, TimeIdx1) {
    size_t timeIdx = 1;
    size_t srcIdx = timeIdx * 2;
    double frequency = 12.5;
    double calculatedSeconds = timeData.getSeconds(srcIdx, frequency);
    EXPECT_EQ(GetSecondsFromIndex(timeIdx, srcIdx, frequency), calculatedSeconds);
}

TEST_F(TimeDataTests, TimeIdx2) {
    size_t timeIdx = 2;
    size_t srcIdx = timeIdx * 160 + 5;
    double frequency = 1000;
    double calculatedSeconds = timeData.getSeconds(srcIdx, frequency);
    double expectedSeconds = GetSecondsFromIndex(timeIdx, srcIdx, frequency);
    // cout << "Test, calculatedSeconds=" << calculatedSeconds 
    //        << ", expectedSeconds=" << expectedSeconds << "\n";
    EXPECT_EQ(expectedSeconds, calculatedSeconds);
    EXPECT_EQ(64530.755, calculatedSeconds);
}

TEST_F(TimeDataTests, TimeIdx3) {
    size_t timeIdx = 3;
    size_t srcIdx = (timeIdx + 1) * 160 + 5;
    double frequency = 1000;
    double calculatedSeconds = timeData.getSeconds(srcIdx, frequency);
    double expectedSeconds = GetSecondsFromIndex(timeIdx, srcIdx, frequency);
    EXPECT_EQ(expectedSeconds, calculatedSeconds);
    //EXPECT_EQ(64530.75, calculatedSeconds);
}

// Test what happens when timeIndex rounds up instead of down.
TEST_F(TimeDataTests, TimeUnderflow) {
    size_t timeIdx = 1;
    size_t srcIdx = timeIdx * 2 - 1;
    double frequency = 12.5;
    double calculatedSeconds = timeData.getSeconds(srcIdx, frequency);
    EXPECT_EQ(GetSecondsFromIndex(timeIdx, srcIdx, frequency), calculatedSeconds);
}

TEST_F(TimeDataTests, DateTimeTest0) {
    tm calculatedTm = timeData.getDateTime(0, 6.25);
    EXPECT_TRUE(CompareDates(calculatedTm, 9, 9, 7, 59, 0));
}

TEST_F(TimeDataTests, DateTimeTest3) {
    tm calculatedTm = timeData.getDateTime(6, 12.5);
    EXPECT_TRUE(CompareDates(calculatedTm, 12, 31, 23, 59, 59));
}
