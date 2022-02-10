#include "gtest/gtest.h"

#include "fileRead.h"

using namespace std;
using namespace gwDainese;


TEST(FileTypeInfoTests, GetExtGood){
    FileTypesData myFileData;
    string actualExt = myFileData.GetExtension(DaineseDataTypes::hhmm);
    ASSERT_EQ(".A0J", actualExt);
}

TEST(FileTypeInfoTests, GetFrequencyGood){
    FileTypesData myFileData;
    double actualFreq = myFileData.GetFrequency (DaineseDataTypes::latitude);
    ASSERT_EQ(12.5, actualFreq);
}

TEST(FileTypeInfoTests, GetFileTypeGood){
    FileTypesData myFileData;
    string actualType = myFileData.GetContentsType (DaineseDataTypes::longitude);
    ASSERT_EQ("double", actualType);
}
