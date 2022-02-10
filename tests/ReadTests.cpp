#include <iostream>
#include <fstream>

#include "gtest/gtest.h"

// #include "plotting.h"
#include "fileRead.h"

using namespace std;
using namespace gwDainese;


bool createTextFile(const string, const string);
string textFileReadTest(const string, const string);

template<typename T> vector<T> numFileReadTest(const string, vector<T>, size_t);
template<typename T> bool createNumFile(const string, const vector<T>);


TEST(FileReadTests, textReadSuccess){
    string fileName = "test.txt";
    string expectedContents = "0123456789\nabccde\nABCDEE\n**&&^%\n";
    ASSERT_EQ(expectedContents, textFileReadTest(fileName, expectedContents));
}

TEST(FileReadTests, textReadFail){
    string fileName = "test2.txt";
    int fileLen = 2048;
    string expectedContents = "0123456789\nabccde\nABCDEE\n**&&^%\n";
        ASSERT_NE(expectedContents, textFileReadTest(fileName, expectedContents + "ZZZ"));
}

TEST(FileReadTests, numReadIntSuccess){
    string fileName = "int.bin";
    vector<int> expectedContents {1, 5, 2134, 321, 888, 543};
    ASSERT_EQ(expectedContents, numFileReadTest<int>(fileName, expectedContents, expectedContents.size()));
}

TEST(FileReadTests, numReadFloatSuccess){
    string fileName = "float.bin";
    vector<float> expectedContents {1.001f, 512343.0f, 2134.299f, 332321.0f, 0.00888f, 543.0f};
    ASSERT_EQ(expectedContents, numFileReadTest<float>(fileName, expectedContents, expectedContents.size()));
}

TEST(FileReadTests, numReadPartialFloatSuccess){
    string fileName = "float.bin";
    vector<float> fullContents {1.001f, 512343.0f, 2134.299f, 332321.0f, 0.00888f, 543.0f};
    vector<float> expectedContents(fullContents.begin(), fullContents.begin()+3);
    ASSERT_EQ(expectedContents, numFileReadTest<float>(fileName, fullContents, expectedContents.size()));
}

TEST(FileReadTests, numReadShortSuccess){
    string fileName = "short.bin";
    vector<short> expectedContents {32767, 5343, 21, 3323, 888, 11111, -56, -32768};
    ASSERT_EQ(expectedContents, numFileReadTest<short>(fileName, expectedContents, expectedContents.size()));
}

TEST(FileReadTests, numReadPartialShortSuccess){
    string fileName = "short.bin";
    vector<short> fullContents {32767, 5343, 21, 3323, 888, 11111, -56, -32768};
    vector<short> expectedContents(fullContents.begin(), fullContents.begin()+5);
    ASSERT_EQ(expectedContents, numFileReadTest<short>(fileName, fullContents, expectedContents.size()));
}

string textFileReadTest(const string fileName, const string fileContents){
    int fileLen = 2048;
    string actualContents = "";
    if (createTextFile(fileName, fileContents)) {
        actualContents = readTextFile(fileName, fileLen);
    }
    else {
        cout << "Failed to create output file.";
    }
    
    return actualContents;
}


// Create file with given name and contents, overwriting if it already exists.
bool createTextFile(const string fileName, const string fileContents){
    ofstream ost(fileName, ios::trunc);
    if (ost.is_open()) {
        ost << fileContents;
        ost.close();
        return true;
    }
    else {
        cout << "can't open output file " << fileName << "\n";
        return false;
    }
}

template<typename T> vector<T> numFileReadTest(const string fileName, vector<T> fileContents, 
                            size_t maxValsToRead) {
    vector<T> actualContents;
    if (createNumFile<T>(fileName, fileContents)) {
        actualContents = readNumericFile<T>(fileName, maxValsToRead);
    }
    else {
        cout << "failed to create output file.";
    }

    return actualContents;
}

// Create binary file with given name and contents, overwriting if it already exists.
// Using write, need to provide memory block, size, where block is assumed to be char*.
// Thus, probably need to create array of int, size of vector, copy ints to that,
// size becomes sizeof(int)*len(vec).
template<typename T> bool createNumFile(const string fileName, const vector<T> fileContents){
    ofstream ost(fileName, ios::trunc|ios::binary);
    if (ost.is_open()) {
        // Copy data to array for writing.
        size_t vecLen = fileContents.size();
        T * numArray = new T [vecLen];
        int i = 0;
        for (auto & next_num: fileContents) {
            numArray[i++] = next_num;
        }

        ost.write((char *) numArray, vecLen * sizeof(T));
        ost.close();
        return true;
    }
    else {
        cout << "can't open output file " << fileName << "\n";
        return false;
    }
}

